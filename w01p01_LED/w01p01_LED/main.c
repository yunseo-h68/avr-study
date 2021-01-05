/*
 * w01p01_LED.c
 *
 * Created: 2020-05-27 오전 11:44:20
 * Author : user
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRA = 0xFF;
	while (1)
	{
		PORTA=0xFF;
		_delay_ms(1000);
		PORTA=0x00;
		_delay_ms(1000);
	}
}