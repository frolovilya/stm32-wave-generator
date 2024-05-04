#include "uart.h"
#include <stm32f446xx.h>

void configure_uart() {
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable clock for USART2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA

    USART2->CR1 |= USART_CR1_UE; // enable USART2
    USART2->CR1 &= ~USART_CR1_M; // 8-bit data word length
    USART2->CR2 &= ~USART_CR2_STOP; // 1 stop bit
    // baud rate = f_clock / (16 * USARTDIV)
    // USARTDIV = 42MHz / (16 * 115200) = 22,7864
    USART2->BRR = 0x16C; // Mantissa = 22 (0x16); Fraction ~= 16 * 0,7864 = 13 (0xD)
    USART2->CR1 |= USART_CR1_TE; // enable transmitter

    // enable alternate function (10) for GPIOA pins PA2 and PA3
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
    // select alternate function (0111: AF7)
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_2;
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL3_0 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_2;
    // set high speed (11) output
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
}

void send_uart(char *data, size_t dataLength) {
    for(size_t i = 0; i < dataLength; i++) {
        USART2->DR = data[i];
        // wait DR to be ready for next data
        while(!(USART2->SR & USART_SR_TXE));
    }

    // wait for transfer to be completed
    while(!(USART2->SR & USART_SR_TC));
}

// to support printf via UART
int __io_putchar(int ch) {
    send_uart((char *) &ch, 1);

    return ch;
}
