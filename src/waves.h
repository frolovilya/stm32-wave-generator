#ifndef __WAVES_H__
#define __WAVES_H__

#include <stddef.h>
#include <stdint.h>

// Using constant sampling rate 48 kHz
#define SAMPLING_RATE 48000

#define MIN_WAVE_FREQUENCY 20
#define MAX_WAVE_FREQUENCY 20000

#define DEFAULT_WAVE_FREQUENCY 440

typedef enum wave_form {
    SIN,
    SQUARE,
    SAW,
    TRIANGLE
} wave_form;

wave_form get_current_wave_form();
uint16_t get_current_frequency();
uint16_t *get_sample_buffer();

size_t generate_wave(wave_form type, uint16_t frequency);

#endif
