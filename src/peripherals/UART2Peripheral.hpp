#ifndef UART2PERIPHERAL_HPP
#define UART2PERIPHERAL_HPP

#include "UARTPeripheral.hpp"

class UART2Peripheral : public UARTPeripheral {
public:
  void configureGPIO() override;
  void configureRCC() override;
  void configureNVIC() override;
  USART_TypeDef *getPeripheral() const override;
};

#endif