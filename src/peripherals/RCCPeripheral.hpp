#ifndef RCCPERIPHERAL_HPP
#define RCCPERIPHERAL_HPP

#include "Peripheral.hpp"
extern "C" {
#include <stm32f446xx.h>
}

class RCCPeripheral : public Peripheral<RCC_TypeDef> {
public:
  void configure() override;
  RCC_TypeDef *getPeripheral() const override;
};

#endif