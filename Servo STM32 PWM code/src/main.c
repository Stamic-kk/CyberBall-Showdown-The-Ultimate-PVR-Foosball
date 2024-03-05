/**
  ******************************************************************************
  * @file    main.c
  * @author  Weili An
  * @version V1.0
  * @date    Nov 4, 2022
  * @brief   ECE 362 Lab 9 Student template
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include <math.h>   // for M_PI

extern int servo_swing = 0;  //used for TIM7 ISR handler, control servo swinging
void Servo_control(int channel, float duty_cycle);
void nano_wait(int);

// 16-bits per digit.
// The most significant 8 bits are the digit number.
// The least significant 8 bits are the segments to illuminate.
uint16_t msg[8] = { 0x0000,0x0100,0x0200,0x0300,0x0400,0x0500,0x0600,0x0700 };
extern const char font[];
// Print an 8-character string on the 8 digits
void print(const char str[]);
// Print a floating-point value.
void printfloat(float f);


//============================================================================
// Lab 9 Functions
//============================================================================
void setup_tim3(void) {
    // TODO: Enable GPIO C
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // TODO: Configure the PC6-9 to be the outputs of TIM3 Ch 1-4
    // TODO: First we clear their MODER bits
    GPIOC->MODER &= ~0xFF000;
    // TODO: Then we set them to AF mode
    GPIOC->MODER |= 0xAA000;
    // TODO: Set PC6-9 to use AF0 since this corresponds to the TIM3 Ch1-4
    // AFR[0] -> AFRL
    // AFR[1] -> AFRH
    GPIOC->AFR[0] &= ~0xFF000000;
    GPIOC->AFR[1] &= ~0xFF;

    // TODO: Enable TIM3 with 1 Hz timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN ;
    TIM3->PSC = 48000-1;
    TIM3->ARR = 1000-1;

    // TODO: Set to PWM mode 1 for all channels
    // Can use the following code to set a channel to PWM mode 1 (110)
    // This line set Timer x's channel 1 to be PWM mode 1 (OC1M bits with 110)
     TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;   //channel 1
     TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;   //channel 2
     TIM3->CCMR2 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;   //channel 3
     TIM3->CCMR2 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1;   //channel

    // TODO: Enable all 4 channel outputs in `TIM3_CCER` using `CC1E` bit
     TIM3->CCER |= TIM_CCER_CC1E |TIM_CCER_CC2E  |TIM_CCER_CC3E |TIM_CCER_CC4E;
    // TODO: Enable TIM3 counter
     TIM3->CR1 |= TIM_CR1_CEN;
    // TODO: Set CCR valueS
     TIM3->CCR1 = 800;
     TIM3->CCR2 = 400;
     TIM3->CCR3 = 200;
     TIM3->CCR4 = 100;

}


int getrgb(void);

// Helper function for you
// Accept a byte in BCD format and convert it to decimal
uint8_t bcd2dec(uint8_t bcd) {
    // Lower digit
    uint8_t dec = bcd & 0xF;

    // Higher digit
    dec += 10 * (bcd >> 4);
    return dec;
}

void setrgb(int rgb) {
//    uint8_t b = bcd2dec(rgb & 0xFF);
//    uint8_t g = bcd2dec((rgb >> 8) & 0xFF);
//    uint8_t r = bcd2dec((rgb >> 16) & 0xFF);

    //testing
    uint8_t b = 1;
    uint8_t g = 20;
    uint8_t r = 75;

    // TODO: Assign values to TIM1->CCRx registers
    // Remember these are all percentage
    // Also LEDs are on when the corresponding PWM output is low
    // so you might want to invert some numbers
    TIM1->CCR1 = (100 - r)*24 ;
    TIM1->CCR2 = (100 - g)*24;  // 100-b is the duty cycle cause  CCR/ARR*100% = duty cycle%
    TIM1->CCR3 = (100 - b)*24;


}

//============================================================================
// Lab 9 Functions end, rest are just copying your previous lab code
//============================================================================


//============================================================================
// enable_ports()
//============================================================================
void enable_ports(void) {

//    Enables the RCC clock to GPIOB and GPIOC without affecting any other RCC clock settings for other peripherals
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
//    Configures pins PB0 – PB10 to be outputs
    GPIOB->MODER &= ~0x3FFFFF;
    GPIOB->MODER |= 0x155555;
//    Configures pins PC4 – PC7 to be outputs
    GPIOC->MODER &= ~0xFF00;
    GPIOC->MODER |= 0x5500;
//    Configures pins PC4 – PC7 to have output type open-drain (using the OTYPER registers)
    GPIOC->OTYPER &= ~0xF0;
    GPIOC->OTYPER |= 0xF0;
//    Configures pins PC0 – PC3 to be inputs
//    Configures pins PC0 – PC3 to be internally pulled high
    GPIOC->PUPDR &= ~0xFF;
    GPIOC->PUPDR |= 0x55;

}

//============================================================================
// setup_dma()
//============================================================================
void setup_dma(void) {
//    Enables the RCC clock to the DMA controller
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
//    Turn off the enable bit for the channel5
    DMA1_Channel5->CCR &= ~DMA_CCR_EN;
//    Set CPAR to the address of the GPIOB_ODR register.
    DMA1_Channel5->CPAR = &GPIOB->ODR;
//    Set CMAR to the msg array base address
    DMA1_Channel5->CMAR = msg;
//    Set CNDTR to 8
    DMA1_Channel5->CNDTR = 8;
//    Set the DIRection for copying from-memory-to-peripheral. bit 5 = 1
//    DMA1_Channel5->CCR &= ~0x10;
    DMA1_Channel5->CCR |= DMA_CCR_DIR   ;
//    Set the MINC to increment the CMAR for every transfer.   bit 7 = 1
//    DMA1_Channel5->CCR &= ~0x80;
//    DMA1_Channel5->CCR |= 0x80;
      DMA1_Channel5->CCR |= DMA_CCR_MINC;
//    Set the memory datum size to 16-bit.  bits 11:10 = 01
    DMA1_Channel5->CCR &= ~0xC00;
    DMA1_Channel5->CCR |= DMA_CCR_MSIZE_0 ;

//    Set the peripheral datum size to 16-bit. Bits 9:8  = 01
    DMA1_Channel5->CCR &= ~0x300;
    DMA1_Channel5->CCR |= DMA_CCR_PSIZE_0;


//    Set the channel for CIRCular operation.    Bit 5 = 1
  //  DMA1_Channel5->CCR &= ~0x20;
    DMA1_Channel5->CCR |= DMA_CCR_CIRC;

}

//============================================================================
// enable_dma()
//============================================================================
void enable_dma(void) {
    //Enable the channel.    bit 0 = 1
    DMA1_Channel5->CCR &= ~0x1;
    DMA1_Channel5->CCR |= 0x1;
}
//============================================================================
// init_tim15()
//============================================================================
void init_tim15(void) {
//    enable TIM15’s clock in RCC   bit 16 = 1
    //and trigger a DMA request at a rate of 1 kHz.    48,000,000 / 2400x = 1000
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN ;
    TIM15->PSC =  20-1;
    TIM15->ARR = 2400-1;

//  enableing the update DMA request by setting the UDE bit in the DIER. Do not set the UIE bit in the DIER this time.
    TIM15->DIER |= TIM_DIER_UDE ;
//    There is no ISR to invoke for this timer this time.
//            The timer will trigger the DMA channel directly. The DMA operation will do the copying that you had to do in the ISR in lab 5.
    TIM15->CR1 |= TIM_CR1_CEN;
}


uint8_t col; // the column being scanned

void drive_column(int);   // energize one of the column outputs
int  read_rows();         // read the four row inputs
void update_history(int col, int rows); // record the buttons of the driven column
char get_key_event(void); // wait for a button event (press or release)
char get_keypress(void);  // wait for only a button press event.
float getfloat(void);     // read a floating-point number from keypad
void show_keys(void);     // demonstrate get_key_event()

//============================================================================
// The Timer 7 ISR
//============================================================================

// TODO To be copied
void TIM7_IRQHandler()
{
    // Remember to acknowledge the interrupt here!
    TIM7->SR &= ~TIM_SR_UIF;
    if (!servo_swing) {
        Servo_control(3, 12);  //Set channel 3 PWM to 12%
        servo_swing = 1;
    }
    else{
        Servo_control(3, 2);
        servo_swing = 0;
    }
}


//============================================================================
// init_tim7()
//============================================================================
void init_tim7(void) {
   // causes the Timer 7 ISR to be invoked at a rate of 2 Hz.
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    TIM7->PSC =  10000-1;
    TIM7->ARR = 2400-1;
    TIM7->DIER |= TIM_DIER_UIE ;
    TIM7->CR1 |= TIM_CR1_CEN;
    //    Enable the Timer 7 interrupt in the NVIC ISER.
    NVIC->ISER[0] |= 1 << TIM7_IRQn;
}

//=============================================================================
// Part 3: Analog-to-digital conversion for a volume level.
//=============================================================================

//uint32_t volume = 2400;
//4.3
int volume = 2400;

//============================================================================
// setup_adc()
//============================================================================
void setup_adc(void) {
//    Enable the clock to GPIO Port A
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//    Set the configuration for analog operation only for the appropriate pin.    PA1 to analog 11
    GPIOA->MODER &= ~0xC;
    GPIOA->MODER |=  0xC;
//    Enable the clock to the ADC peripheral
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
//    Turn on the “high-speed internal” 14 MHz clock (HSI14)
    RCC->CR2 |= RCC_CR2_HSI14ON  ;
//    Wait for the 14 MHz clock to be ready
    while(!(RCC->CR2 & RCC_CR2_HSI14RDY));
    //    Enable the ADC by setting the ADEN bit in the CR register
     ADC1->CR |= ADC_CR_ADEN;

    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    //    Wait for the ADC to be ready
        //    Select the corresponding channel for ADC_IN1 in the CHSELR
        ADC1->CHSELR = 0;
        ADC1->CHSELR |= 1<<1;

//    Wait for the ADC to be ready
    while(!(ADC1->ISR & ADC_ISR_ADRDY));

}
//============================================================================
// Varables for boxcar averaging.
//============================================================================
#define BCSIZE 32
int bcsum = 0;
int boxcar[BCSIZE];
int bcn = 0;

//============================================================================
// Timer 2 ISR
//============================================================================
// TODO To be copied

void TIM2_IRQHandler()
{
//    Acknowledge the interrupt.
    TIM2->SR &= ~TIM_SR_UIF;
//    Start the ADC by turning on the ADSTART bit in the CR.
    ADC1->CR = ADC_CR_ADSTART;
//    Wait until the EOC bit is set in the ISR.
    while(!(ADC1->ISR & ADC_ISR_EOC ));
    bcsum -= boxcar[bcn];
    bcsum += boxcar[bcn] = ADC1->DR;
    bcn += 1;
    if (bcn >= BCSIZE)
        bcn = 0;
    volume = bcsum / BCSIZE;

}
//============================================================================
// init_tim2()
//============================================================================
void init_tim2(void) {
    //sets up Timer 2 to invoke the ISR exactly ten times per second (10 Hz).
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC =  2000-1;
    TIM2->ARR = 2400-1;
    TIM2->DIER |= TIM_DIER_UIE ;
    TIM2->CR1 |= TIM_CR1_CEN;
    //    Enable the Timer 2 interrupt in the NVIC ISER.
    NVIC->ISER[0] |= 1 << TIM2_IRQn;
}


//===========================================================================
// Part 4: Create an analog sine wave of a specified frequency
//===========================================================================
void dialer(void);

// Parameters for the wavetable size and expected synthesis rate.
#define N 1000
#define RATE 20000
short int wavetable[N];
int step0 = 0;
int offset0 = 0;
int step1 = 0;
int offset1 = 0;

//===========================================================================
// init_wavetable()
// Write the pattern for a complete cycle of a sine wave into the
// wavetable[] array.
//===========================================================================
void init_wavetable(void) {
    for(int i=0; i < N; i++)
        wavetable[i] = 32767 * sin(2 * M_PI * i / N);
}

//============================================================================
// set_freq()
//============================================================================
void set_freq(int chan, float f) {
    if (chan == 0) {
        if (f == 0.0) {
            step0 = 0;
            offset0 = 0;
        } else
            step0 = (f * N / RATE) * (1<<16);
    }
    if (chan == 1) {
        if (f == 0.0) {
            step1 = 0;
            offset1 = 0;
        } else
            step1 = (f * N / RATE) * (1<<16);
    }
}

//============================================================================
// setup_dac()
//============================================================================
void setup_dac(void) {

//    Enable the clock to GPIO Port A (reenabling is safe)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//    Set the configuration for analog operation only for the appropriate pin .  PA4, 11 for analog
    GPIOA->MODER &= ~0x300;
    GPIOA->MODER |=  0x300;
//    Enable the RCC clock for the DAC
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
//    Select a TIM6 TRGO trigger for the DAC with the TSEL field of the CR register  , select channel 1
    DAC->CR  &= ~DAC_CR_TSEL1 ;
//    Enable the trigger for the DAC
    DAC->CR |= DAC_CR_TEN1;
//    Enable the DAC
    DAC->CR |=DAC_CR_EN1  ;

}
//============================================================================
// Timer 6 ISR
//============================================================================
// TODO To be copied
void TIM6_DAC_IRQHandler()
{
    // Acknowledge the interrupt right here!
    TIM6->SR &= ~TIM_SR_UIF;
    offset0 += step0;
    offset1 += step1;
    if (offset0 >= (N << 16))
        offset0 -= (N << 16);
    if (offset1 >= (N << 16))
        offset1 -= (N << 16);
    int samp = wavetable[offset0>>16] + wavetable[offset1>>16];
    samp = samp * volume;
    samp = samp >> 18;
    samp += 1200;
   // DAC->DHR12R1 = samp;


    //4.3
//    samp = (((samp * volume)& 0xfff)>>18) + 1200;
    TIM1->CCR4 = samp;


}


//============================================================================
// init_tim6()
//============================================================================
void init_tim6(void) {
//initializes timer 6 so that it’s interrupt is called at RATE times per second (20000 for this exercise).
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC =  1000-1;
    TIM6->ARR = (48000/RATE)-1;
    TIM6->DIER |= TIM_DIER_UIE ;
    TIM6->CR1 |= TIM_CR1_CEN;
 //   TIM6->CR2 |= TIM_CR2_MMS_1  ;
    //    Enable the Timer 2 interrupt in the NVIC ISER.
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;

}

void setup_tim1(void)
{

//    Activate the RCC clock to GPIO Port A.
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
//    Configure the MODER for the four pins to set them for alternate function use.  (PA8,9,10,11 for  TIM1_CH1, TIM1_CH2, TIM1_CH3, and TIM1_CH4.
    GPIOA->MODER &= ~0xFF0000;
    // TODO: Then we set them to AF mode
    GPIOA->MODER |= 0xAA0000;
//    Set the alternate function register to route the timer function to the external pins.
    GPIOA->AFR[1] &= ~0xFFFF;    //PA8,9,10,11    clear first
    //to Set to route the timer function to the external pins. set to to AF2   0010
    GPIOA->AFR[1] |= 0x2222;

//    Activate the RCC clock to Timer 1.
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

//    You will be using Timer 1 for PWM output rather than interrupt generation.
//            In order for output to work at all you must enable the MOE bit of the break and dead-time register (BDTR).
    TIM1->BDTR |= TIM_BDTR_MOE;
//    Set the prescaler to divide by 1, and set the ARR so that an update event occurs 20000 times per second.
//    TIM1->PSC = 0;  //original
    TIM1->PSC = 40-1;   // together output 50hz
    TIM1->ARR = 24000-1;   // 48M/20000 = 2400,

//    Configure the output channels of Timer 1:
//         Configure the “capture/compare mode registers”, CCMR1 and CCMR2, to set channels 1, 2, 3, and 4 for PWM mode 1.
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 ;   //110 for PWM model 1
    TIM1->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 ;
    TIM1->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 ;   //110 for PWM model 1
    TIM1->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 ;
//         Configure the CCMR2 register to set the “output compare preload enable” bit only for channel 4 (OC4PE bit).
    TIM1->CCMR2 |=  TIM_CCMR2_OC4PE;
//         Enable the (uninverted) channel outputs for all four channels by turning on the CC1E, CC2E, etc.
//         bits for in the “capture/compare enable register”, CCER. Until you do this for each channel, the timer will not affect the outputs.
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E  | TIM_CCER_CC3E | TIM_CCER_CC4E ;
//         Enable the timer.
    TIM1->CR1 |= TIM_CR1_CEN ;


}


void Servo_control(int channel, float duty_cycle)
{
    switch (channel)
    {
    case 1: TIM1->CCR1 = duty_cycle *240;       //TIM1 CH1: PA8
            break;
    case 2: TIM1->CCR2 = duty_cycle *240;       //TIM1 CH2: PA9
            break;
    case 3: TIM1->CCR3 = duty_cycle *240;       //TIM1 CH3: PA10
            break;
    case 4: TIM1->CCR4 = duty_cycle *240;       //TIM1 CH4: PA11
            break;
    }
}
//============================================================================
//              Main
//============================================================================
int main(void) {

    enable_ports();
    setup_tim1();
    init_tim7();

    //Note: using TIM7 to generate interrupts and control swinging servo

    //Lateral Servo Control for Channel 2
     Servo_control(2, 12);
     nano_wait(2000000000);

     Servo_control(2, 2);
     nano_wait(2000000000);

     Servo_control(2, 12);
     nano_wait(2000000000);

     Servo_control(2, 5);
     nano_wait(2000000000);




//    //Lateral Servo Control
//     uint8_t duty_cycle3 = 12;
//     TIM1->CCR3 = duty_cycle3 *240;  // 100-b is the duty cycle cause  CCR/ARR*100% = duty cycle%
//     nano_wait(2000000000);
//     duty_cycle3 = 2;
//     TIM1->CCR3 = duty_cycle3 *240;
//     nano_wait(2000000000);
//
//     duty_cycle3 = 7;
//     TIM1->CCR3 = duty_cycle3 *240;
//     nano_wait(2000000000);


//    for(;;) {
//
//          //Swinging Servo Control

//           uint8_t duty_cycle3 = 12;
//           TIM1->CCR3 = duty_cycle3 *240;  // 100-b is the duty cycle cause  CCR/ARR*100% = duty cycle%
//           nano_wait(450000000);

//           duty_cycle3 = 2;
//           TIM1->CCR3 = duty_cycle3 *240;
//           nano_wait(450000000);
////           nano_wait(450000000);
//
//    }

}
