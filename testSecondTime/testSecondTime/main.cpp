/*
 * testSecondTime.cpp
 *
 * Created: 10/8/2020 7:11:47 PM
 * Author : thanh
 */ 

#include <avr/io.h>
#include "servo.h"
#include "utility.h"

int main(void)
{
    /* Replace with your application code */
	servoInit();
	generatePulse(hashAngle(90), hashSpeed(100));	
	//generatePulse(1470, 19760);			
    while (1) 
    {
    }
}

