#include "EXTI.h"
#include "spi.h"

void init_exti(){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	// select portB 10, 11, 14 to trigger interrupt
	SYSCFG->EXTICR[2] &= 0x00ff;
	SYSCFG->EXTICR[2] |= 0x1100; //set PB 10, 11

	SYSCFG->EXTICR[3] &= 0xf0ff;
	SYSCFG->EXTICR[3] |= 0x0100; //set PB 14

	//set rising edge trigger
	EXTI->RTSR |= EXTI_RTSR_TR10 | EXTI_RTSR_TR11 | EXTI_RTSR_TR14;

	//unmask the interrupt
	EXTI->IMR  |= EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR14;

	//enable interrupt
	NVIC->ISER[0] |= 0x80;

}

void EXTI4_15_IRQHandler(){
	if( (EXTI->PR & EXTI_PR_PR10) == EXTI_PR_PR10){
		EXTI->PR |= (EXTI_PR_PR10);
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	spi2_display1("Reset game");// display
		printf("Reset game\n");
	}
	else if( (EXTI->PR & EXTI_PR_PR11) == EXTI_PR_PR11){
		EXTI->PR |= EXTI_PR_PR11;
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	spi2_display1("Start");// display
		printf("Start\n");
	}
	else if( (EXTI->PR & EXTI_PR_PR14) == EXTI_PR_PR14){
		EXTI->PR |= EXTI_PR_PR14;
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	spi2_display1("End");// display
		printf("Stop\n");
	}
}
