#include "tim2.h"
#include "servo.h"

uint16_t values[3];
uint16_t pos[3];
extern int goal_num = 0;

void init_tim2(void) {
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2 -> PSC = 48000 - 1;
    TIM2 -> ARR = 10 - 1;
    TIM2 -> DIER |= TIM_DIER_UIE;
    TIM2 -> CR1 |= TIM_CR1_CEN;
    NVIC -> ISER [0] |= 1 << TIM2_IRQn;
    NVIC_SetPriority(TIM2_IRQn, 3); //

}

void TIM2_IRQHandler(){

    TIM2 -> SR &= ~TIM_SR_UIF;             //confirm the interrupt
    // NVIC -> ISER [0] &= ~(1 << TIM2_IRQn); //disable the interrupt for a moment
//    ADC1 -> CR |= ADC_CR_ADSTART;          //start the conversion by setting the ADSTART to 1

    // array to store converted values

//    //Warning: May change to interrupt triggered to avoid interrupt happened in side the loop.
//    for(int i = 0; i<4; i++)
//    {
//    	while ((ADC1->ISR & ADC_ISR_EOC) == 0); //{printf("EOC: %d \n", ADC1->ISR & ADC_ISR_EOC);}//wait for converting complete
//    	values[i] = ADC1->DR & 0xffff;                  //the converted valued is stored in DR
//    }
//
//    printf("sensor 1: %d,  sensor 2: %d,  sensor 3: %d,  sensor 4: %d", values[0], values[1], values[2], values[3]);
//    if(values[0]==0 || values[2]==0 || values[3]==0 || values[1] == 0)
//    {
//    	printf("   detected  \n");
//    	goal_num = goal_num + 1;
//    	char str[12];
//    	sprintf(str, "%d", goal_num);
//    	// if detected, then print the number of goal
//    	spi_cmd(0x01);        // clear the screen
//    	nano_wait(2000000);   // wait for the screen to be cleaned
//    	spi2_display1("GOAL!!!");// display goal! to screen
//    	for (int i = 0; i < 500; i++){
//    		nano_wait(2000000);
//    	}
//
//    	spi_cmd(0x01);
//    	nano_wait(2000000);
//    	spi2_display1("Score: ");   //the next goal can only be detected after 4 secs. (dumb way to debounce)
//    	spi2_display2(str);
//    	for (int i = 0; i < 1000; i++){
//    		nano_wait(2000000);
//    	}
//
//    } else {
//    	printf("   not detected  \n");
//    }

    // NVIC -> ISER [0] |= 1 << TIM2_IRQn;


    char upper_data;
    char lower_data;

//    values[0] = ~(GPIOA->IDR);
//    values[1] = ~((GPIOB->IDR) | 0b1111110000000000);
//    values[2] = ~(GPIOC->IDR);
    values[0] = ~(GPIOA->IDR) & 0xFF;	//PA0 to PA7
    values[0] |= (~(GPIOA->IDR) & 0x1000) >> 4;
    values[0] |= (~(GPIOA->IDR) & 0x8000) >> 6;
    values[1] = ~(GPIOB->IDR) & 0x3FF;
    values[2] = ~(GPIOC->IDR) & 0x3FF;
    // Assume at most two elements can be 0.
//    int temp = values[1];
//    for(int i  = 0; i < 10; i++){
//    	printf("%d", temp & 1);
//    	temp = temp >> 1;
//    }
//    printf("%n");
    for(int i = 0; i < 3; i++){
    	pos[i] = 0;
    }

    for(int i = 0; i < 10; i++){
    	for(int j = 0; j < 3; j++){
    		if(values[j] == 0){
    			if(pos[j] == 0)
    				pos[j] = i;
    			else
    				pos[j] = (pos[j] + i)/2.0;
    		}
    	}
    	// Add a break if.
    }


//    printf("Sensor: %d\n", values[1]);

    // GPIOA inputs
//    if(values[0] && 0x0000){
//    	usart_send(USART3, 0x00 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0001){
//    	usart_send(USART3, 0x01 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0003){
//        usart_send(USART3, 0x02 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0002){
//        usart_send(USART3, 0x03 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0006){
//        usart_send(USART3, 0x04 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0004){
//        usart_send(USART3, 0x05 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x000c){
//        usart_send(USART3, 0x06 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0008){
//        usart_send(USART3, 0x07 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0018){
//        usart_send(USART3, 0x08 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0010){
//		usart_send(USART3, 0x09 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0030){
//		usart_send(USART3, 0x0a | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0020){
//		usart_send(USART3, 0x0b | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0060){
//		usart_send(USART3, 0x0c | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0040){
//		usart_send(USART3, 0x0d | (0x00 << 6) );
//    }
//    else if(values[0] && 0x00c0){
//		usart_send(USART3, 0x0e | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0080){
//		usart_send(USART3, 0x0f | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0180){
//    	usart_send(USART3, 0x10 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0100){
//        usart_send(USART3, 0x11 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0300){
//        usart_send(USART3, 0x12 | (0x00 << 6) );
//    }
//    else if(values[0] && 0x0200){
//        usart_send(USART3, 0x13 | (0x00 << 6) );
//    }


    // GPIOB inputs
//    if(values[1] == 0x0000){
////    	usart_send(USART3, 0x00 | (0x01 << 6) );
////    	printf("state0\n");
////    	Servo_control(2, 2);
//    }
//    else if(values[1] == 0x0001){
////    	usart_send(USART3, 0x01 | (0x01 << 6) );
////    	printf("state1\n");
//    	Servo_control(2, 2.5);
//    }
//    else if(values[1] == 0x0003){
////        usart_send(USART3, 0x02 | (0x01 << 6) );
////    	printf("state2\n");
//        Servo_control(2, 3);
//    }
//    else if(values[1] == 0x0002){
////        usart_send(USART3, 0x03 | (0x01 << 6) );
//        Servo_control(2, 3.5);
//    }
//    else if(values[1] == 0x0006){
////        usart_send(USART3, 0x04 | (0x01 << 6) );
//        Servo_control(2, 4);
//    }
//    else if(values[1] == 0x0004){
////        usart_send(USART3, 0x05 | (0x01 << 6) );
//        Servo_control(2, 4.5);
//    }
//    else if(values[1] == 0x000c){
////        usart_send(USART3, 0x06 | (0x01 << 6) );
//        Servo_control(2, 5);
//    }
//    else if(values[1] == 0x0008){
////        usart_send(USART3, 0x07 | (0x01 << 6) );
//        Servo_control(2, 5.5);
//    }
//    else if(values[1] == 0x0018){
////        usart_send(USART3, 0x08 | (0x01 << 6) );
//        Servo_control(2, 6);
//    }
//    else if(values[1] == 0x0010){
////		usart_send(USART3, 0x09 | (0x01 << 6) );
//		Servo_control(2, 6.5);
//    }
//    else if(values[1] == 0x0030){
////		usart_send(USART3, 0x0a | (0x01 << 6) );
//		Servo_control(2, 7);
//    }
//    else if(values[1] == 0x0020){
////		usart_send(USART3, 0x0b | (0x01 << 6) );
//		Servo_control(2, 7.5);
//    }
//    else if(values[1] == 0x0060){
////		usart_send(USART3, 0x0c | (0x01 << 6) );
//		Servo_control(2, 8);
//    }
//    else if(values[1] == 0x0040){
////		usart_send(USART3, 0x0d | (0x01 << 6) );
//		Servo_control(2, 8.5);
//    }
//    else if(values[1] == 0x00c0){
////		usart_send(USART3, 0x0e | (0x01 << 6) );
//		Servo_control(2, 9);
//    }
//    else if(values[1] == 0x0080){
////		usart_send(USART3, 0x0f | (0x01 << 6) );
//		Servo_control(2, 9.5);
//    }
//    else if(values[1] == 0x0180){
////    	usart_send(USART3, 0x10 | (0x01 << 6) );
//    	Servo_control(2, 10);
//    }
//    else if(values[1] == 0x0100){
////        usart_send(USART3, 0x11 | (0x01 << 6) );
//        Servo_control(2, 10.5);
//    }
//    else if(values[1] == 0x0300){
////        usart_send(USART3, 0x12 | (0x01 << 6) );
//        Servo_control(2, 11);
//    }
//    else if(values[1] == 0x0200){
////        usart_send(USART3, 0x13 | (0x01 << 6) );
//        Servo_control(2,11.5);
//    }


    // GPIOC inputs
//    if(values[2] && 0x0000){
//    	usart_send(USART3, 0x00 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0001){
//    	usart_send(USART3, 0x01 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0003){
//        usart_send(USART3, 0x02 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0002){
//        usart_send(USART3, 0x03 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0006){
//        usart_send(USART3, 0x04 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0004){
//        usart_send(USART3, 0x05 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x000c){
//        usart_send(USART3, 0x06 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0008){
//        usart_send(USART3, 0x07 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0018){
//        usart_send(USART3, 0x08 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0010){
//		usart_send(USART3, 0x09 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0030){
//		usart_send(USART3, 0x0a | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0020){
//		usart_send(USART3, 0x0b | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0060){
//		usart_send(USART3, 0x0c | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0040){
//		usart_send(USART3, 0x0d | (0x02 << 6) );
//    }
//    else if(values[2] && 0x00c0){
//		usart_send(USART3, 0x0e | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0080){
//		usart_send(USART3, 0x0f | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0180){
//    	usart_send(USART3, 0x10 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0100){
//        usart_send(USART3, 0x11 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0300){
//        usart_send(USART3, 0x12 | (0x02 << 6) );
//    }
//    else if(values[2] && 0x0200){
//        usart_send(USART3, 0x13 | (0x02  << 6) );
//    }




}
