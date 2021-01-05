/*
 * w04work01_2digit_segment_updown.c
 *
 * Created: 2020-06-17 오전 10:11:38
 * Author : user
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(n) (1 << n)

void SegOn(unsigned char i, unsigned char num)
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
	PORTA = SegAnode[num];
	PORTE = BV(i);
	PORTE &= ~BV(!i);
}

int main(void)
{
	int i = 0, j = 0;
	int f = 0;
	
	DDRA = 0xFF;
	DDRE = 0x03;
	
	while (1)
	{
		if(~PINE & BV(4)) f = 1;
		else if(~PINE & BV(5)) f = -1;
		
		if(i == -1 && f == -1) i = 99;
		else if(i == 100 && f == 1) i = 0;
		
		for(j = 0; f && j < 50; j++)
		{
			if(~PINE & BV(4)) f = 1;
			else if(~PINE & BV(5)) f = -1;
			
			SegOn(0, i/10);
			_delay_ms(5);
			SegOn(1, i%10);
			_delay_ms(5);
		}
		
		i += f;
	}
}
