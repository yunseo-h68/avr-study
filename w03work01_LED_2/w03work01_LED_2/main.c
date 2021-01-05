/*
 * w03work01_LED_2.c
 *
 * Created: 2020-06-10 오전 8:58:27
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(n) (1<<n)
#define SW0 4
#define SW1 5

int main(void)
{
	DDRA = 0xFF;
	DDRE = 0x00;
	PORTA = 0x00;
	
    while (1) 
    {
		if((~PINE & BV(SW0)) && (~PINE & BV(SW1))) 
		{
			PORTA = 0xFF;
		} else if(!(~PINE & BV(SW0)) && !(~PINE & BV(SW1)))
		{
			PORTA = 0x00;
		} else if((~PINE & BV(SW0))&& !(~PINE & BV(SW1)))
		{
			PORTA = 0x03;
		} else if(!(~PINE & BV(SW0)) && (~PINE & BV(SW1)))
		{
			PORTA = 0x0F;
		}
    }
}

