#ifndef DACPERIPHERAL_HPP
#define DACPERIPHERAL_HPP

#include "Peripheral.hpp"
#include <cstdint>
extern "C" {
#include <stm32f446xx.h>
}

class DACPeripheral : public Peripheral<DAC_TypeDef> {
public:
  void configure() override;
  void start(uint16_t *dacBuffer, int dataLength);
  DAC_TypeDef *getPeripheral() const override;
  uint32_t getFrequency() const;
  uint16_t getMaxValue() const;

private:
  void configureGPIOA();
  void configureDMA();
  void configureTimer();
};

#endif