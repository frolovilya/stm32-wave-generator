#include "peripherals/Peripherals.hpp"
#include "Waves.hpp"
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stm32f446xx.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <string>

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

  dacInstance.start(get_sample_buffer(), samplesCount);
}

void parse_and_apply_received_command(std::string str) {
  std::istringstream iss(str);
  std::string item;
  std::vector<std::string> splitString;
  while (std::getline(iss, item, ' ')) {
    std::back_inserter(splitString) = item;
  }

  if (splitString.size() != 2) {
    print_usage_help();
    return;
  }

  WaveForm waveForm;
  if (!string_to_wave_form(splitString[0].c_str(), &waveForm)) {
    print_usage_help();
    return;
  }

  uint16_t frequency = string_to_frequency(splitString[1].c_str());
  stream(waveForm, frequency);
}

UARTPeripheral *getUARTPeripheral() {
  #ifdef USE_UART3
  return &uart3Instance;
  #else
  return &uart2Instance;
  #endif
}

int main() {
  auto uart = getUARTPeripheral();

  uart->configure();
  uart->start();
  uart->receive(&parse_and_apply_received_command);

  dacInstance.configure();

  stream(DEFAULT_WAVE_FORM, DEFAULT_WAVE_FREQUENCY);

  while (1) {
  }

  return 0;
}