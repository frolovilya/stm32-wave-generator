#include "TestSuite.hpp"
#include <iostream>

using namespace std;

void testSuite(const UART &uart, const Oscilloscope &scope) {
  cout << "Testing UART controls\n";
  testUARTCommand(uart, "help", "Usage: sine|square|saw|triangle 20..20000");
  testUARTCommand(uart, "sine 10", "Generating 20Hz sine wave");
  testUARTCommand(uart, "sine 220", "Generating 220Hz sine wave");
  testUARTCommand(uart, "sine 100000", "Generating 20000Hz sine wave");
  testUARTCommand(uart, "saw 440", "Generating 440Hz saw wave");
  testUARTCommand(uart, "square 1000", "Generating 1000Hz square wave");
  testUARTCommand(uart, "triangle 15000", "Generating 15000Hz triangle wave");

  cout << "\nTesting wave generation (tolerance=" << waveFrequencyTolerancePercent << "%)\n";
  testWaveFrequency(uart, scope, "sine", 200);
  testWaveFrequency(uart, scope, "sine", 440);
  testWaveFrequency(uart, scope, "sine", 2000);
  testWaveFrequency(uart, scope, "sine", 5000);

  testWaveFrequency(uart, scope, "square", 200);
  testWaveFrequency(uart, scope, "square", 440);
  testWaveFrequency(uart, scope, "square", 2000);
  testWaveFrequency(uart, scope, "square", 5000);

  testWaveFrequency(uart, scope, "saw", 200);
  testWaveFrequency(uart, scope, "saw", 440);
  testWaveFrequency(uart, scope, "saw", 2000);
  testWaveFrequency(uart, scope, "saw", 5000);

  testWaveFrequency(uart, scope, "triangle", 200);
  testWaveFrequency(uart, scope, "triangle", 440);
  testWaveFrequency(uart, scope, "triangle", 2000);
  testWaveFrequency(uart, scope, "triangle", 5000);
}