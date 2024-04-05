#include "servo.h"

void Servo_control(int channel, float duty_cycle)
{
	if(duty_cycle < MIN_Y || duty_cycle > MAX_Y){
//		printf("Error: Duty Cycle out of bounds");
//		printf(" Got %f \n\r", duty_cycle);
		return;
	}
    switch (channel)
    {
    case 0: TIM1->CCR1 = duty_cycle *240;       //TIM1 CH1: PA8
    		printf("Rod 0\n\r");
            break;
    case 1: TIM1->CCR2 = duty_cycle *240;       //TIM1 CH2: PA9
    		printf("Rod 1\n\r");
    		break;
    case 2: TIM1->CCR4 = duty_cycle *240;       //TIM1 CH4: PA10
    		printf("Rod 2\n\r");
    		break;
    case 3: TIM1->CCR3 = duty_cycle *240;       //TIM1 CH3: PA11
            break;
    }
}

// duty_cycle 2 -12, range( maybe 0 - 180 )


float pixelToDutyCycle(int pixelPos){
	return 2 + (float)pixelPos/21 * (MAX_Y - MIN_Y);
}
