#include "peripherals/Peripherals.hpp"
#include "waves/Frequency.hpp"
#include "waves/WaveFactory.hpp"
#include "waves/WaveForm.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

// Max signal amplitude (value) for 12-bit DAC
constexpr double amplitude = 0xfff;

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
      generateWavePeriod<uint16_t>(waveForm, samplingRate, frequency, amplitude);
  printCurrentWaveInfo(waveForm, frequency);

  dacInstance.start(samples.data(), samples.size());
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
#ifdef USE_USART3
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