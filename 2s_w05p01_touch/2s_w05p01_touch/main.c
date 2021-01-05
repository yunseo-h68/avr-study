/*
 * 2s_w05p01_touch.c
 *
 * Created: 2020-10-28 오전 9:08:21
 * Author : Yunseo Hwang
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#define BV(bit) (1 << bit)
volatile int tone;
char f_table[9] = {17,43,66,77,97,114,127,137,255};
	
ISR(TIMER0_OVF_vect){
	PORTB ^= 1<<4;
	TCNT0 = f_table[tone];
}
int main(void)
{
	int cnt = 0;
    while(1)
    {
        DDRA=0x00;          // PD0 = Sound Sensor
        DDRB=0x10;
		TCCR0 = 0x03;
		TIMSK = 0x01;
		sei();
		tone=8; 
        while(1)
        {
			if(PINA & 0x01){
				tone = cnt;
				_delay_ms(500);
				cnt ++;
				if(cnt==8) cnt=0;
			}
        }
    }
}

/*
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TOUCH 0
#define LED 0

#define BV(n) (1 << n)

#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define RA 5
#define SI 6
#define DDO 7

#define ON 0
#define OFF 1

volatile int state, tone;
char f_table[8] = {17, 43, 66, 77, 97, 114, 117, 137};
int song[] = {DO, RE, MI, FA, SOL, RA, SI, DDO};

ISR(TIMER0_OVF_vect)
{
	if (state == OFF)
	{
		PORTB |= 1 << 4;
		state = ON;
	}
	else
	{
		PORTB &= ~(1 << 4);
		state = OFF;
	}
	TCNT0 = f_table[tone];
}

int main()
{
	int i=0;
	DDRA = 0x00;
	DDRB = 0x10;
	DDRD = 0x01;              
	TCCR0 = 0x03;                   
	TIMSK = 0x01;                 
	TCNT0 = f_table[song[i]];
	
	sei();
	while(1) {
		if (PINA & BV(TOUCH)) {
			PORTD |= BV(LED);
			state = ON;
			tone = song[i % 8];
			i++;
			_delay_ms(500);
			PORTD &= ~BV(LED);
		}
		state = OFF;
	}
}
*/