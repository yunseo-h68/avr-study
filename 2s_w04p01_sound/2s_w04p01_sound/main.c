/*
 * 2s_w04p01_sound.c
 *
 * Created: 2020-10-21 오전 10:04:03
 * Author : Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BV(n) (1 << n)
#define BUTTON 1
#define SOUND  0

int main(void)
{
	int led_value = 0;
	
	DDRA = 0x00;
	DDRD = 0xFF; 
	PORTD = 0x00;
	
    while (1) 
    {
		if (!(PINA & BV(SOUND))) {
			led_value++;
			if (led_value == 0x10) {
				led_value = 0x00;
			}
			PORTD = led_value;
			_delay_ms(500);			
		}
		
		if (!(PINA & BV(BUTTON))) {
			led_value = 0x00;
			PORTD = led_value;
			_delay_ms(500);
		}
    }
}