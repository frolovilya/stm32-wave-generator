#include "Level.hpp"
#include "../peripherals/Peripherals.hpp"

uint16_t getMaxLevelMV() {
  return adcInstance.getVddaMV() / 2;
}

uint16_t getPeakToPeak(uint16_t signalLevelMV) {
  return signalLevelMV * 2 / getMaxLevelMV() * dacInstance.getMaxValue();
}

uint16_t stringToLevel(const std::string str) {
  int newLevelMV = std::stoi(str);
  if (newLevelMV < minLevelMV) {
    return minLevelMV;
  }
  if (newLevelMV > getMaxLevelMV()) {
    return getMaxLevelMV();
  }
  return static_cast<uint16_t>(newLevelMV);
}