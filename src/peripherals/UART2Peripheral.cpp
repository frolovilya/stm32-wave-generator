#include "UART2Peripheral.hpp"
#include "Peripherals.hpp"
#include <stm32f446xx.h>

USART_TypeDef *UART2Peripheral::getPeripheral() { return USART2; }

/**
 * USART2 is connected to STLink USB on Nucleo boards.
 * Otherwise using the following pins:
 * TX: PA2
 * RX: PA3
 */
void UART2Peripheral::configureGPIO() {
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA

  // enable alternate function (10) for GPIOA pins PA2 and PA3
  GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
  // select alternate function (0111: AF7)
  GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2;
  GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2;
  // set high speed (11) output
  GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
}

void UART2Peripheral::configureRCC() {
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable clock
}

void UART2Peripheral::configureNVIC() {
  NVIC_EnableIRQ(USART2_IRQn); // enable interrupts
}

void USART2_IRQHandler() { uart2Instance.handleInterrupt(); }

#ifndef USE_UART3
// to support printf via USART2
int __io_putchar(int ch) {
  uart2Instance.send((char *)&ch, 1);
  return ch;
}
#endif