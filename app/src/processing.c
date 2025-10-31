/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 * SPDX-License-Identifier: Apache-2.0
 *
 * Contains all logic for processing PPG sensor data, including FFT.
 */

#include <zephyr/logging/log.h>
#include <string.h>
#include <math.h>
#include "processing.h"
#include "global.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

#define FFT_SIZE SEGMENT_SAMPLES
#define FFT_BINS ((FFT_SIZE / 2) + 1)
#define MIN_FREQ_HZ 0.5f
#define MAX_FREQ_HZ 3.0f
#define FFT_LOGN 10 // log2(1024) = 10

// Hann window amplitude correction factor (compensates for window energy loss)
#define HANN_AMPLITUDE_CORRECTION 2.0f

LOG_MODULE_REGISTER(processing, CONFIG_APP_LOG_LEVEL);

// Custom complex type
typedef struct
{
    float r;
    float i;
} cpx;

// buffers for FFT
static cpx fft_complex[FFT_SIZE];
static float fft_magnitudes[FFT_BINS];

static cpx cpx_add(cpx a, cpx b)
{
    cpx res = {a.r + b.r, a.i + b.i};
    return res;
}

static cpx cpx_sub(cpx a, cpx b)
{
    cpx res = {a.r - b.r, a.i - b.i};
    return res;
}

static cpx cpx_mult(cpx a, cpx b)
{
    cpx res = {a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r};
    return res;
}

static int bit_reverse(int idx, int logn)
{
    int rev = 0;
    for (int b = 0; b < logn; b++)
    {
        rev = (rev << 1) | (idx & 1);
        idx >>= 1;
    }
    return rev;
}

static void custom_fft(cpx *X, int N, int logn)
{
    // Bit reversal permutation
    for (int i = 0; i < N; i++)
    {
        int j = bit_reverse(i, logn);
        if (i < j)
        {
            cpx temp = X[i];
            X[i] = X[j];
            X[j] = temp;
        }
    }

    // Butterfly stages
    for (int s = 1; s <= logn; s++)
    {
        int m = 1 << s;
        int m2 = m / 2;
        float angle_step = -2.0f * (float)M_PI / (float)m;
        for (int k = 0; k < N; k += m)
        {
            cpx w = {1.0f, 0.0f}; // Start w at 1 + 0i
            for (int j = 0; j < m2; j++)
            {
                cpx u = X[k + j];
                cpx t = cpx_mult(w, X[k + j + m2]);
                X[k + j] = cpx_add(u, t);
                X[k + j + m2] = cpx_sub(u, t);
                // Rotate w by angle_step
                float cos_a = cosf(angle_step);
                float sin_a = sinf(angle_step);
                w = cpx_mult(w, (cpx){cos_a, sin_a});
            }
        }
    }
}

/**
 * Process PPG segment with FFT and return dominant frequency
 *
 * @param segment Array of PPG samples
 * @return Dominant frequency in Hz, or 0.0 if no peak found
 */
float fft_processing(const struct ppg_sample *segment)
{
    const float Nf = (float)FFT_SIZE;
    const float fs = (float)SAMPLES_PER_SECOND;
    const float bin_hz = fs / Nf;

    memset(fft_complex, 0, sizeof(fft_complex));
    memset(fft_magnitudes, 0, sizeof(fft_magnitudes));

    float mean = 0.0f;
    for (int i = 0; i < FFT_SIZE; ++i)
    {
        float sample = (float)segment[i].ir;
        mean += sample;
    }
    mean /= Nf;

    for (int i = 0; i < FFT_SIZE; ++i)
    {
        fft_complex[i].r = (float)segment[i].ir - mean;
        fft_complex[i].i = 0.0f;
    }

    // Apply Hann window
    for (int i = 0; i < FFT_SIZE; ++i)
    {
        float cos_arg = 2.0f * (float)M_PI * (float)i / (Nf - 1.0f);
        float w = 0.5f * (1.0f - cosf(cos_arg));
        fft_complex[i].r *= w;
    }

    custom_fft(fft_complex, FFT_SIZE, FFT_LOGN);

    for (int k = 0; k < FFT_BINS; ++k)
    {
        // Normalize by N
        float re = fft_complex[k].r / Nf;
        float im = fft_complex[k].i / Nf;
        fft_magnitudes[k] = re * re + im * im;
        fft_magnitudes[k] *= (HANN_AMPLITUDE_CORRECTION * HANN_AMPLITUDE_CORRECTION);
    }

    int min_bin = (int)ceilf(MIN_FREQ_HZ / bin_hz);
    int max_bin = (int)floorf(MAX_FREQ_HZ / bin_hz);

    if (min_bin < 1)
        min_bin = 1; // Start from bin 1 (ignore DC)
    if (max_bin >= FFT_BINS)
        max_bin = FFT_BINS - 1;
    if (min_bin > max_bin)
        return 0.0f; // Invalid range

    float max_mag = 0.0f;
    int max_bin_idx = 0;

    for (int k = min_bin; k <= max_bin; ++k)
    {
        if (fft_magnitudes[k] > max_mag)
        {
            max_mag = fft_magnitudes[k];
            max_bin_idx = k;
        }
    }

    if (max_mag <= 0.0f || max_bin_idx == 0)
        return 0.0f;

    return (float)max_bin_idx * bin_hz;
}
