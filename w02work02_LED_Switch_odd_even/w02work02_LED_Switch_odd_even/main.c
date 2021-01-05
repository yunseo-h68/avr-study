/*
 * w02work02_LED_Switch_odd_even.c
 *
 * Created: 2020-06-04 오전 9:07:03
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(bit) (1<<bit)

int main(void)
{
	DDRA = 0xFF;
	DDRE = 0x00;
	PORTA = 0x00;
	
    while (1) 
    {
		if(~PINE & BV(4)) PORTA = 0x02;
		else if(~PINE & BV(5)) PORTA = 0x01;
		
		if(PORTA == 0x55){
			_delay_ms(500);
			PORTA = 0x00;
			_delay_ms(500);
			PORTA = 0x01;
		}
		else if(PORTA == 0xAA){
			_delay_ms(500);
			PORTA = 0x00;
			_delay_ms(500);
			PORTA = 0x02;
		}
		
		_delay_ms(500);
		PORTA |= (PORTA << 2);
    }
}

