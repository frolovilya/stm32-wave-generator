#include "peripherals/Peripherals.hpp"
#include "signals/Frequency.hpp"
#include "signals/Level.hpp"
#include "signals/SignalFactory.hpp"
#include "signals/WaveForm.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

// Samples buffer
vector<uint16_t> samples;

void printUsageHelp() {
  cout << "Usage: sine|square|saw|triangle frequency(Hz) level(V)\n";
}

void printCurrentSignalInfo(WaveForm waveForm, uint16_t frequency,
                            double level) {
  cout << "Generating " << waveFormToString(waveForm) << " " << frequency
       << "Hz "
       << " " << level << "V"
       << " signal\n";
}

void stream(WaveForm waveForm, uint16_t frequency, double level) {
  samples = generateSignalPeriod<uint16_t>(waveForm, frequency, level);
  printCurrentSignalInfo(waveForm, frequency, level);

  dacInstance.start(samples.data(), samples.size());
}

void parseAndApplyReceivedCommand(std::string str) {
  std::istringstream iss(str);
  std::string item;
  std::vector<std::string> splitString;
  while (std::getline(iss, item, ' ')) {
    std::back_inserter(splitString) = item;
  }

  if (splitString.size() != 3) {
    throw std::invalid_argument(
        "Expecting three input parameters: wave, frequency, level");
  }

  WaveForm waveForm = stringToWaveForm(splitString[0]);
  uint16_t frequency = stringToFrequency(splitString[1]);
  double level = std::stod(splitString[2]);
  stream(waveForm, frequency, level);
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

  adcInstance.configure();

  stream(defaultWaveForm, defaultWaveFrequency, defaultLevelVolts);

  while (1) {
  }

  return 0;
}