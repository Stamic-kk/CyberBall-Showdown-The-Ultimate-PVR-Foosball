#ifndef TIM2_H
#define TIM2_H
#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>
#include "spi.h"
#include "usart.h"

void init_tim2();
extern uint16_t pos[3];
#endif
