#include "tim2.h"
#include "spi.h"

extern int values[4];

void init_tim2(void) {
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2 -> PSC = 48000 - 1;
    TIM2 -> ARR = 100 - 1;
    TIM2 -> DIER |= TIM_DIER_UIE;
    TIM2 -> CR1 |= TIM_CR1_CEN;
    NVIC -> ISER [0] |= 1 << TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn, 3); //

}

void TIM2_IRQHandler(){

    TIM2 -> SR &= ~TIM_SR_UIF;             //confirm the interrupt
    ADC1 -> CR |= ADC_CR_ADSTART;          //start the conversion by setting the ADSTART to 1

    // array to store converted values

    //Warning: May change to interrupt triggered to avoid interrupt happened in side the loop.
    for(int i = 0; i<4; i++)
    {
    	while ((ADC1->ISR & ADC_ISR_EOC) == 0); //{printf("EOC: %d \n", ADC1->ISR & ADC_ISR_EOC);}//wait for converting complete
    	values[i] = ADC1->DR & 0xffff;                  //the converted valued is stored in DR
    }
//	int value = ADC1->DR & 0xffff;                  //the converted valued is stored in DR
//	printf("value: %d\n", value);
    printf("sensor 1: %d,  sensor 2: %d,  sensor 3: %d,  sensor 4: %d", values[0], values[1], values[2], values[3]);
    if(values[0]==0 || values[2]==0 || values[3]==0)
    {
    	printf("   detected  \n");
    } else {
    	printf("   not detected  \n");
    }

    //send data to led ( just for testing)
//    spi_cmd(0xff0f);
    uint16_t i;
    for (i=0; i<5; i++){
//    	printf("data number: %d\n", i);
    	SPI1->DR = 0x000 | (uint16_t)i;
    }

}
