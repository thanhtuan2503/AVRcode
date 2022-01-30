/*
 * servoTest.cpp
 *
 * Created: 10/6/2020 8:38:36 PM
 * Author : thanh
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "bitOp.h"
#include "myDelay.h"
unsigned int Gia_tri_moi;
int a = 0 ;
int b = 0 ;
void setupServo(){
	// TIMER1 FOR SERVO CONTROL
	TCNT1 = 0;
	TCCR1A = 0 ;TCCR1B = 0;
	SET(DDRB, 5);
	CLR(PORTB, 5);
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) ;
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10); 
	TIMSK1 = 0;
	OCR1A = 8480;
	ICR1 = 65535;
	CLR(PRR0, 3);
	// TIMER3 FOR MAIN LOOP
	
	
	sei();	
}

void set(unsigned int x)
{
	if (Gia_tri_moi != x) {
		OCR1A = x;
		Gia_tri_moi = OCR1A;
	}
	else {
		return; // thoát ngay
	}
}
/*ISR(TIMER3_OVF_vect){
	if(a == 0){
//		OCR1B = 4800;
		a = 1;
		b = 0;
	}
	else {
		OCR1B = 800;
		a = 0;
		b = 1;
	}
}*/

int main(void)
{
	setupServo();
    /* Replace with your application code */
    while (1) 
    {
		set(8480);
		delay_ms(2000);
		set(38550);
		delay_ms(2000);
	}
}

