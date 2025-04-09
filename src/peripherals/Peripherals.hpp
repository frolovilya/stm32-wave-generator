#ifndef PERIPHERALS_HPP
#define PERIPHERALS_HPP

#include "RCCPeripheral.hpp"
#include "DACPeripheral.hpp"
#include "ADCPeripheral.hpp"
#include "UART2Peripheral.hpp"
#include "UART3Peripheral.hpp"

extern RCCPeripheral rccInstance;
extern DACPeripheral dacInstance;
extern ADCPeripheral adcInstance;
extern UART2Peripheral uart2Instance;
extern UART3Peripheral uart3Instance;

#endif