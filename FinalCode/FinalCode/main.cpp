/*
 * FinalCode.cpp
 *
 * Created: 10/8/2020 8:42:02 AM
 * Author : thanh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <math.h>
#include "bitOp.h"
#include "linecal.h"
#include "func.h"
#include "definepin.h"
#include "servo.h"
#include "utility.h"

#define CW 0
#define CCW 1
#define STOP 2


#define DEFAULT_SPEED 500
/*
	0 degree: OCR1A = 1060
	180 degree: OCR1A = 4840
*/
#define STD_POSITION 2950	
#define STD_ICR1 40000

#define numOfSensor 8

uint8_t servoSpeed = 0;
uint8_t motorSpeed = 0;
int arr[8];
float val[numOfSensor][numOfSensor];
uint16_t halfLine = 0;
uint16_t fullLine = 0;
uint16_t intersectSignal = 0;
uint16_t isTurning = 0;
float angleOut = 0;
double xCoordinate = 0;
uint16_t pin[8] = {A_IN1, A_IN2, A_IN3, A_IN4, A_IN5, A_IN6, A_IN7, A_IN8};
int maxArr[8];
int minArr[8];
double ledVal[8];


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

void setupADC(){
	ADMUX |= (1 << REFS0);
	DIDR0 = 0b11111111;
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
}

uint16_t readAnalog(uint8_t channel) {
	ADMUX &= 0b11111000;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return (ADC);
}

void initTimer4() {
	TCNT4 = 0;
	OCR4A = 3200;
	TCCR4B |= (1 << WGM42) | (1 << CS42) | (1 << CS40);
	TIMSK4 = (1 << OCIE4A);
	CLR(PRR1, 4);
}

void readMinMax(){
	//   for(uint16_t i = 0; i < N; i++){
	//    maxArr[i] = MAXVALUE;
	//    minArr[i] = MINVALUE;
	//  }
	for(uint16_t i = 0; i < N; i++){
		maxArr[i] = MINVALUE;
		minArr[i] = MAXVALUE;
	}
	int preSensor[8];
	for (uint16_t i = 30; i < 150; i = i + 10){
		generatePulse(hashAngle(i), hashSpeed(100), true);
		for(uint16_t j = 0; j < N ; j++){
			preSensor[j] = readAnalog(j);
			if (preSensor[j] > maxArr[j]) maxArr[j] = preSensor[j];
			if (preSensor[j] < minArr[j]) minArr[j] = preSensor[j];
		}
	}
	for (uint16_t i = 150; i > 30; i = i - 10){
		generatePulse(hashAngle(i), hashSpeed(1), true);
		for(uint16_t j = 0; j < N ; j++){
			preSensor[j] = readAnalog(j);
			if (preSensor[j] > maxArr[j]) maxArr[j] = preSensor[j];
			if (preSensor[j] < minArr[j]) minArr[j] = preSensor[j];
		}
	}
}

void update(){
	switch (lineType(ledVal)) {
		case 0: //Line Middle
		if (isTurning) {
			//"Turning Completed. Moving Ahead."
			isTurning = 0;
			fullLine = 0;
			halfLine = 0;
			intersectSignal = 0;
		}
		if (fullLine == 2) {
			//"Line Detected. Moving Ahead."
			fullLine = 0;
			halfLine = 0;
		}
		xCoordinate = getLinePos(ledVal);
		break;
		case 1: //Line Full White
		if (fullLine == 0 && halfLine != 1) {
			//"Challenge is about to appear. Moving ahead"
			fullLine = 1;
			//End game not solving.
		}
		if (halfLine == 1) {
			//"Turn left at the T-Intersection."
			xCoordinate = 0;
			isTurning = 1;
		}
		else if (halfLine == 2) {
			//"Turn right at the T-Intersection."
			xCoordinate = 7;
			isTurning = 1;
		}
		break;
		case 2: //Line Full Black
		if (isTurning) {
			//"Turning slowly. Avoid Crashing."
		}
		else if (halfLine == 1) {
			if (fullLine == 0 || fullLine == 2) {
				//"Line is disappearing for a while. Moving Left for Searching Line"
			}
		}
		else if (halfLine == 2) {
			if (fullLine == 0 || fullLine == 2) {
				//"Line is disappearing for a while. Moving Right for Searching Line"
			}
			fullLine = 2;
		}
		else {
			//"Line is disappearing for a while. Keep moving ahead."
			//Handle obstacle.
		}
		
		fullLine = 2;
		break;
		case 3: //Half-Line Left
		if (fullLine == 1) {
			//"Turn left 90 degree."
			xCoordinate = 0;
			isTurning = 1;
		}
		else {
			//"Half-Line Left Signal For Searching Line in Black or Intersection."
			halfLine = 1;
		}
		break;
		case 4: //Half-Line Right
		if (fullLine == 1) {
			// "Turn right 90 degree."
			xCoordinate = 7;
			isTurning = 1;
		}
		else {
			//"Half-Line Right Signal For Searching Line in Black." << endl;
			halfLine = 2;
		}
		break;
		case 5:
		xCoordinate = getLinePos(ledVal);
		break;
	}
	
}

int main(void)
{
    /* Replace with your application code */
	initMotorControl();
	servoInit();
	initTimer4();
	setupADC();
	generatePulse(hashAngle(90), hashSpeed(100), true);
	_delay_ms(2000);
	//readMinMax();
	//while (1) {
	//
		//
	//}
	while (1) 
    {
		
		if (TCNT4 == OCR4A) {
			for (int i = 0;  i < N; i++){
				ledVal[i] = (double)readAnalog(i);
			}
			//normalize(ledVal, maxArr, minArr, MAXVALUE, MINVALUE);
			update();
			angleOut = atan2(absolute(3.5 - xCoordinate), 20.5)/(3.14)*180;
			
			servoSpeed = 100;
			if (xCoordinate > 3.5) generatePulse(hashAngle(90-angleOut), hashSpeed(servoSpeed),false);
				else generatePulse(hashAngle(90+angleOut), hashSpeed(servoSpeed), false);
		}
	}
}
