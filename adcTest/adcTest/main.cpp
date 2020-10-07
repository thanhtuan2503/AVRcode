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

void setupADC(){
	ADMUX = (1 << ADLAR);
	DIDR0 = 0b1111'1111;
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1);
	
	TCNT4 = 0;
	OCR4B = 0x7A12;
	TCCR4B = (1 << WGM42) | (1 << CS42) | (1 << CS40);
	TIMSK4 = (1 << OCIE4A);
}

uint8_t readAnalog(uint8_t channel) {
	ADMUX &= 0b1111'1000;
	ADMUX |= channel;
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return (ADC);
}

ISR(TIMER4_COMPA_vect) {

}

int main(void)
{
    /* Replace with your application code */
	setupADC();
    while (1) 
    {
    }
}

