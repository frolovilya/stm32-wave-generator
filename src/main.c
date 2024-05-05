#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "peripherals/rcc.h"
#include "peripherals/timer.h"
#include "peripherals/uart.h"
#include "peripherals/dac.h"
#include "waves.h"

static uint16_t dacBuffer[BUFFER_SIZE(MIN_WAVE_FREQUENCY)];

void print_current_frequency() {
    printf("Freq: %u\n", get_current_frequency());
}

void uart_callback(char *str) {
    size_t bufferSize = generate_sine_wave(str_to_freq(str), &dacBuffer[0]);
    print_current_frequency();

    start_dac(&dacBuffer[0], bufferSize);
}

int main()
{
    configure_clocks();
    
    configure_uart(USART3);
    start_uart();
    receive_uart(&uart_callback);

    configure_timer();
    configure_dac();

    size_t bufferSize = generate_sine_wave(DEFAULT_WAVE_FREQUENCY, &dacBuffer[0]);

    start_dac(&dacBuffer[0], bufferSize);
    start_timer();

    print_current_frequency();

    while(1) {
    }

    return 0;

}