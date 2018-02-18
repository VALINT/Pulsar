/*
 * _1801_One_Wire_7Seg.cpp
 *
 * Created: 11.01.2018 16:26:31
 *  Author: VAL
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "Pulsar7Seg.h"

#define DATA 0

//float num = 0;
int num = 0;

int main(void)
{
	DisplayInit();
	while(1)
	{
		for(uint8_t i = 0; i <10; i +=1)
		{
			PrintInteger(num);
		}
		num += 10;
		//_delay_ms(100);
	}
}

