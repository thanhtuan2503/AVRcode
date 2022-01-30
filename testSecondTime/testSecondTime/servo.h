#include <avr/interrupt.h>

#pragma once
#ifndef _SERVO_H_
#define _SERVO_H_

#ifndef SERVO_PWM
#define SERVO_PWM	PB6
#endif

void servoInit();
void generatePulse(const unsigned int& _highTimeMS, const unsigned int& _lowTimeMS);
void stopPulse();

#endif //_SERVO_H_