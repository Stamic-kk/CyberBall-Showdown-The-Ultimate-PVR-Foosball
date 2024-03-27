#include "tim1.h"

void init_tim1(void)
{

	//    Activate the RCC clock to GPIO Port A.
	    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//    Configure the MODER for the four pins to set them for alternate function use.  (PA8,9,10,11 for  TIM1_CH1, TIM1_CH2, TIM1_CH3, and TIM1_CH4.
	    GPIOA->MODER &= ~0xFF0000;
	    // TODO: Then we set them to AF mode
	    GPIOA->MODER |= 0xAA0000;
	//    Set the alternate function register to route the timer function to the external pins.
	    GPIOA->AFR[1] &= ~0xFFFF;    //PA8,9,10,11    clear first
	    //to Set to route the timer function to the external pins. set to to AF2   0010
	    GPIOA->AFR[1] |= 0x2222;

	//    Activate the RCC clock to Timer 1.
	    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	//    You will be using Timer 1 for PWM output rather than interrupt generation.
	//            In order for output to work at all you must enable the MOE bit of the break and dead-time register (BDTR).
	    TIM1->BDTR |= TIM_BDTR_MOE;
	//    Set the prescaler to divide by 1, and set the ARR so that an update event occurs 20000 times per second.
	//    TIM1->PSC = 0;  //original
	    TIM1->PSC = 40-1;   // together output 50hz
	    TIM1->ARR = 24000-1;   // 48M/20000 = 2400,

	//    Configure the output channels of Timer 1:
	//         Configure the “capture/compare mode registers”, CCMR1 and CCMR2, to set channels 1, 2, 3, and 4 for PWM mode 1.
	    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ;   //110 for PWM model 1
	    TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 ;
	    TIM1->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 ;   //110 for PWM model 1
	    TIM1->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 ;
	//         Configure the CCMR2 register to set the “output compare preload enable” bit only for channel 4 (OC4PE bit).
	    TIM1->CCMR2 |=  TIM_CCMR2_OC4PE;
	//         Enable the (uninverted) channel outputs for all four channels by turning on the CC1E, CC2E, etc.
	//         bits for in the “capture/compare enable register”, CCER. Until you do this for each channel, the timer will not affect the outputs.
	    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E  | TIM_CCER_CC3E | TIM_CCER_CC4E ;
	//         Enable the timer.
	    TIM1->CR1 |= TIM_CR1_CEN ;


}

