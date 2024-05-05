#include "uart.h"

static USART_TypeDef *selectedUart;

static void configure_uart_gpioa() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA

    // enable alternate function (10) for GPIOA pins PA2 and PA3
    GPIOA->MODER |= GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1;
    // select alternate function (0111: AF7)
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2;
    // set high speed (11) output
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3;
}

static void configure_uart_gpioc() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock for GPIOC

    // enable alternate function (10) for GPIOC pins PC10 and PC11
    GPIOC->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1;
    // select alternate function (0111: AF7)
    GPIOC->AFR[1] |= GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;
    GPIOC->AFR[1] |= GPIO_AFRH_AFSEL11_0 | GPIO_AFRH_AFSEL11_1 | GPIO_AFRH_AFSEL11_2;
    // set high speed (11) output
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11;
}

void configure_uart(USART_TypeDef *uartHandler) {
    selectedUart = uartHandler;

    if (uartHandler == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable clock for USART2
        configure_uart_gpioa();
    } else if (uartHandler == USART3) {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable clock for USART3
        configure_uart_gpioc();
    }

    uartHandler->CR1 |= USART_CR1_UE; // enable USART
    uartHandler->CR1 &= ~USART_CR1_M; // 8-bit data word length
    uartHandler->CR2 &= ~USART_CR2_STOP; // 1 stop bit
    // baud rate = f_clock / (16 * USARTDIV)
    // USARTDIV = 42MHz / (16 * 115200) = 22,7864
    uartHandler->BRR = 0x16C; // Mantissa = 22 (0x16); Fraction ~= 16 * 0,7864 = 13 (0xD)
}

void start_uart() {
    selectedUart->CR1 |= USART_CR1_TE; // start transmitter
}

void send_uart(char *data, size_t dataLength) {
    for(size_t i = 0; i < dataLength; i++) {
        selectedUart->DR = data[i];
        // wait DR to be ready for next data
        while(!(selectedUart->SR & USART_SR_TXE));
    }

    // wait for transfer to be completed
    while(!(selectedUart->SR & USART_SR_TC));
}

// to support printf via UART
int __io_putchar(int ch) {
    send_uart((char *) &ch, 1);

    return ch;
}
