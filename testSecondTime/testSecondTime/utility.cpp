#include "utility.h"

unsigned int hashAngle(float angle) {
	return (unsigned int )(10.5f * angle) + (unsigned int )530;
}

unsigned long  hashSpeed(uint8_t speed) {
	return (unsigned long )100000 - (unsigned long )(800 * speed);
}
