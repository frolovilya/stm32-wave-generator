#include "TestSuite.hpp"
#include "dwf/DwfDeviceHandler.hpp"
#include "dwf/Oscilloscope.hpp"
#include "dwf/UART.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

int main() {
  try {
    DwfDeviceHandler deviceHandler = DwfDeviceHandler::openFirst();
    UART uart =
        UART(deviceHandler, defaultBaudRate, defaultTxPin, defaultRxPin);
    Oscilloscope scope =
        Oscilloscope(deviceHandler, defaultScopeFrequency, defaultScopeChannel,
                     defaultSampleBufferSize);

    testSuite(uart, scope);

    deviceHandler.close();

  } catch (const std::runtime_error &e) {
    cout << e.what() << "\n";
  }

  return 0;
}