#include "peripherals/Peripherals.hpp"
#include "waves/Frequency.hpp"
#include "waves/SineGenerator.hpp"
#include "waves/WaveForm.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stm32f446xx.h>
#include <string>

using namespace std;

// Max signal amplitude (value) for 12-bit DAC
constexpr uint16_t amplitude = 0xfff;

static auto waveGenerator = SineGenerator<uint16_t>(samplingRate);

void printUsageHelp() {
  cout << "Usage: sine|square|saw|triangle " << minWaveFrequency << ".."
       << maxWaveFrequency << "\n";
}

void printCurrentWaveInfo(WaveForm waveForm, uint16_t frequency) {
  cout << "Generating " << frequency << "Hz " << waveFormToString(waveForm)
       << " wave\n";
}

void stream(WaveForm waveForm, uint16_t frequency) {
  vector<uint16_t> samples =
      waveGenerator.generatePeriod(frequency, amplitude, 0);
  printCurrentWaveInfo(waveForm, frequency);

  dacInstance.start(samples.data(), static_cast<uint16_t>(samples.size()));
}

void parseAndApplyReceivedCommand(std::string str) {
  std::istringstream iss(str);
  std::string item;
  std::vector<std::string> splitString;
  while (std::getline(iss, item, ' ')) {
    std::back_inserter(splitString) = item;
  }

  if (splitString.size() != 2) {
    throw std::invalid_argument(
        "Expecting two input parameters: wave and frequency");
  }

  WaveForm waveForm = stringToWaveForm(splitString[0]);
  uint16_t frequency = stringToFrequency(splitString[1]);
  stream(waveForm, frequency);
}

void tryParseAndApplyReceivedCommand(std::string str) {
  try {
    parseAndApplyReceivedCommand(str);
  } catch (const std::exception &) {
    printUsageHelp();
  }
}

UARTPeripheral *getUARTPeripheral() {
#ifdef USE_UART3
  return &uart3Instance;
#else
  return &uart2Instance;
#endif
}

int main() {
  rccInstance.configure();

  auto uart = getUARTPeripheral();
  uart->configure();
  uart->start();
  uart->receive(&tryParseAndApplyReceivedCommand);

  dacInstance.configure();

  stream(defaultWaveForm, defaultWaveFrequency);

  while (1) {
  }

  return 0;
}