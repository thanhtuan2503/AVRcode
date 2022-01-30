/*
 * ConMeNo.h
 *
 * Created: 12/2/2019 9:27:31 PM
 *  Author: Asus
 */ 


#ifndef MYDELAY_H_
#define MYDELAY_H_

/*
 * LedBlinking.cpp
 *
 * Created: 12/2/2019 8:33:01 PM
 * Author : Asus
 */ 

#include<avr/cpufunc.h>
int _iteratorI, _iteratorJ;
inline void delay_ms(uint32_t delayTime) {
	_iteratorI = 0;
	while (_iteratorI < delayTime) {
		_iteratorJ = 0;
		while(_iteratorJ < 2000) {
			_NOP();
			_NOP();
			_NOP();
			_iteratorJ++;
		}
		_iteratorI++;
	}
}

inline void delay_us(uint32_t delayTime) {
	_iteratorI = 0;
	while (_iteratorI < delayTime) {
		_iteratorJ = 0;
		while(_iteratorJ < 2) {
			_NOP();
			_NOP();
			_NOP();
			_iteratorJ++;
		}
		_iteratorI++;
	}
}




#endif /* MYDELAY_H_ */