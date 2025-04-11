#include "CommandParser.hpp"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

void printUsageHelp() {
  cout << "Usage: sine|square|saw|triangle [" << minWaveFrequency << ".."
       << maxWaveFrequency << "](Hz) [" << minLevelMV << ".." << getMaxLevelMV()
       << "](mV)\n";
}

void printCurrentSignalInfo(WaveForm waveForm, uint16_t frequency,
                            uint16_t level) {
  cout << "Generating " << waveFormToString(waveForm) << " " << frequency
       << "Hz "
       << " " << level << "mV"
       << " signal\n";
}

std::tuple<WaveForm, uint16_t, uint16_t> parseCommand(std::string str) {
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
  uint16_t level = stringToLevel(splitString[2]);

  return {waveForm, frequency, level};
}

constexpr std::tuple<WaveForm, uint16_t, uint16_t> defaultCommand() {
  return {defaultWaveForm, defaultWaveFrequency, defaultLevelMV};
}

std::tuple<WaveForm, uint16_t, uint16_t> tryParseCommand(std::string str) {
  try {
    return parseCommand(str);
  } catch (const std::exception &) {
    printUsageHelp();
    return defaultCommand();
  }
}