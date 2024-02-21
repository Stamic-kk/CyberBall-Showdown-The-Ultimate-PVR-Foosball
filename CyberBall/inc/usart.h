#ifndef USART_H
#define USART_H
#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>
void init_usart5();
void init_usart3();
char usart_get(USART_TypeDef * u);
void setUpSampling(USART_TypeDef * u);
extern char data;
#endif
