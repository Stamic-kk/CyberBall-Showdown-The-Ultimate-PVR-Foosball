/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "usart.h"
#include <stdint.h>
#include "adc.h"
#include "tim2.h"
#include "spi.h"


#include "fifo.h"
#include "tty.h"
#include <stdio.h>
#include <string.h> // for memmove()
#include <stdlib.h>
// PD2 use for USART RX
// PC12 use for USART TX




int main(void)
{

    init_usart5();
	init_usart3();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    printf("a");
    printf("b");
    printf("c");
//    while(!(USART5->ISR & USART_ISR_TXE)){}
//    while(!(USART3->ISR & USART_ISR_TXE));
//    USART3->TDR = 'R';
//    printf("12345");
    setUpSampling(USART3);
//    while (!((USART3->ISR & USART_ISR_RXNE) == USART_ISR_RXNE)){}
//    char c = usart_get(USART3);
//    printf("%c", c);
//    asm("wfi");
//    printf("asdasd");



//    setup_adc();
    //init_spi1_for_stm32_stm32();
//    init_spi1_for_LCD();
//    spi1_init_oled();
//    spi1_display1("Ult foosball");
//
//    init_tim2();
//

	for(;;);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///**
//  ******************************************************************************
//  * @file    main.c
//  * @author  Ac6
//  * @version V1.0
//  * @date    01-December-2013
//  * @brief   Default main function.
//  ******************************************************************************
//*/
//
//
//#include "stm32f0xx.h"
//#include "fifo.h"
//#include "tty.h"
//#include "usart.h"
//#include <stdio.h>
//#include <string.h> // for memmove()
//#include <stdlib.h>
//#include <stdint.h>
//
//void init_GPIO(){
//	//pa15 SPI1_NSS    pin(24) |   pa15
//	//pa5 SPI1_SCK	   pin(23) |   pa5
//	//pa7 SPI1_MOSI    pin(19) |   pa7
//    RCC->APB2ENR|= RCC_APB2ENR_SPI1EN;
//    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//    GPIOA->MODER &=~(0x3f<<10);
//    GPIOA->MODER |= (0x2A<<10);
//    GPIOA->MODER &=~(0x3<<30);
//    GPIOA->MODER |=(0x2<<30);
//    GPIOA->AFR[0]&=~(0xfff<<(5*4));
//    GPIOA->AFR[1]&=~(0xf<<((15-8)*4));
//}
//
//void init_spi(){
//	RCC->APB2ENR|= RCC_APB2ENR_SPI1EN;
//	SPI1->CR1 &= ~(SPI_CR1_SPE);
//
////	SPI1->CR1 &= ~(SPI_CR1_MSTR); //set the device to slave mode
////	SPI1->CR1 |= SPI_CR1_BR;
////	SPI1->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_3;
////	SPI1->CR2 &= ~(SPI_CR2_DS_1|SPI_CR2_DS_2);
////	SPI1->CR1 |= SPI_CR1_SPE;
//
////	SPI1->CR2 = SPI_CR2_RXNEIE | SPI_CR2_FRXTH
////	 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 | SPI_CR2_DS_3; /* (1) */
//
//	SPI1->CR2 = SPI_CR2_RXNEIE | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 | SPI_CR2_DS_3; /* (1) */
////	SPI1->CR1 |= SPI_CR1_CPOL;
//
//	SPI1->CR1 |= SPI_CR1_SPE; /* (2) */
//
//}
//
//int data_rec(){
//	int data;
//	while (!((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)){}
//	data = (uint16_t)SPI1->DR;
//	return data;
//}
//
//
//int main(void)
//{
//	init_usart5();
//
//	int data = 0;
//	init_GPIO();
//	init_spi();
//    setbuf(stdin,0);
//    setbuf(stdout,0);
//    setbuf(stderr,0);
//	while(1){
//		data = data_rec();
//		printf("data: %d\n", data);
//	}
////    for(int i = 0; i < 10; i++){
////    	printf("Incremental Printout Test %d\n", i);
////    }
//}

