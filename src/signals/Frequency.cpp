#include "Frequency.hpp"
#include "../peripherals/Peripherals.hpp"

uint32_t getSamplingRate() { return dacInstance.getFrequency(); }

/**
 * Convert string to frequency input taking minWaveFrequency and
 * maxWaveFrequency into account
 *
 * @param str input string to parse
 * @return frequency
 */
uint16_t stringToFrequency(const std::string str) {
  int newFreq = std::stoi(str);
  if (newFreq < minWaveFrequency) {
    return minWaveFrequency;
  }
  if (newFreq > maxWaveFrequency) {
    return maxWaveFrequency;
  }
  return static_cast<uint16_t>(newFreq);
}