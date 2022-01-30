/*
 * utility.h
 *
 * Created: 10/8/2020 9:47:42 PM
 *  Author: thanh
 */ 

#pragma once
#ifndef UTILITY_H_
#define UTILITY_H_
#include "utility.h"

unsigned int hashAngle(const float& angle) {
	return (unsigned int )(10.5f * angle) + (unsigned int )530;
}

unsigned long  hashSpeed(const uint8_t& speed) {
	return (unsigned long )100000 - (unsigned long )(800 * speed);
}

#endif /* UTILITY_H_ */