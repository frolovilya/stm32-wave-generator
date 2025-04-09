#ifndef FREQUENCY_HPP
#define FREQUENCY_HPP

#include <cstdint>
#include <string>

constexpr uint16_t minWaveFrequency = 20;
constexpr uint16_t maxWaveFrequency = 30000;
constexpr uint16_t defaultWaveFrequency = 440;

uint32_t getSamplingRate();
uint16_t stringToFrequency(const std::string str);

#endif