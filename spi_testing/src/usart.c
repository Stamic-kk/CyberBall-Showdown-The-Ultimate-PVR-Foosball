#include "usart.h"

void init_usart5() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIODEN;
    GPIOC->MODER |= GPIO_MODER_MODER12_1;
    GPIOD->MODER |= GPIO_MODER_MODER2_1;
    GPIOC->AFR[1] |= (0x2 <<(4*(12-8)));
    GPIOD -> AFR[0] |= (0x2 <<(4*2));
    RCC->APB1ENR|= RCC_APB1ENR_USART5EN;
    USART5->CR1 &= ~(USART_CR1_UE);
    USART5->CR1 &= ~(USART_CR1_M|(USART_CR1_M<<16)|USART_CR1_PCE|USART_CR1_OVER8);
    USART5->CR1 &= ~(USART_CR2_STOP);
    USART5->BRR =0x1A1;
    USART5->CR1 |= USART_CR1_TE|USART_CR1_RE;
    USART5->CR1 |= USART_CR1_UE;
    while(!(USART5->ISR & USART_ISR_TEACK)){};
    while(!(USART5->ISR & USART_ISR_REACK)){};
}



int __io_putchar(int c) {
    if(c == '\n')
    {
        while(!(USART5->ISR & USART_ISR_TXE));
        USART5->TDR = '\r';
    }
    while(!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
    while (!(USART5->ISR & USART_ISR_RXNE));
    char c = USART5->RDR;
    if (c == '\r') c = '\n';
    __io_putchar(c);
    return c;
}
