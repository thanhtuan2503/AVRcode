#include "servo.h"
#include "bitOp.h"

void servoInit() {
	SET(DDRB, 6);
	CLR(PORTB, 6);
	// Make SERVO_PWM output
	TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1); 
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
	// Mode 15 - Fast PWM, non-inverting mode, scale f/64
	CLR(TIMSK1, 2);
	CLR(TIMSK1, 1);
	// Disable interupt at pin OCIE1A and OCIE1B
	OCR1A = 0;
	OCR1B = 0;	
	CLR(PRR0, 3);
	//Turn on power
	return;
}
void generatePulse(const int& _highTimeUS, const int& _lowTimeUS, const bool& _stop_until_reach) {
	TCNT1 = 0;
	OCR1B = (uint16_t)(0.26f * _highTimeUS);
	OCR1A = OCR1B + (uint16_t)(0.26f * _lowTimeUS);
	SET(TIMSK1, 2);
	SET(TIMSK1, 1);
	if(_stop_until_reach) {
		while (TCNT1 != OCR1A);
		return;
	}
	return;
}
void generatePulse(const int& _highTimeUS, const int& _lowTimeUS) {
	TCNT1 = 0;
	OCR1B = (uint16_t)(0.26f * _highTimeUS);
	OCR1A = OCR1B + (uint16_t)(0.26f * _lowTimeUS);
	SET(TIMSK1, 2);
	SET(TIMSK1, 1);
	
	return;
}

void stopPulse() {
	OCR1A = 0;
	OCR1B = 0;
	CLR(TIMSK1, 2);
	CLR(TIMSK1, 1);
	return;
}