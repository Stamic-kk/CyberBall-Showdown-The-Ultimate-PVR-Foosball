#include "adc.h"
//#include "stm32f0xx.h"
void setup_adc(void) {
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA -> MODER &= ~0x0000000c;
    GPIOA -> MODER |= 0x0000000c;                  //PA1 is used to be the input of ADC1
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC -> CR2 |= RCC_CR2_HSI14ON;
    while ((RCC -> CR2 & RCC_CR2_HSI14RDY) == 0){
        ;
    }
    ADC1 -> CR |= ADC_CR_ADEN;                     //Enable ADC before further config
    while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
        ;
    }
    ADC1 -> CHSELR = ADC_CHSELR_CHSEL1;            //Since PA1 is used, we should select channel 1
    while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
            ;
    }
}
