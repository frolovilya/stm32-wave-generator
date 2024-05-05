#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "peripherals/rcc.h"
#include "peripherals/timer.h"
#include "peripherals/uart.h"
#include "peripherals/dac.h"

#define SAMPLING_RATE 48000
#define MIN_WAVE_FREQUENCY 20
#define MAX_WAVE_FREQUENCY 10000

#define BUFFER_SIZE(F) SAMPLING_RATE / F

static uint16_t dacBuffer[BUFFER_SIZE(MIN_WAVE_FREQUENCY)];

size_t generate_sine_wave(uint16_t frequency) {
    size_t bufferSize = BUFFER_SIZE(frequency);

    for (size_t i = 0; i < bufferSize; i++) {
        dacBuffer[i] = (sin(2 * M_PI * i / bufferSize) + 1) * 0xfff / 2;
    }

    return bufferSize;
}

int main()
{
    configure_clocks();
    
    configure_uart(USART2);
    start_uart();

    configure_timer();

    size_t bufferSize = generate_sine_wave(440);

    configure_dac(&dacBuffer[0], bufferSize);

    start_dac();
    start_timer();

    char *msg = "Hello, world!\n";
    printf(msg);

    while(1) {
    }

    return 0;

}