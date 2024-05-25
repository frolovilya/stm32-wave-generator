#ifndef WAVES_HPP
#define WAVES_HPP

#include <stddef.h>
#include <stdint.h>

typedef enum WaveForm {
    SINE,
    SQUARE,
    SAW,
    TRIANGLE
} WaveForm;

// Using constant sampling rate 48 kHz
#define SAMPLING_RATE 48000

#define MIN_WAVE_FREQUENCY 20
#define MAX_WAVE_FREQUENCY 20000

#define DEFAULT_WAVE_FORM SINE
#define DEFAULT_WAVE_FREQUENCY 440

WaveForm get_current_wave_form();
uint16_t get_current_frequency();
uint16_t *get_sample_buffer();

char const *wave_form_to_string(WaveForm waveForm);
int string_to_wave_form(const char *str, WaveForm *waveFormPtr);
uint16_t string_to_frequency(const char *str);

size_t generate_wave(WaveForm type, uint16_t frequency);

#endif
