/*
 * Millis.cpp
 *
 * Created: 4/14/2020 4:30:41 PM
 * Author : thanh
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include "bitOp.h"
#include "myDelay.h"
int a = 0;

void initializeIO(){
	SET(DDRD, 7);
	CLR(PORTD, 7);
}

void initializeTimer(){ 
	TCCR1B = (1 <<WGM12) | (1 << CS10) | (1 << CS12);
	OCR1A = 15624;
}

void initMillis(){
	if (a != 1)
	{ 
		TCNT1 = 0;
		TIMSK1 = 1<<OCIE1A;
		a = 1;
	}
}
 
ISR(TIMER1_COMPA_vect){
	TIMSK1 = 0;
	INV(PORTD, 7);
	a = 0;
}
int main(void)
{	
    /* Replace with your application code */
	initializeIO();
	initializeTimer();
	sei();
    while (1) 
    {
		initMillis();
    }
}

