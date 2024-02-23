#include <GPIO_INIT.h>

void init_GPIO_C(void){
	// PC 0-12
	RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER &= 0xfc000000; // clean PC 12-0

	//set PC 0-9 to input mode, PC 10 - 12 to Alternate function mode
	GPIOC->MODER |= 0x02A00000;

	//PC10 USART3_TX alt func 1
	//PC11 USART3_RX alt func 1
	//PC12 USART5_TX alt func 2
	GPIOC->AFR[1] &= ~(0x000fff00);
	GPIOC->AFR[1] |= 0x00021100;

	//PC 0-9 to PULL_UP
	GPIOC->PUPDR &= 0xfff00000;
	GPIOC->PUPDR |= 0x00055555;
}

void init_GPIO_A(void){
	//PA 0 - 12 and PA15
	RCC->AHBENR   |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER  &= 0x3C000000; //clean PA 0-12, clean PA15

	//set PA 0-7 and 12, 15 to input mode, set PA 8-11   alter func mode
	GPIOA->MODER  |= 0x00aa0000;

	//PA8  TIM1_CH1 alt func 2
	//PA9  TIM1_CH2 alt func 2
	//PA10 TIM1_CH3 alt func 2
	//PA11 TIM1_CH4 alt func 2
	GPIOA->AFR[1] &= 0xffff0000;
	GPIOA->AFR[1] |= 0x00002222;

	//PA 0-7, 12, 15 to PULL_UP
	GPIOA->PUPDR &= 0x3cff0000;
	GPIOA->PUPDR |= 0x41005555;

}

void init_GPIO_B(void){
	//PB 0 -15
	RCC->AHBENR  |= RCC_AHBENR_GPIOBEN;
	GPIOB->MODER &= 0x00000000;

	//set PB 0-11, 14 input mod4, 12,13,15 alt func
	GPIOB->MODER |= 0x8a000000;

	//PB12 SPI2_NSS alt func 0
	//PB13 SPI2_SCK alt func 0
	//PB15 SPI2_MOSI alt func 0
	GPIOB->AFR[1] &= 0x0f00ffff;

	//PB 0-9, 10,11, 14 to PULL_UP
	GPIOB->PUPDR &= 0xcf000000;
	GPIOB->PUPDR |= 0x10555555;

}
