#include "tim2.h"

void init_tim2(void) {
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2 -> PSC = 48000 - 1;
    TIM2 -> ARR = 100 - 1;
    TIM2 -> DIER |= TIM_DIER_UIE;
    TIM2 -> CR1 |= TIM_CR1_CEN;
    NVIC -> ISER [0] |= 1 << TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn, 3);

}

void TIM2_IRQHandler(){
    TIM2 -> SR &= ~TIM_SR_UIF;             //confirm the interrupt
    ADC1 -> CR |= ADC_CR_ADSTART;          //start the conversion by setting the ADSTART to 1
    while ((ADC1->ISR & ADC_ISR_EOC) == 0) //wait for converting complete
    {
        ;
    }
    int value = ADC1->DR & 0xffff;                  //the converted valued is stored in DR
    printf("value: %d\n", value);

}
