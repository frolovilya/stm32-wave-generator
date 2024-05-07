#include "waves.h"
#include <math.h>
#include <stdlib.h>

#define PI 3.1415926

// Buffer must fit all samples for a whole period of a given frequency
#define SAMPLES_COUNT(F) SAMPLING_RATE / F + 1

// Max signal amplitude (value) for 12-bit DAC
#define AMPLITUDE 0xfff

// Buffer size is fixed to fit all frequencies from MIN_WAVE_FREQUENCY to
// MAX_WAVE_FREQUENCY
static uint16_t sampleBuffer[SAMPLES_COUNT(MIN_WAVE_FREQUENCY)];

static wave_form currentWaveForm = SIN;
static uint16_t currentFrequency = 0;

wave_form get_current_wave_form() { return currentWaveForm; }
uint16_t get_current_frequency() { return currentFrequency; }
uint16_t *get_sample_buffer() { return &sampleBuffer[0]; }

static void generate_sine_wave(size_t samplesCount) {
  for (size_t i = 0; i < samplesCount; i++) {
    sampleBuffer[i] = (sin(2 * PI * i / samplesCount) + 1) * AMPLITUDE / 2;
  }
}

static void generate_square_wave(size_t samplesCount) {
  for (size_t i = 0; i < samplesCount; i++) {
    sampleBuffer[i] = i < samplesCount / 2 ? AMPLITUDE : 0;
  }
}

static int modulo(int x, int p) {
    return ((x % p) + p) % p;
}

static void generate_saw_wave(size_t samplesCount) {
  int period = (int) samplesCount;
  for (int i = 0; i < (int) samplesCount; i++) {
    sampleBuffer[i] = AMPLITUDE / period 
        * abs(modulo(i - period / 2, period));
  }
}

static void generate_triangle_wave(size_t samplesCount) {
  int period = (int) samplesCount;
  for (int i = 0; i < (int) samplesCount; i++) {
    sampleBuffer[i] = 2 * AMPLITUDE / period
        * abs(modulo(i - period / 4, period) - period / 2);
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
size_t generate_wave(wave_form type, uint16_t frequency) {
  currentWaveForm = type;
  currentFrequency = frequency;
  size_t samplesCount = SAMPLES_COUNT(frequency);

  switch (type) {
  case SIN:
    generate_sine_wave(samplesCount);
    break;
  case SQUARE:
    generate_square_wave(samplesCount);
    break;
  case SAW:
    generate_saw_wave(samplesCount);
    break;
  case TRIANGLE:
    generate_triangle_wave(samplesCount);
    break;
  }

  return samplesCount;
}