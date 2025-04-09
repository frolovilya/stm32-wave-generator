#include "ADCPeripheral.hpp"
#include <iostream>

ADC_TypeDef *ADCPeripheral::getPeripheral() const { return ADC1; }

const uint16_t *vRefIntCalibratedAddress = reinterpret_cast<uint16_t *>(0x1FFF7A2A);
constexpr double vRefIntCalibratedViltage = 3.3;

void ADCPeripheral::configure() {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // enable ADC clock
    ADC1->CR2 |= ADC_CR2_ADON; // power on ADC
    ADC->CCR |= ADC_CCR_TSVREFE; // enable VREFINT reading
   
    ADC1->SQR1 &= ~ADC_SQR1_L; // 1 conversion
    ADC1->SQR3 = 17; // VREFINT -> ADC1_IN17 channel
}

/**
 * Returns internal Vref for ADC.
 * Can be used to calculate Vdda.
 */
uint16_t ADCPeripheral::getVRefInt() {
    ADC1->CR2 |= ADC_CR2_SWSTART; // start ADC

    // wait for ADC reading to finish
    while (!(ADC1->SR & ADC_SR_EOC))
    ;

    // The converted data are stored into the 16-bit ADC_DR register
    return ADC1->DR;
}

/**
 * Raw data acquired at temperature of 30 °C and VDDA = 3.3 V
 * Fixed memory address for STM32F446RE: 0x1FFF7A2A - 0x1FFF7A2B
 */
uint16_t ADCPeripheral::getVRefIntCalibrated() const {
    // Internal reference voltage calibration value at 3.3 Vdda
    return *vRefIntCalibratedAddress;
}

/**
 * Calculate Vdda based on Vrefint and Vrefint_calibrated
 */
double ADCPeripheral::getVdda() {
    return vRefIntCalibratedViltage * getVRefIntCalibrated() / getVRefInt();
}