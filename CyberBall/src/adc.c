#include "adc.h"
//#include "stm32f0xx.h"
void setup_adc(void) {
    RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;

    GPIOA -> MODER &= ~0x0000000c;
    GPIOA -> MODER |= 0x0000000c;                  //PA1 is used to be the input of ADC1

    GPIOA -> MODER &= ~0x00000030;
    GPIOA -> MODER |= 0x00000030;                  //PA2 is used as second input of ADC1

    GPIOA -> MODER &= ~0x000003c0;                 //config PA3 and PA4
    GPIOA -> MODER |= 0x000003c0;



    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC -> CR2 |= RCC_CR2_HSI14ON;
    while ((RCC -> CR2 & RCC_CR2_HSI14RDY) == 0){
        ;
    }
    ADC1 -> CR |= ADC_CR_ADEN;                     //Enable ADC before further config
    while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
        ;
    }

    //enable channel 1 and channel 2 for ADC
    ADC1 -> CHSELR |= ADC_CHSELR_CHSEL1;            //Since PA1 is used, we should select channel 1
    while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
            ;
    }
    ADC1 -> CHSELR |= ADC_CHSELR_CHSEL2;            //Since PA2 is used, we should select channel 2
	while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
			;
	}

    ADC1 -> CHSELR |= ADC_CHSELR_CHSEL3;            //Since PA1 is used, we should select channel 3
    while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
            ;
    }
    ADC1 -> CHSELR |= ADC_CHSELR_CHSEL4;            //Since PA2 is used, we should select channel 4
	while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
			;
	}

	//set scan sequence direction
	ADC1 -> CFGR1 = ADC_CFGR1_SCANDIR;
	while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
			;
	}

//	ADC1 -> CFGR1 |= 0x00001000;
//	while ((ADC1 -> ISR & ADC_ISR_ADRDY) == 0){
//			;
//	}
}
