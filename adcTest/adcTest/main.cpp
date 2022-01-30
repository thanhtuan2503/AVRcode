/*
 * adcTest.cpp
 *
 * Created: 10/6/2020 9:39:59 PM
 * Author : thanh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bitOp.h"
#include "myDelay.h"

int arr[8];
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
	OCR4A = 0x7A12;
	TCCR4B |= (1 << WGM42) | (1 << CS42) | (1 << CS40);
	TIMSK4 = (1 << OCIE4A);
	CLR(PRR1, 4);
}

int main(void)
{
    /* Replace with your application code */
	//SET(DDRB, 7);
	//CLR(PORTB, 7);
	initTimer4();
	setupADC();
    while (1) 
    {
		if (TCNT4 == OCR4A) {
			//INV(PORTB, 7);
		}
    }
}

