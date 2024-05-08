#include "waves.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.1415926

// Buffer must fit all samples for a whole period of a given frequency
#define SAMPLES_COUNT(F) SAMPLING_RATE / F + 1

// Max signal amplitude (value) for 12-bit DAC
#define AMPLITUDE 0xfff

// Buffer size is fixed to fit all frequencies from MIN_WAVE_FREQUENCY to
// MAX_WAVE_FREQUENCY
static uint16_t sampleBuffer[SAMPLES_COUNT(MIN_WAVE_FREQUENCY)];

static WaveForm currentWaveForm;
static uint16_t currentFrequency;

uint16_t *get_sample_buffer() { return sampleBuffer; }
WaveForm get_current_wave_form() { return currentWaveForm; }
uint16_t get_current_frequency() { return currentFrequency; }

static const struct {
  WaveForm val;
  const char *str;
} wave_form_conversion[] = {
    {SINE, "sine"}, {SQUARE, "square"}, {SAW, "saw"}, {TRIANGLE, "triangle"}};

/**
 * Convert WaveForm enum value to string representation
 *
 * @param waveForm WaveForm enum value
 * @return string representation or NULL if failed to parse
 */
char const *wave_form_to_string(WaveForm waveForm) {
  for (size_t i = 0;
       i < sizeof(wave_form_conversion) / sizeof(wave_form_conversion[0]);
       i++) {
    if (wave_form_conversion[i].val == waveForm) {
      return wave_form_conversion[i].str;
    }
  }

  return NULL;
}

/**
 * Convert given string to WaveForm enum value
 *
 * @param str string to parse
 * @param waveFormPtr pointer to store conversion result
 * @return 1 if successful or 0 otherwise
 */
int string_to_wave_form(char *str, WaveForm *waveFormPtr) {
  for (size_t i = 0;
       i < sizeof(wave_form_conversion) / sizeof(wave_form_conversion[0]);
       i++) {
    if (strcmp(wave_form_conversion[i].str, str) == 0) {
      *waveFormPtr = wave_form_conversion[i].val;
      return 1;
    }
  }

  return 0;
}

/**
 * Convert string to frequency input taking MIN_WAVE_FREQUENCY and
 * MAX_WAVE_FREQUENCY into account
 *
 * @param str input string to parse
 * @return frequency
 */
uint16_t string_to_frequency(char *str) {
  int newFreq = atoi(str);
  if (newFreq < MIN_WAVE_FREQUENCY) {
    return MIN_WAVE_FREQUENCY;
  }
  if (newFreq > MAX_WAVE_FREQUENCY) {
    return MAX_WAVE_FREQUENCY;
  }
  return (uint16_t)newFreq;
}

static void generate_sine_wave(int period) {
  for (int i = 0; i < period; i++) {
    sampleBuffer[i] = (sin(2 * PI * i / period) + 1) * AMPLITUDE / 2;
  }
}

static void generate_square_wave(int period) {
  for (int i = 0; i < period; i++) {
    sampleBuffer[i] = i < period / 2 ? AMPLITUDE : 0;
  }
}

static int modulo(int x, int p) { return ((x % p) + p) % p; }

static void generate_saw_wave(int period) {
  for (int i = 0; i < period; i++) {
    sampleBuffer[i] = AMPLITUDE / period * abs(modulo(i - period / 2, period));
  }
}

static void generate_triangle_wave(int period) {
  for (int i = 0; i < period; i++) {
    sampleBuffer[i] = 2 * AMPLITUDE / period *
                      abs(modulo(i - period / 4, period) - period / 2);
  }
}

/**
 * Generate wave of a given type and frequency
 * and fill the buffer with samples
 *
 * @param type wave form type
 * @param frequency target wave frequency
 * @return generated samples count
 */
size_t generate_wave(WaveForm type, uint16_t frequency) {
  currentWaveForm = type;
  currentFrequency = frequency;

  size_t samplesCount = SAMPLES_COUNT(frequency);
  int period = (int)samplesCount;

  switch (type) {
  case SINE:
    generate_sine_wave(period);
    break;
  case SQUARE:
    generate_square_wave(period);
    break;
  case SAW:
    generate_saw_wave(period);
    break;
  case TRIANGLE:
    generate_triangle_wave(period);
    break;
  }

  return samplesCount;
}