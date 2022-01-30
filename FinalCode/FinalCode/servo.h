#include <avr/interrupt.h>

#pragma once
#ifndef _SERVO_H_
#define _SERVO_H_

#ifndef SERVO_PWM
#define SERVO_PWM	PB6
#endif

void servoInit();
void generatePulse(const int& _highTimeMS, const int& _lowTimeMS);
void generatePulse(const int& _highTimeUS, const int& _lowTimeUS, const bool& _stop_until_reach);
void stopPulse();

#endif //_SERVO_H_