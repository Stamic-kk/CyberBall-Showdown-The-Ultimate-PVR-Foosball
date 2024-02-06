#ifndef SPI_H
#define SPI_H



#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h> // for memmove()
#include <stdlib.h>

static char ** vals;
//void spi_display(void);
void init_spi1();
void spi1_init_oled();
void spi1_display1(const char *string);
void spi_cmd(unsigned int data);
void spi_data(unsigned int data);


#endif
