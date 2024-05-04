#include "timer.h"
#include <stm32f446xx.h>

void configure_timer() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2

    TIM2->PSC = 10000-1; // prescaler
    TIM2->ARR = 4200-1; // period
    TIM2->CR1 &= TIM_CR1_CKD; // clock division by 1
}