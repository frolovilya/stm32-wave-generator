#include "dac.h"
#include <stm32f446xx.h>
#include <stdio.h>

static void configure_dac_gpioa() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock for GPIOA

    GPIOA->MODER |= GPIO_MODER_MODER4; // analog mode (11) for GPIOA pin PA4
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4; // no pull
}

static void configure_dac_dma() {
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN; // enable clock for DMA1

    // using DMA Stream 5
    DMA1_Stream5->CR |= DMA_SxCR_CHSEL; // use Channel 7
    DMA1_Stream5->CR |= DMA_SxCR_DBM; // enable double-buffering
    DMA1_Stream5->CR |= DMA_SxCR_PL; // very high priority
    // half-word data size
    DMA1_Stream5->CR &= ~DMA_SxCR_PSIZE_1;
    DMA1_Stream5->CR |= DMA_SxCR_PSIZE_0;
    DMA1_Stream5->CR &= ~DMA_SxCR_MSIZE_1;
    DMA1_Stream5->CR |= DMA_SxCR_MSIZE_0;
    DMA1_Stream5->CR |= DMA_SxCR_MINC; // enable memory increment
    DMA1_Stream5->CR &= ~DMA_SxCR_PINC; // disable peripheral increment
    DMA1_Stream5->CR |= DMA_SxCR_CIRC; // circular mode enabled
    // memory to peripheral direction
    DMA1_Stream5->CR &= ~DMA_SxCR_DIR_1;
    DMA1_Stream5->CR |= DMA_SxCR_DIR_0; 
    DMA1_Stream5->CR |= DMA_SxCR_TCIE; // transfer interrupt enable
    DMA1_Stream5->CR |= DMA_SxCR_HTIE; // half transfer interrupt enable
    DMA1_Stream5->CR |= DMA_SxCR_TEIE; // transfer error interrupt enable

    // peripheral address
    DMA1_Stream5->PAR = (uint32_t) &(DAC->DHR12R1);

    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

void configure_dac() {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN; // enable DAC clock

    DAC->CR |= DAC_CR_EN1; // enable Channel 1
    DAC->CR &= ~DAC_CR_BOFF1; // enable output buffer
    DAC->CR |= DAC_CR_TEN1; // enable trigger
    // use TIM2 Update event as a trigger
    DAC->CR &= ~DAC_CR_TSEL1;
    DAC->CR |= DAC_CR_TSEL1_2;
    DAC->CR &= ~DAC_CR_WAVE1; // disable built-in wave generation
    DAC->CR |= DAC_CR_DMAUDRIE1; // enable DMA Underran interrupt
    DAC->CR |= DAC_CR_DMAEN1; // enable DMA
    
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    configure_dac_gpioa();
    configure_dac_dma();
}

void start_dac(uint16_t *dacBuffer, uint16_t dataLength) {
    DMA1_Stream5->CR &= ~DMA_SxCR_EN; // stop DMA

    // memory address
    DMA1_Stream5->NDTR = dataLength / 2;
    DMA1_Stream5->M0AR = (uint32_t) dacBuffer;
    DMA1_Stream5->M1AR = (uint32_t) (dacBuffer + dataLength / 2);

    DMA1_Stream5->CR |= DMA_SxCR_EN; // start DMA
}

void TIM6_DAC_IRQHandler() {
    if (DAC->SR & DAC_SR_DMAUDR1) {
        printf("DMA1 Underrun\n");

        // The software should clear the DMAUDRx flag
        DAC->SR &= ~DAC_SR_DMAUDR1;

        // clear the DMAEN bit of the used DMA stream 
        DAC->CR &= ~DAC_CR_DMAEN1;

        // and re-initialize both DMA and DAC channelx to restart the transfer correctly
        DAC->CR &= ~DAC_CR_EN1;

        DMA1_Stream5->CR &= ~DMA_SxCR_EN;
        DMA1_Stream5->CR |= DMA_SxCR_EN;

        DAC->CR |= DAC_CR_DMAEN1;
        DAC->CR |= DAC_CR_EN1;
    }
}

void DMA1_Stream5_IRQHandler() {
    if (DMA1->HISR & DMA_HISR_TCIF5) {
        //printf("Transfer completed\n");
        DMA1->HIFCR |= DMA_HIFCR_CTCIF5; // clear flag
    }
    if (DMA1->HISR & DMA_HISR_HTIF5) {
        //printf("Half transfer completed\n");
        DMA1->HIFCR |= DMA_HIFCR_CHTIF5; // clear flag
    }

    //printf("CT = %d", (DMA1_Stream5->CR & DMA_SxCR_CT) == 0);
}