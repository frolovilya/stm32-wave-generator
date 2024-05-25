#ifndef UART3PERIPHERAL_HPP
#define UART3PERIPHERAL_HPP

#include "UARTPeripheral.hpp"

class UART3Peripheral : public UARTPeripheral {
public:
  void configureGPIO() override;
  void configureRCC() override;
  void configureNVIC() override;
  USART_TypeDef *getPeripheral() const override;
};

#endif