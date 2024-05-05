#include "timer.h"
#include <stm32f446xx.h>
#include <stdio.h>

void configure_timer() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable TIM2

    // APB1 = 84MHz
    // scaling to 48kHz
    TIM2->PSC = 10-1; // prescaler
    TIM2->ARR = 175-1; // period
    TIM2->CR1 &= ~TIM_CR1_CKD; // clock division by 1

    //TIM2->DIER |= TIM_DIER_UIE; // enable update interrupt
    //NVIC_EnableIRQ(TIM2_IRQn);

    // The update event is selected as trigger output (TRGO)
    TIM2->CR2 &= ~TIM_CR2_MMS;
    TIM2->CR2 |= TIM_CR2_MMS_1;
}

void start_timer() {
    TIM2->CR1 |= TIM_CR1_CEN; // start timer
}

void TIM2_IRQHandler() {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;

        printf("Tick!\n");
    }
}