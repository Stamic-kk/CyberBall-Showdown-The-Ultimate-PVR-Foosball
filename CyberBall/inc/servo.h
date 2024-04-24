#ifndef SERVO_H
#define SERVO_H
#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>

#define MAX_Y 7.1
#define MIN_Y 2

extern int swing_control;


void Servo_control(int channel, float duty_cycle);
float pixelToDutyCycle(int pixelPos);
#endif
