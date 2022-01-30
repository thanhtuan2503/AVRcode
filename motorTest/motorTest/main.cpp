/*
 * motorTest.cpp
 *
 * Created: 10/7/2020 11:36:31 PM
 * Author : thanh
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"
#include "bitOp.h"

#define CW 0
#define CCW 1
#define STOP 2


#define DEFAULT_SPEED 500
void initMotorControl() {
	//SET PIN4, PIN3, PIN 2 on Arduino in OUTPUT MODE
	// PIN 3 = INA = PE5; PIN 4 = INB = PG5; PIN 2 = EN = PE4
	SET(DDRG, 5);
	CLR(PORTG, 5);
	SET(DDRE, 5);
	CLR(PORTE, 5);
	SET(DDRE, 4);
	CLR(PORTE, 4);
	SET(DDRE, 3);
	
	SET(PORTE, 4);			//Enable motor driver
	
	TCNT3 = 0;
	TCCR3A = 0;
	TCCR3B = 0;			//SET PIN5 on Arduino in OUTPUT MODE
	CLR(PORTE, 3);			
	TCCR3A |= (1 << WGM31) | (1 << COM3A1);
	TCCR3B |= (1 << WGM32) | (1 << WGM33) | (1 << CS31);
	TIMSK3 = 0;
	OCR3A = DEFAULT_SPEED;
	ICR3 = 2000;
	CLR(PRR1, 3);
}
void setSpeed(const uint8_t& _speed) {
	OCR3A = _speed * 6 + 200;
}
void motorControl(const uint8_t& _motorEnable, const uint8_t& _direction, const uint8_t& _speed) {
	if(!_motorEnable) {
		switch(_direction) {
			case CW: 
				CLR(PORTE, 5);			//Write INA to LOW
				SET(PORTG, 5);			//Write INB to HIGH
				setSpeed(_speed);
				break;
			case CCW:
				SET(PORTE, 5);			//Write INA to HIGH
				CLR(PORTG, 5);			//Write INB to LOW
				setSpeed(_speed);	
				break;
			default:
				CLR(PORTE, 5);
				CLR(PORTG, 5);
		}
	}
}

int main(void)
{
    /* Replace with your application code */
	initMotorControl();
    while (1) 
    {
		motorControl(0,CW, 255);
    }
}

