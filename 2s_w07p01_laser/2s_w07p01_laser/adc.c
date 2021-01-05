/*
 * adc.c
 *
 * Created: 2020-07-29 오전 11:08:09
 *  Author: Yunseo Hwang
 */ 

#include "adc.h"

void ADC_init()
{
	DDRF = 0x00;
	ADCSRA = 0x87;
	ADMUX = 0x40;
}

int ADC_read(char channel)
{	
	ADMUX = (0x40) | (channel & 0x07);
	_delay_ms(50);
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADIF)) == 0);
	
	ADCSRA |= (1 << ADIF);
	return ADCW;
}
