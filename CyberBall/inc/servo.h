#ifndef SERVO_H
#define SERVO_H
#include "fifo.h"
#include "tty.h"
#include "stm32f0xx.h"
#include <stdio.h>

void Servo_control(int channel, float duty_cycle);

#endif
