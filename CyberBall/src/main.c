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
// PD2 use for USART RX
// PC12 use for USART TX




int main(void)
{
    init_usart5();
    setup_adc();
    init_tim2();

    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);

//    for(int i = 0; i < 10; i++){
//    	printf("Incremental Printout Test %d\n", i);
//    }
}

