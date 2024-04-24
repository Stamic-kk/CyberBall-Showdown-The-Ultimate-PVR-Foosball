#include "EXTI.h"
#include "spi.h"
#include "usart.h"

int number_goal = 0;

int in_game = 0;


void init_exti(){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	// select portB 10, 11, 14 to trigger interrupt
	SYSCFG->EXTICR[2] &= 0x00ff;
	SYSCFG->EXTICR[2] |= 0x1100; //set PB 10, 11

	SYSCFG->EXTICR[3] &= 0xf00f;
	SYSCFG->EXTICR[3] |= 0x0120; //set PB 14, PC13

	//set rising edge trigger
	EXTI->RTSR |= EXTI_RTSR_TR10 | EXTI_RTSR_TR11 | EXTI_RTSR_TR14 | EXTI_RTSR_TR13;

	//unmask the interrupt
	EXTI->IMR  |= EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR14 | EXTI_IMR_MR13;

	//enable interrupt
	NVIC->ISER[0] |= 0x80;

}

void EXTI4_15_IRQHandler(){
	if( (EXTI->PR & EXTI_PR_PR10) == EXTI_PR_PR10){
		EXTI->PR |= (EXTI_PR_PR10);
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
//    	spi2_display2("Another challenger?");
    	spi2_display1("Score: 0");// display
    	usart_send(USART3, 'S');
    	number_goal = 0;			//reset goal
//		printf("Rest game\n");
	}
	else if( (EXTI->PR & EXTI_PR_PR11) == EXTI_PR_PR11){
		EXTI->PR |= EXTI_PR_PR11;
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	spi2_display1("Welcome");
    	spi2_display2("Challenager");// display
    	in_game = 1;
    	usart_send(USART3, 'T');
//		printf("Start\n");
	}
	else if( (EXTI->PR & EXTI_PR_PR14) == EXTI_PR_PR14){
		EXTI->PR |= EXTI_PR_PR14;
    	spi_cmd(0x01);        // clear the screen
    	nano_wait(2000000);   // wait for the screen to be cleaned
    	in_game = 0;
    	spi2_display2("Nice play!");
    	spi2_display1("End");// display
//		printf("Stop\n");
	}
	else if( (EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13){
		char str[12];
		EXTI->PR |= EXTI_PR_PR13;

		EXTI->IMR &= ~(EXTI_IMR_MR13);

		number_goal = number_goal + 1;
		sprintf(str, "%d", number_goal);
		spi_cmd(0x01);        // clear the screen
		nano_wait(2000000);   // wait for the screen to be cleaned
		spi2_display1("GOAL!!!");// display goal! to screen
//		spi2_display2("I didn't play serisouly");
//		printf("GOAL!!!!!!!!!!!!!!!!!!!!!!!!\n");
		for (int i = 0; i < 500; i++){
			nano_wait(2000000);
		}

		spi_cmd(0x01);
		nano_wait(2000000);
		spi2_display1("Score: ");   //the next goal can only be detected after 4 secs. (dumb way to debounce)
		spi2_display2(str);

		EXTI->IMR |= EXTI_IMR_MR13;

	}
}
