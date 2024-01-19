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
#include <stdint.h>

void initb();
void initc();
void setn(int32_t pin_num, int32_t val);
int32_t readpin(int32_t pin_num);
void buttons(void);
void keypad(void);

void mysleep(void) {
    for(int n = 0; n < 1000; n++);
}

int main(void) {
    // Uncomment when most things are working
  //  autotest();

//    initb();
//    initc();

    // uncomment one of the loops, below, when ready
    // while(1) {
    //   buttons();
    // }

    // while(1) {
    //   keypad();
    // }
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~GPIO_MODER_MODER6;
    GPIOC->MODER |= GPIO_MODER_MODER6_0;
    GPIOC -> ODR |= 0x40;

    for(;;);
}

/**
 * @brief Init GPIO port B
 *        Pin 0: input
 *        Pin 4: input
 *        Pin 8-11: output
 *
 */
void initb() {
    int32_t mask = 0x3;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER &= ~mask;
    mask = mask <<8;
    GPIOB->MODER &= ~mask;
    mask = 0x55 <<16;
    GPIOB->MODER &= ~(0xFF<<16);
    GPIOB->MODER |= mask;
}

/**
 * @brief Init GPIO port C
 *        Pin 0-3: inputs with internal pull down resistors
 *        Pin 4-7: outputs
 *
 */
void initc() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    int32_t mask = 0xFF;
    GPIOC->MODER &= ~mask;
    GPIOC->PUPDR &= ~0xFF;
    GPIOC->PUPDR |= 0xAA;
    mask = 0x55 <<8;
    GPIOC->MODER &= ~(0xFF<<8);
    GPIOC->MODER |= mask;
}

/**
 * @brief Set GPIO port B pin to some value
 *
 * @param pin_num: Pin number in GPIO B
 * @param val    : Pin value, if 0 then the
 *                 pin is set low, else set high
 */
void setn(int32_t pin_num, int32_t val) {
    if(val !=0)
    {
        val = 1<<pin_num;
        GPIOB->ODR |= val;
    }
    else
    {
        val = 1<<pin_num;
        GPIOB->BRR |= val;
    }
}

/**
 * @brief Read GPIO port B pin values
 *
 * @param pin_num   : Pin number in GPIO B to be read
 * @return int32_t  : 1: the pin is high; 0: the pin is low
 */
int32_t readpin(int32_t pin_num) {
    int32_t ret = GPIOB->IDR;
    ret = (ret>>pin_num) & 0x1;
    return ret;
}

/**
 * @brief Control LEDs with buttons
 *        Use PB0 value for PB8
 *        Use PB4 value for PB9
 *
 */
void buttons(void) {
    int32_t PB0 = readpin(0);
    setn(8,PB0);
    int32_t PB4 = readpin(4);
    setn(9,PB4);
}

/**
 * @brief Control LEDs with keypad
 *
 */
void keypad(void) {
    for(int i =1; i<5;i++)
    {
        GPIOC->ODR &=0;
        GPIOC->ODR |= (0x1<<(3+i));
        mysleep();
        int32_t in = (GPIOC->IDR & 0xF);
        setn(7+i, in);
    }
}
