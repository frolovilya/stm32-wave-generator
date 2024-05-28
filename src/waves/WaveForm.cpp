#include "WaveForm.hpp"
#include <stdexcept>

static const struct {
  WaveForm val;
  std::string str;
} toStringConversion[] = {{WaveForm::SINE, "sine"},
                          {WaveForm::SQUARE, "square"},
                          {WaveForm::SAW, "saw"},
                          {WaveForm::TRIANGLE, "triangle"}};

/**
 * Convert WaveForm enum value to string representation
 *
 * @param waveForm WaveForm enum value
 * @return string representation or logic_error exception is thrown if failed to
 * convert
 */
std::string waveFormToString(const WaveForm waveForm) {
  for (unsigned int i = 0;
       i < sizeof(toStringConversion) / sizeof(toStringConversion[0]); i++) {
    if (toStringConversion[i].val == waveForm) {
      return toStringConversion[i].str;
    }
  }

  throw std::logic_error("Unable to convert WaveForm enum to string");
}

/**
 * Convert given string to WaveForm enum value
 *
 * @param str string to parse
 * @param waveFormPtr pointer to store conversion result
 * @return converted waveform or invalid_argument exception is thrown if no wave
 * form with such name found
 */
WaveForm stringToWaveForm(const std::string str) {
  for (unsigned int i = 0;
       i < sizeof(toStringConversion) / sizeof(toStringConversion[0]); i++) {
    if (toStringConversion[i].str == str) {
      return toStringConversion[i].val;
    }
  }

  throw std::invalid_argument("Unable to convert string to WaveForm enum");
}
