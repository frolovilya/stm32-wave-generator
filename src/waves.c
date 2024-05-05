#include "waves.h"
#include <math.h>
#include <stdlib.h>

static uint16_t current_frequency = 0;

uint16_t str_to_freq(char *str) {
    int16_t newFreq = atoi(str);
    if (newFreq < MIN_WAVE_FREQUENCY) {
        return MIN_WAVE_FREQUENCY;
    }
    if (newFreq > MAX_WAVE_FREQUENCY) {
        return MAX_WAVE_FREQUENCY;
    }
    return (uint16_t) newFreq;
}

uint16_t get_current_frequency() {
    return current_frequency;
}

size_t generate_sine_wave(uint16_t frequency, uint16_t *buffer) {
    current_frequency = frequency;
    size_t bufferSize = BUFFER_SIZE(frequency);

    for (size_t i = 0; i < bufferSize; i++) {
        buffer[i] = (sin(2 * M_PI * i / bufferSize) + 1) * 0xfff / 2;
    }

    return bufferSize;
}