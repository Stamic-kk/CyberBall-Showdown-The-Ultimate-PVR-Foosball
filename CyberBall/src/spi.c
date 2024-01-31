
#include "spi.h"
#include "tim2.h"

void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

//char * bit12_int_to_str(uint16_t val)
//{
//	char *str;
//
//	str[0] = (val / 1000) + 48;
//	uint16_t val1 = val % 1000;
//	str[1] = (val1 / 100) + 48;
//	uint16_t val2 = val1 % 100;
//	str[2] = (val2 / 10) + 48;
//	str[3] = (val2 % 10) + 48;
//	str[4] = '\0';
//	return str;
//}
//
//
//char ** display_contents()
//{
//	printf("----------------------------------Value------------------: %d", values[0]);
//	char *val1 = bit12_int_to_str(values[0]);
//	char *val2 = bit12_int_to_str(values[1]);
//	char *val3 = bit12_int_to_str(values[2]);
//	char *val4 = bit12_int_to_str(values[3]);
//	char **vals;
//
//	vals[0] = val1;
//	vals[1] = val2;
//	vals[2] = val3;
//	vals[3] = val4;
//	vals[4] = '\0';
//
//	return vals; // add
//}
//
//
//uint16_t str_display =
//{0x002,
// 0x200+'S', 0x200+'1',
// 0x0c0,
// 0x200+'S', 0x200+'3',
//};
////{
////        0x002, // Command to set the cursor at the first position line 1
////        0x200+'S', 0x200+'1', 0x200+':', 0x200+' ', 0x200+vals[0][0], + 0x200+vals[0][1], 0x200+vals[0][2], 0x200+vals[0][3],
////        0x200+' ', 0x200+'S', 0x200+'2', 0x200+':', + 0x200+' ', 0x200+vals[1][0], 0x200+vals[1][1], 0x200+vals[1][2],  0x200+vals[1][3],
////        0x0c0, // Command to set the cursor at the first position line 2
////        0x200+'S', 0x200+'3', 0x200+':', 0x200+' ', 0x200+vals[2][0], + 0x200+vals[2][1], 0x200+vals[2][2], 0x200+vals[2][3],
////        0x200+' ', 0x200+'S', 0x200+'4', 0x200+':', + 0x200+' ', 0x200+vals[3][0], 0x200+vals[3][1], 0x200+vals[3][2],  0x200+vals[3][3],
////};
//
//
//void spi1_setup_dma(void)
//{
//    RCC -> AHBENR |= 0b1; //enable dma1
//    DMA1_Channel3 -> CCR &= ~0x1; //turn off enable bit before set up
//    DMA1_Channel3 -> CPAR = &(SPI1 -> DR); //Set CPAR to the address of the SPI1_DR register.
//    DMA1_Channel3 -> CMAR = str_display; //Set CMAR to the display array base address
//    DMA1_Channel3 -> CNDTR = 36; //Set CNDTR to 34(number of elements)
//    DMA1_Channel3 -> CCR |= 0x10; //Set the DIRection for copying from-memory-to-peripheral.
//    DMA1_Channel3 -> CCR |= 0x80; //Set the MINC to increment the CMAR for every transfer.
//    DMA1_Channel3 -> CCR |= 0x500; //Set the memory datum size to 16-bit.
//    DMA1_Channel3 -> CCR &= ~0xa00; //Set the peripheral datum size to 16-bit.
//    DMA1_Channel3 -> CCR |= 0x20; //Set the channel for CIRCular operation.
//}
//
//
//void spi1_enable_dma(void) {
//    DMA1_Channel3 -> CCR |= 0x1;
//}
//
//void init_spi1() {
//    // PA5  SPI1_SCK
//    // PA6  SPI1_MISO
//    // PA7  SPI1_MOSI
//    // PA15 SPI1_NSS
//
//    //Set PA5, 6, 7, and 15 to alternate function 0
//	RCC -> AHBENR |= RCC_AHBENR_GPIOAEN;
//	GPIOA -> MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7 | GPIO_MODER_MODER15);
//	GPIOA -> MODER |= (GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER15_1);
//	GPIOA -> AFR[0] &= ~0xfff00000;
//	GPIOA -> AFR[1] &= ~0xf0000000;
//
//	RCC -> APB2ENR |= 0x1000;
//    SPI1 -> CR1 &= ~0x40; //Disable SPI1
//    SPI1 -> CR1 |= 0x38; //Set baud control rate divisor to 256
//    SPI1 -> CR2 = 0x900; //Configure the interface for a 10-bit word size
//    SPI1 -> CR1 |= 0x4; //Configure the SPI channel to be in â€œmaster modeâ€�
//    SPI1 -> CR2 |= 0x4; //Set the SS Output enable bit
//    SPI1 -> CR2 |= 0x8; //enable NSSP
//    SPI1 -> CR2 |= 0x2; //enable DMA transfers on transmit buffer empty
//    SPI1 -> CR1 |= 0x40; //Enable SPI1
//}
//
//
//
//void spi_cmd(unsigned int data) {
//    while(!(SPI1->SR & SPI_SR_TXE)) {}
//    SPI1->DR = data;
//}
//
//void spi_data(unsigned int data) {
//    spi_cmd(data | 0x200);
//}
//
//void spi1_init_oled() {
//    nano_wait(1000000);
//    spi_cmd(0x38);
//    spi_cmd(0x08);
//    spi_cmd(0x01);
//    nano_wait(2000000);
//    spi_cmd(0x06);
//    spi_cmd(0x02);
//    spi_cmd(0x0c);
//}
//
//void spi1_display1(const char *string) {
//    spi_cmd(0x02);
//    while(*string != '\0') {
//        spi_data(*string);
//        string++;
//    }
//}
//void spi1_display2(const char *string) {
//    spi_cmd(0xc0);
//    while(*string != '\0') {
//        spi_data(*string);
//        string++;
//    }
//}
//
//
//void spi_display(){
//	//vals = display_contents();
//	init_spi1();
//	spi1_init_oled();
//    spi1_setup_dma();
//    spi1_enable_dma();
//}

void init_spi1() {
    // PA5  SPI1_SCK
    // PA6  SPI1_MISO
    // PA7  SPI1_MOSI
    // PA15 SPI1_NSS
    RCC->APB2ENR|= RCC_APB2ENR_SPI1EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &=~(0x3f<<10);
    GPIOA->MODER |= (0x2A<<10);
    GPIOA->MODER &=~(0x3<<30);
    GPIOA->MODER |=(0x2<<30);
    GPIOA->AFR[0]&=~(0xfff<<(5*4));
    GPIOA->AFR[1]&=~(0xf<<((15-8)*4));
    SPI1->CR1 &= ~(SPI_CR1_SPE);
    SPI1->CR1 |= SPI_CR1_MSTR|SPI_CR1_BR;
    SPI1->CR2 |= SPI_CR2_DS_0|SPI_CR2_DS_3|SPI_CR2_SSOE|SPI_CR2_TXDMAEN|SPI_CR2_NSSP;
    SPI1->CR2 &= ~(SPI_CR2_DS_1|SPI_CR2_DS_2);
    SPI1->CR1 |= SPI_CR1_SPE;
}


void spi_cmd(unsigned int data) {
    while(!(SPI1->SR & SPI_SR_TXE)) {}
    SPI1->DR = data;
}
void spi_data(unsigned int data) {
    spi_cmd(data | 0x200);
}
void spi1_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}
void spi1_display1(const char *string) {
    spi_cmd(0x02);
    while(*string != '\0') {
        spi_data(*string);
        string++;
    }
}


