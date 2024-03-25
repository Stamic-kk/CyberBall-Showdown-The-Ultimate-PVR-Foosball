#include "servo.h"

void Servo_control(int channel, float duty_cycle)
{
    switch (channel)
    {
    case 1: TIM1->CCR1 = duty_cycle *240;       //TIM1 CH1: PA8
            break;
    case 2: TIM1->CCR2 = duty_cycle *240;       //TIM1 CH2: PA9
            break;
    case 3: TIM1->CCR3 = duty_cycle *240;       //TIM1 CH3: PA10
            break;
    case 4: TIM1->CCR4 = duty_cycle *240;       //TIM1 CH4: PA11
            break;
    }
}

// duty_cycle 2 -12, range( maybe 0 - 180 )
