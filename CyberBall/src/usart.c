#include "usart.h"
#include "servo.h"
#include "spi.h"

char data;
uint8_t data_int;
float data_float;

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
		int rod_id = data_int & 0b11000000;
		int pos = data_int - rod_id;
		rod_id = rod_id >> 6;
		int relative_pos = pos % 21;
		float dutyCycle = pixelToDutyCycle(relative_pos);
		//data_float = ((float)(data_int) / 2) + 2;
		printf("Duty Cycle: %f \n\r", dutyCycle);
		printf("Control rod %d: move to %d\n\r", rod_id, relative_pos);
		Servo_control(rod_id, dutyCycle);
//		Servo_control(1, 7.5);
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
