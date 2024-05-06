#include "waves.h"
#include <math.h>

#define PI 3.1415926

// Buffer must fit all samples for a whole period of a given frequency
#define SAMPLES_COUNT(F) SAMPLING_RATE / F + 1

// Buffer size is fixed to fit all frequencies from MIN_WAVE_FREQUENCY to MAX_WAVE_FREQUENCY
static uint16_t sampleBuffer[SAMPLES_COUNT(MIN_WAVE_FREQUENCY)];

static uint16_t currentFrequency = 0;

uint16_t get_current_frequency() {
    return currentFrequency;
}

uint16_t *get_sample_buffer() {
    return &sampleBuffer[0];
}

size_t generate_sine_wave(uint16_t frequency) {
    currentFrequency = frequency;
    size_t samplesCount = SAMPLES_COUNT(frequency);

    for (size_t i = 0; i < samplesCount; i++) {
        sampleBuffer[i] = (sin(2 * PI * i / samplesCount) + 1) * 0xfff / 2;
    }

    return samplesCount;
}