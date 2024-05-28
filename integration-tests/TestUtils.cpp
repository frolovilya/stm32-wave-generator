#include "TestUtils.hpp"
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

/**
 * Test UART command response
 *
 * @param uart UART handler
 * @param request request string to be sent via UART
 * @param expectedResponse expected command response
 */
void testUARTCommand(const UART &uart, string request,
                     string expectedResponse) {
  uart.send(request);
  string response = uart.receive();

  cout << "request=" << request << ", expected response='" << expectedResponse
       << "', actual response='" << response << "'\n";

  if (response == expectedResponse) {
    cout << "\t(+) passed\n";
  } else {
    cout << "\t(-) failed!\n";
  }
  cout << "-----\n";
}

/**
 * Test generated wave frequency
 *
 * @param uart UART handler
 * @param scope Oscilloscope handler
 * @param waveForm target wave form (sine|square|saw|triangle)
 * @param expectedFrequency expected frequency is passed to the generator as an
 * input and compared with the actual result
 */
void testWaveFrequency(const UART &uart, const Oscilloscope &scope,
                       string waveForm, int expectedFrequency) {
  const string request = waveForm + " " + to_string(expectedFrequency);
  uart.send(request);

  const vector<double> samples = scope.captureSamples();
  double actualFrequency = scope.measureFrequency(samples);

  double frequencyDiff = fabs(actualFrequency - expectedFrequency);
  double diffPercent = frequencyDiff / expectedFrequency * 100;

  cout << "wave=" << waveForm << ", expected frequency=" << expectedFrequency
       << ", actual frequency=" << actualFrequency << ", diff=" << frequencyDiff
       << " (" << diffPercent << "%)\n";

  if (diffPercent < waveFrequencyTolerancePercent) {
    cout << "\t(+) passed\n";
  } else {
    cout << "\t(-) failed!\n";
  }
  cout << "-----\n";
}