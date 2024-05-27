#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "dwf/UART.hpp"
#include "dwf/Oscilloscope.hpp"

constexpr double waveFrequencyTolerancePercent = 5;

void testUARTCommand(const UART &uart, std::string request,
                     std::string expectedResponse);

void testWaveFrequency(const UART &uart, const Oscilloscope &scope,
                       std::string waveForm, int expectedFrequency);              

#endif