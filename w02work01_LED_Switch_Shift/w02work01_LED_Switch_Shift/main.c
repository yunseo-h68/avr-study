/*
 * w02work01_LED_Switch_Shift.c
 *
 * Created: 2020-06-03 오전 10:00:48
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(bit) (1 << bit)

int main(void)
{	
	int dir = -1;
	
	DDRA = 0xFF;
	DDRE = 0x00;
	PORTA = 0x01;
	
    while (1)
    {
		PORTA = PORTA << dir;
		PORTA = PORTA >> (!dir);
		_delay_ms(500);
		
		if(dir && PORTA == 0x00) PORTA = 0x01;
		else if(!dir && PORTA == 0x00) PORTA = 0x80;
		
		if(dir && PORTA == 0x80) {
			PORTA = PORTA << dir;
			_delay_ms(500);
			PORTA = 0x01;
			_delay_ms(500);
		}
		if(!dir && PORTA == 0x01) {
			PORTA = PORTA >> 1;
			_delay_ms(500);
			PORTA = 0x80;
			_delay_ms(500);
		}
		
		if(~PINE & BV(4)) dir = 1;
		else if(~PINE & BV(5)) dir = 0;
    }
}

