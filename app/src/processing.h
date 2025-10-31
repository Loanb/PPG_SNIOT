#ifndef PROCESSING_H
#define PROCESSING_H
// Ensure ppg_sample is defined for callers
#include "global.h"

float fft_processing(const struct ppg_sample *segment);

#endif // PROCESSING_H
