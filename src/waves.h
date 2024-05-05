#ifndef __WAVES_H__
#define __WAVES_H__

#include <stdint.h>
#include <stddef.h>

#define SAMPLING_RATE 48000
#define MIN_WAVE_FREQUENCY 20
#define MAX_WAVE_FREQUENCY 20000
#define DEFAULT_WAVE_FREQUENCY 440

#define BUFFER_SIZE(F) SAMPLING_RATE / F

uint16_t str_to_freq(char *str);
uint16_t get_current_frequency();
size_t generate_sine_wave(uint16_t frequency, uint16_t *buffer);

#endif
