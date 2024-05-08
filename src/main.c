#include "peripherals/dac.h"
#include "peripherals/rcc.h"
#include "peripherals/uart.h"
#include "waves.h"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stm32f446xx.h>
#include <string.h>

void print_usage_help() {
  printf("Usage: sine|square|saw|triangle %d..%d\n",
         MIN_WAVE_FREQUENCY, MAX_WAVE_FREQUENCY);
}

void print_current_wave_info() {
  printf("Generating %uHz %s wave\n", get_current_frequency(),
         wave_form_to_string(get_current_wave_form()));
}

void stream(WaveForm waveForm, uint16_t frequency) {
  size_t samplesCount = generate_wave(waveForm, frequency);
  print_current_wave_info();

  start_dac(get_sample_buffer(), samplesCount);
}

void parse_and_apply_received_command(char *str) {
  char *inputStr = strdup(str);

  WaveForm waveForm;
  if (!string_to_wave_form(strtok(inputStr, " "), &waveForm)) {
    print_usage_help();
    return;
  }

  uint16_t frequency = string_to_frequency(strtok(NULL, " "));
  free(inputStr);

  stream(waveForm, frequency);
}

int main() {
  configure_clocks();

  configure_uart();
  start_uart();
  receive_uart(&parse_and_apply_received_command);
  configure_dac();

  stream(DEFAULT_WAVE_FORM, DEFAULT_WAVE_FREQUENCY);

  while (1) {
  }

  return 0;
}