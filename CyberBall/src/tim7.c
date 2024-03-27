#include "tim2.h"
#include "servo.h"

extern int servo_swing = 0;  //used for TIM7 ISR handler, control servo swinging

void init_tim7(void) {
   // causes the Timer 7 ISR to be invoked at a rate of 2 Hz.
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->PSC =  10000-1;
    TIM7->ARR = 2400-1;
    TIM7->DIER |= TIM_DIER_UIE ;
    TIM7->CR1 |= TIM_CR1_CEN;
    //    Enable the Timer 7 interrupt in the NVIC ISER.
    NVIC->ISER[0] |= 1 << TIM7_IRQn;
}

void TIM7_IRQHandler()
{
    // Remember to acknowledge the interrupt here!
    TIM7->SR &= ~TIM_SR_UIF;
    if (!servo_swing) {
        Servo_control(3, 12);  //Set channel 3 PWM to 12%
        servo_swing = 1;
    }
    else{
        Servo_control(3, 2);
        servo_swing = 0;
    }
}
