#ifndef FREQUENCY_HPP
#define FREQUENCY_HPP

#include <cstdint>
#include <string>

// Using constant sampling rate 48 kHz
constexpr uint16_t samplingRate = 48000;

constexpr uint16_t minWaveFrequency = 20;
constexpr uint16_t maxWaveFrequency = 30000;
constexpr uint16_t defaultWaveFrequency = 440;

uint16_t stringToFrequency(const std::string str);

#endif