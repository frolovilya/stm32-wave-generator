#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <cstdint>
#include <string>

constexpr uint16_t minLevelMV = 1;
uint16_t getMaxLevelMV();
constexpr uint16_t defaultLevelMV = 1000;

uint16_t getPeakToPeak(uint16_t signalLevelMV);
uint16_t stringToLevel(const std::string str);

#endif