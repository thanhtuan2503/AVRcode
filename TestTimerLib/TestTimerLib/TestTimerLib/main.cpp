/*
 * TestTimerLib.cpp
 *
 * Created: 4/14/2020 9:00:58 PM
 * Author : thanh
 */ 

#include "millisFunc.h"
#include "bitOp.h"
#include "myDelay.h"

void setup(){
	SET(DDRD, 7);
	SET(PORTD, 7);
}

int main(void)
{
	setup();
	initMillis();
	unsigned long pre_millis;
	pre_millis = millis();
    /* Replace with your application code */
    while (1) 
    {
		if (millis() - pre_millis > 500)
		{
			INV(PORTD, 7);
			pre_millis = millis();
		}
    }
	return 0;
}

