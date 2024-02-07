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
//	//pa4 SPI1_NSS     |   pa15
//	//pa5 SPI1_SCK	   |   pa5
//	//pa7 SPI1_MOSI    |   pa6
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
//	SPI1->CR1 &= ~(SPI_CR1_MSTR); //set the device to slave mode
//	SPI1->CR1 |= SPI_CR1_BR;
//	SPI1->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_3;
//	SPI1->CR2 &= ~(SPI_CR2_DS_1|SPI_CR2_DS_2);
//
//	SPI1->CR1 |= SPI_CR1_SPE;
//}
//
//int data_rec(){
//	int data;
//	while(SPI1->SR & SPI_SR_BSY){}
//	data = SPI1->DR;
//	return data;
//}
//
//
//int main(void)
//{
//	init_usart5();
//
//	int data = 0;
////	init_GPIO();
////	init_spi();
//    setbuf(stdin,0);
//    setbuf(stdout,0);
//    setbuf(stderr,0);
////	while(1){
////		printf("data: \n");
//////		data = data_rec();
//////		printf("data: %d\n", data);
////	}
//    for(int i = 0; i < 10; i++){
//    	printf("Incremental Printout Test %d\n", i);
//    }
//}
//
//

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
//    setup_adc();
//    init_tim2();
//    //spi_display();

    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);

    for(int i = 0; i < 10; i++){
    	printf("Incremental Printout Test %d\n", i);
    }
	for(;;);
}


