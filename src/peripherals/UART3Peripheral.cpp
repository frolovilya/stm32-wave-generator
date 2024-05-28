#include "UART3Peripheral.hpp"
#include "Peripherals.hpp"
#include <string>

extern "C" {
#include <stm32f446xx.h>

void USART3_IRQHandler() { uart3Instance.handleInterrupt(); }

#ifdef USE_USART3
// to support printf via USART3
int __io_putchar(int ch) {
  uart3Instance.send(std::string{static_cast<char>(ch)}, false);
  return ch;
}
#endif
}

USART_TypeDef *UART3Peripheral::getPeripheral() const { return USART3; }

/**
 * USART3 is using the following pins:
 * TX: PC10,
 * RX: PC11
 */
void UART3Peripheral::configureGPIO() {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for GPIOC

  // enable alternate function (10) for GPIOC pins PC10 and PC11
  GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
  // select alternate function (0111: AF7)
  GPIOC->AFR[1] |=
      GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;
  GPIOC->AFR[1] |=
      GPIO_AFRH_AFSEL11_0 | GPIO_AFRH_AFSEL11_1 | GPIO_AFRH_AFSEL11_2;
  // set high speed (11) output
  GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
}

void UART3Peripheral::configureRCC() {
  RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable clock
}

void UART3Peripheral::configureNVIC() {
  NVIC_EnableIRQ(USART3_IRQn); // enable interrupts
}
