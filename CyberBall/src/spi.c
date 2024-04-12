
#include "spi.h"
#include "tim2.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

//void init_spi1_for_stm32_stm32() {
//    // PA5  SPI1_SCK
//    // PA6  SPI1_MISO
//    // PA7  SPI1_MOSI
//    // PA15 SPI1_NSS
//
////    RCC->APB2ENR|= RCC_APB2ENR_SPI1EN;
////    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
////    GPIOA->MODER &=~(0x3f<<10);
////    GPIOA->MODER |= (0x2A<<10);
////    GPIOA->MODER &=~(0x3<<30);
////    GPIOA->MODER |=(0x2<<30);
////    GPIOA->AFR[0]&=~(0xfff<<(5*4));
////    GPIOA->AFR[1]&=~(0xf<<((15-8)*4));
////    SPI1->CR1 &= ~(SPI_CR1_SPE);
////    SPI1->CR1 |= SPI_CR1_MSTR|SPI_CR1_BR;
////    SPI1->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_3|SPI_CR2_SSOE|SPI_CR2_TXDMAEN|SPI_CR2_NSSP;
////    SPI1->CR2 &= ~(SPI_CR2_DS_1|SPI_CR2_DS_2);
////    SPI1->CR1 |= SPI_CR1_SPE;
//
//	// set up for com between STM32 and STM32
//    RCC->APB2ENR|= RCC_APB2ENR_SPI1EN;
//    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//    GPIOA->MODER &=~(0x3f<<10);
//    GPIOA->MODER |= (0x2A<<10);
//    GPIOA->MODER &=~(0x3<<30);
//    GPIOA->MODER |=(0x2<<30);
//    GPIOA->AFR[0]&=~(0xfff<<(5*4));
//    GPIOA->AFR[1]&=~(0xf<<((15-8)*4));
//    SPI1->CR1 &= ~(SPI_CR1_SPE);
//    SPI1->CR1 |= SPI_CR1_MSTR;
//    //SPI1->CR1 |= SPI_CR1_LSBFIRST;
//    SPI1->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3|SPI_CR2_SSOE|SPI_CR2_TXDMAEN|SPI_CR2_NSSP;
//    //SPI1->CR2 &= ~(SPI_CR2_DS_3);
//    SPI1->CR1 |= SPI_CR1_SPE;
//}

void init_spi2_for_LCD(){
    RCC->APB1ENR|= RCC_APB1ENR_SPI2EN;
    SPI2->CR1 &= ~(SPI_CR1_SPE);
    SPI2->CR1 |= SPI_CR1_BR|SPI_CR1_MSTR;
    SPI2->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_3|SPI_CR2_SSOE|SPI_CR2_TXDMAEN|SPI_CR2_NSSP;
    SPI2->CR2 &= ~(SPI_CR2_DS_1|SPI_CR2_DS_2);
    SPI2->CR1 |= SPI_CR1_SPE;
}

void spi_cmd(unsigned int data) {
    while(!(SPI2->SR & SPI_SR_TXE)) {}
    SPI2->DR = data;
}
void spi_data(unsigned int data) {
    spi_cmd(data | 0x200);
}
void spi2_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}
void spi2_display1(const char *string) {
    spi_cmd(0x02);
    while(*string != '\0') {
        spi_data(*string);
        string++;
    }
}

void spi2_display2(const char *string) {
    spi_cmd(0xc0);
    while(*string != '\0') {
        spi_data(*string);
        string++;
    }
}


