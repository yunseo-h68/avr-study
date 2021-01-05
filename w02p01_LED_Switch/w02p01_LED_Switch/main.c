/*
 * w02p01_LED_Switch.c
 *
 * Created: 2020-06-03 오전 9:44:17
 * Author : Yunseo Hwang
 */ 

#include <avr/io.h>


int main(void)
{
	DDRA = 0xFF;
	DDRE = 0x00;
	PORTA = 0x00;
	
    while (1)
    {
		
    }
}

