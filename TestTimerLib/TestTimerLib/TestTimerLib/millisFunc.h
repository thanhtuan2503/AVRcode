/*
 * IncFile2.h
 *
 * Created: 4/14/2020 8:34:56 PM
 *  Author: thanh
 */ 

#ifndef INCFILE2_H_
#define INCFILE2_H_

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#define CLK 16000000
#define TOP 249

volatile unsigned long timer1_millis;

ISR(TIMER1_COMPA_vect){
	timer1_millis++;
}

void initMillis(){
	TCCR1B = (1 <<WGM12) | (1 << CS10) | (1 << CS11);
	OCR1A = 249;
	TCNT1 = 0;
	TIMSK1 = 1<<OCIE1A;
	sei();
}

unsigned long millis(){
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer1_millis;
	}
	return millis_return;
}

#endif /* INCFILE2_H_ */