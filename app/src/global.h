
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define SAMPLES_PER_SECOND 100
#define SEGMENT_SAMPLES 1024
#define SEGMENT_BYTES (SEGMENT_SAMPLES * sizeof(struct ppg_sample))


#include <zephyr/kernel.h>

struct ppg_sample
{
    int ir;
    int red;
};

struct ppg_result
{
    float heart_rate;
    float spo2;
};

extern struct ppg_sample buffer[];

#endif /* GLOBAL_H_ */