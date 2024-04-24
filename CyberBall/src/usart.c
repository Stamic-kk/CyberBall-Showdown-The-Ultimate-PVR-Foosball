#include "usart.h"
#include "servo.h"
#include "tim2.h"
#include "spi.h"

char data;
uint8_t data_int;
float data_float;
extern int number_goal;

void init_usart5() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN|RCC_AHBENR_GPIODEN;
    GPIOC->MODER |= GPIO_MODER_MODER12_1;
    GPIOD->MODER |= GPIO_MODER_MODER2_1;
    GPIOC->AFR[1] |= (0x2 <<(4*(12-8)));
    GPIOD -> AFR[0] |= (0x2 <<(4*2));
    RCC->APB1ENR|= RCC_APB1ENR_USART5EN;
    USART5->CR1 &= ~(USART_CR1_UE);
    USART5->CR1 &= ~(USART_CR1_M|(USART_CR1_M<<16)|USART_CR1_PCE|USART_CR1_OVER8);
    USART5->CR1 &= ~(USART_CR2_STOP);
    USART5->BRR =0x1A1;
    USART5->CR1 |= USART_CR1_TE;
    USART5->CR1 |= USART_CR1_UE;
    while(!(USART5->ISR & USART_ISR_TEACK)){};
//    while(!(USART5->ISR & USART_ISR_REACK)){};
}


void init_usart3(){
//	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//	GPIOC->MODER |= GPIO_MODER_MODER10_1| GPIO_MODER_MODER11_1;
//	GPIOC->AFR[1] &= ~GPIO_AFRH_AFR10;
//	GPIOC->AFR[1] &= ~GPIO_AFRH_AFR11;
//	GPIOC->AFR[1] |= (0x1 << (4*3)) | (0x1 << (4*2));		//AF1 for PC10&11
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    USART3->CR1 &= ~(USART_CR1_UE);
    USART3->CR1 &= ~(USART_CR1_M|(USART_CR1_M<<16)|USART_CR1_PCE|USART_CR1_OVER8);
    USART3->CR1 &= ~(USART_CR2_STOP);
    USART3->BRR =0x1A1;
//    USART3->BRR = 65520;
    USART3->CR1 |= USART_CR1_RE|USART_CR1_TE;
    USART3->CR1 |= USART_CR1_UE;
    while(!(USART3->ISR & USART_ISR_TEACK)){};
    while(!(USART3->ISR & USART_ISR_REACK)){};
    USART3->RQR |= USART_RQR_RXFRQ;
}


char usart_get(USART_TypeDef * u){
	//printf("asdasdasd");
	 while (!(u->ISR & USART_ISR_RXNE)) {
//		 printf("%ld\n", u->ISR);
	 }
	 	 return u->RDR;
}

void usart_send(USART_TypeDef * u, char c){
	while(!(USART5->ISR & USART_ISR_TXE)){}
	u->TDR = c;
}

void setUpSampling(USART_TypeDef * u){
	u->CR1 |= USART_CR1_RXNEIE;
	//setup DMA if necessary
	//maybe we do not have DMA on the board
	NVIC -> ISER[0] |= 1<<USART3_8_IRQn;		//This enables interrupt for both 3 and 5
}

void USART3_4_5_6_7_8_IRQHandler(){
	if((USART3->ISR & USART_ISR_RXNE) ==  USART_ISR_RXNE){

		data_int = usart_get(USART3);
//		if(data_int == 0xFF){
//			char str[12];
//			number_goal = number_goal + 1;
//			sprintf(str, "%d", number_goal);
//			spi_cmd(0x01);        // clear the screen
//			nano_wait(2000000);   // wait for the screen to be cleaned
//			spi2_display1("GOAL!!!");// display goal! to screen
////			printf("GOAL!!!!!!!!!!!!!!!!!!!!!!!!\n");
//			for (int i = 0; i < 500; i++){
//				nano_wait(2000000);
//			}
//
//			spi_cmd(0x01);
//			nano_wait(2000000);
//			spi2_display1("Score: ");   //the next goal can only be detected after 4 secs. (dumb way to debounce)
//			spi2_display2(str);
//		}
		if(data_int == 0xFF){
			// command for kicking the ball
			swing_control = 1;
			return;
		}


		int rod_id = data_int & 0b11000000;
		int pos_hat = data_int - rod_id;
//		if(rod_id == 3){
//			if(pos_hat == 1) Servo_control(3, 2);
//		}
		rod_id = rod_id >> 6;
		int relative_pos = pos_hat % 21;
//		printf("Rod %d Receive %d    Translated to: %d \n", rod_id, pos_hat, relative_pos);
		//Find which player is the closest
		if (0)
		{
			float first_player_at_rod = pos[rod_id] / 10.0 * 21.0;
			int closest_idx = 0;
			float move_to_pos = first_player_at_rod;
			float shortest_distance = 64;
			for(int i = 0; i < 3; i++){
				float distance = pos_hat - move_to_pos;
				if(abs(distance) < abs(shortest_distance)){
					shortest_distance = distance;
					closest_idx = i;
				}
				move_to_pos += 21;
			}
			float relative_pos = first_player_at_rod + shortest_distance;
		}
		//27 	5	26	47
		//		22  1 	-20
		//


		float dutyCycle = pixelToDutyCycle(relative_pos);
//		printf("Duty Cycle: %f \n\r", dutyCycle);
//		printf("Control rod %d: move to %d\n\r", rod_id, relative_pos);
		if(rod_id==1) dutyCycle+= 0.2;
//		if(rod_id == 0) dutyCycle += 0.1;
		Servo_control(rod_id, dutyCycle);
	}
	else if (USART3->ISR & USART_ISR_ORE){
		//if overrun, ignore
		USART3->ICR |= USART_ICR_ORECF;
	}
	else{
		// if USART5 interrupt enabled.
		usart_get(USART5);
	}
}


int __io_putchar(int c) {
    if(c == '\n')
    {
        while(!(USART5->ISR & USART_ISR_TXE));
        USART5->TDR = '\r';
    }
    while(!(USART5->ISR & USART_ISR_TXE));
    USART5->TDR = c;
    return c;
}

int __io_getchar(void) {
    while (!(USART5->ISR & USART_ISR_RXNE));
    char c = USART5->RDR;
    if (c == '\r') c = '\n';
    __io_putchar(c);
    return c;
}
