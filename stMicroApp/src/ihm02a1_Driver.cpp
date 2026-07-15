#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdfloat>

#include <iocsh.h>
#include <epicsThread.h>

#include <asynOctetSyncIO.h>

#include <epicsExport.h>
#include "ihm02a1_Driver.h"

uint32_t revEndian(uint32_t x){
	uint32_t y=0;
	for(int i = 0; i < 4; i++){
		y <<= 8; //shift LSB over one; shifts zeroes on first run
		y += x & 255; //copy LSB of x into LSB of y
		x >>= 8; //discard LSB
	}
	return y;
}

uint32_t reverseBitShift(int32_t x, uint8_t regLength){ //takes signed input for type compatibility but will throw an error on a negative input
	x &= (1 << regLength) - 1; //zero out unwanted bits
	x <<= 8*(3-(regLength-1)/8)); //rounds up to 24 (1-byte arg), 16, 8 or 0 (4-byte arg)
	uint32_t arg = revEndian(x);
	return arg;
}

