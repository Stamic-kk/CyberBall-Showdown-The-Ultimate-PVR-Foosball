#ifndef TIM7_H
#define TIM7_H
#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>

void init_tim7(void);
void TIM7_IRQHandler();

#endif
