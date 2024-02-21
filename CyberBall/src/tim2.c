#include "tim2.h"


extern int values[4];
extern int goal_num = 0;

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
    NVIC -> ISER [0] &= ~(1 << TIM2_IRQn); //disable the interrupt for a moment
    ADC1 -> CR |= ADC_CR_ADSTART;          //start the conversion by setting the ADSTART to 1

    // array to store converted values

    //Warning: May change to interrupt triggered to avoid interrupt happened in side the loop.
    for(int i = 0; i<4; i++)
    {
    	while ((ADC1->ISR & ADC_ISR_EOC) == 0); //{printf("EOC: %d \n", ADC1->ISR & ADC_ISR_EOC);}//wait for converting complete
    	values[i] = ADC1->DR & 0xffff;                  //the converted valued is stored in DR
    }

    printf("sensor 1: %d,  sensor 2: %d,  sensor 3: %d,  sensor 4: %d", values[0], values[1], values[2], values[3]);
    if(values[0]==0 || values[2]==0 || values[3]==0 || values[1] == 0)
    {
    	printf("   detected  \n");
    	goal_num = goal_num + 1;
    	char str[12];
    	sprintf(str, "%d", goal_num);
    	// if detected, then print the number of goal
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	spi1_display1("GOAL!!!");// display goal! to screen
    	for (int i = 0; i < 500; i++){
    		nano_wait(2000000);
    	}

    	spi_cmd(0x01);
    	nano_wait(2000000);
    	spi1_display1("Score: ");   //the next goal can only be detected after 4 secs. (dumb way to debounce)
    	spi1_display2(str);
    	for (int i = 0; i < 1000; i++){
    		nano_wait(2000000);
    	}

    } else {
    	printf("   not detected  \n");
    }

    NVIC -> ISER [0] |= 1 << TIM2_IRQn;


//    uint16_t i;
//    for (i=0; i<5; i++){
////    	printf("data number: %d\n", i);
//    	SPI1->DR = 0x000 | (uint16_t)i;
//    }

}
