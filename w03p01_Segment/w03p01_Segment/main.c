/*
 * w03p01_Segment.c
 *
 * Created: 2020-06-10 오전 11:46:04
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	const unsigned char SegAnode[10] = {
		0x03, // 0
		0x9F, // 1
		0x25, // 2
		0x0D, // 3
		0x99, // 4
		0x49, // 5
		0x41, // 6
		0x1F, // 7
		0x01, // 8
		0x09, // 9
	};
	int i = 0;
	
	DDRA = 0xFF;
	
    while (1) 
    {
		for(i = 0; i < 10; i++) 
		{
			PORTA = SegAnode[i];
			_delay_ms(500);
		}
    }
}

