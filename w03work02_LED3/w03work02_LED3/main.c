/*
 * w03work02_LED3.c
 *
 * Created: 2020-06-10 오전 9:26:41
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(n) (1 << n)
#define SW0 4
#define SW1 5

#define READ_SW(n) (~PINE & BV(n))

int main(void)
{
	DDRA = 0xFF;
	DDRE = 0x00;
	PORTA = 0x00;
	
	while (1)
	{
		if(READ_SW(SW0) && READ_SW(SW1))
		{
			PORTA = 0xAA;
			_delay_ms(500);
			if(!READ_SW(SW0) || !READ_SW(SW1)) continue;
		} 
		else if(READ_SW(SW0)&& !READ_SW(SW1))
		{
			PORTA = 0xCC;
			_delay_ms(500);
			if(!READ_SW(SW0) || READ_SW(SW1)) continue;
		} 
		else if(!READ_SW(SW0) && READ_SW(SW1))
		{
			PORTA = 0xF0;
			_delay_ms(500);
			if(READ_SW(SW0) || !READ_SW(SW1)) continue;
		} 
		else
		{
			PORTA = 0xFF;
			_delay_ms(500);
			if(READ_SW(SW0) || READ_SW(SW1)) continue;
		}
		
		PORTA = ~PORTA;
		_delay_ms(500);
	}
}

