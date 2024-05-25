#ifndef DACPERIPHERAL_HPP
#define DACPERIPHERAL_HPP

#include "Peripheral.hpp"
#include <cstdint>
#include <stm32f446xx.h>

class DACPeripheral : public Peripheral<DAC_TypeDef> {
public:
  void configure() override;
  void start(uint16_t *dacBuffer, uint16_t dataLength);
  DAC_TypeDef *getPeripheral() const override;

private:
  void configureGPIOA();
  void configureDMA();
  void configureTimer();
};

#endif