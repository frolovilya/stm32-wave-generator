#include "UART.hpp"
#include "dwf.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace std;

constexpr int rxBufferSize = 100;

UART::UART(const DwfDeviceHandler &deviceHandler, int baudRate, int txPin, int rxPin)
    : deviceHandler{deviceHandler}, baudRate{baudRate}, txPin{txPin}, rxPin{rxPin} {
  configure();
}

/**
 * Configure UART protocol for transmitting and receiving data
 */
void UART::configure() {
  cout << "Configuring UART (pins TX=" << txPin << ", RX=" << rxPin << ")\n";

  auto device = deviceHandler.getDevice();

  FDwfDigitalUartRateSet(device, baudRate);
  FDwfDigitalUartBitsSet(device, 8);
  FDwfDigitalUartParitySet(device, 0);
  FDwfDigitalUartStopSet(device, 1);
  FDwfDigitalUartTxSet(device, txPin);
  FDwfDigitalUartRxSet(device, rxPin);

  int responseSize = 0;
  int responseParity = 0;
  char *dummy_buffer = nullptr;

  // initialize TX, drive with idle level
  FDwfDigitalUartTx(device, dummy_buffer, 0);
  // initialize RX reception
  FDwfDigitalUartRx(device, dummy_buffer, 0, &responseSize,
                    &responseParity);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

/**
 * Send a message via UART protocol
 */
void UART::send(string message) const {
  message += "\n";

  if (!FDwfDigitalUartTx(deviceHandler.getDevice(), message.data(), message.length())) {
    throw runtime_error(deviceHandler.getLastError("Error sending UART message"));
  }
  cout << "Sent: " << message;

  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

/**
 * Receive a message from UART
 *
 * @param device DWF device handler
 * @return received string data buffer (must be freed after usage) or NULL if
 * error
 */
string UART::receive() const {
  char rxBuffer[rxBufferSize];
  int responseSize = 0;
  int responseParity = 0;
  while (responseSize == 0) {
    // read up to rxBufferSize - 1 to leave space for \0 string null temination
    if (!FDwfDigitalUartRx(deviceHandler.getDevice(), rxBuffer, rxBufferSize - 1, &responseSize,
                           &responseParity)) {
      throw runtime_error(deviceHandler.getLastError("Error receiving UART data"));
    }
  }

  rxBuffer[responseSize] = '\0';
  string responseString(rxBuffer, rxBuffer + responseSize);
    
  cout << "Received: " << responseString << "\n";

  return responseString;
}