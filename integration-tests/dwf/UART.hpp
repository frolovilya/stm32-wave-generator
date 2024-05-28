#ifndef UART_HPP
#define UART_HPP

#include "DwfDeviceHandler.hpp"
#include <string>

constexpr int defaultBaudRate = 115200;
constexpr int defaultTxPin = 0;
constexpr int defaultRxPin = 1;

class UART {
public:
  UART(const DwfDeviceHandler &deviceHandler, int baudRate, int txPin, int rxPin);

  void send(std::string message, bool appendNewLine = true) const;
  std::string receive() const;

private: 
  const DwfDeviceHandler &deviceHandler;
  const int baudRate;
  const int txPin;
  const int rxPin;

  void configure();
};

#endif
