#ifndef ADCPERIPHERAL_HPP
#define ADCPERIPHERAL_HPP

#include "Peripheral.hpp"
#include <cstdint>
extern "C" {
#include <stm32f446xx.h>
}

class ADCPeripheral : public Peripheral<ADC_TypeDef> {
public:
  void configure() override;
  ADC_TypeDef *getPeripheral() const override;
  uint16_t getVddaMV();

private:
  void configureGPIOA();
  void configureDMA();
  void configureTimer();
  uint16_t getVRefInt();
  uint16_t getVRefIntCalibrated() const;
};

#endif