#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include "peripherals/rcc.h"
#include "peripherals/uart.h"
#include "peripherals/dac.h"
#include "waves.h"

void print_current_frequency() {
    printf("Freq: %u\n", get_current_frequency());
}

uint16_t str_to_freq(char *str) {
    int16_t newFreq = atoi(str);
    if (newFreq < MIN_WAVE_FREQUENCY) {
        return MIN_WAVE_FREQUENCY;
    }
    if (newFreq > MAX_WAVE_FREQUENCY) {
        return MAX_WAVE_FREQUENCY;
    }
    return (uint16_t) newFreq;
}

void uart_data_received_callback(char *str) {
    size_t samplesCount = generate_sine_wave(str_to_freq(str));
    print_current_frequency();

    start_dac(get_sample_buffer(), samplesCount);
}

int main()
{
    configure_clocks();
    
    configure_uart(USART3);
    start_uart();
    receive_uart(&uart_data_received_callback);

    configure_dac();

    size_t samplesCount = generate_sine_wave(DEFAULT_WAVE_FREQUENCY);

    start_dac(get_sample_buffer(), samplesCount);

    print_current_frequency();

    while(1) {
    }

    return 0;

}