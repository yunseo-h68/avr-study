/*
 * buzzer.c
 *
 * Created: 2020-05-28 오후 3:59:01
 *  Author: user
 */ 
/*
#define F_CPU 16000000UL
#include <avr/io.h>
#include "buzzer.h"
#define BV(bit) (1 << bit)

char f_table[8] = {17, 43, 66, 77, 97, 114, 117, 137};

void buzzerInit()
{
	BUZZER_DDR |= BV(BUZZER_PIN);
	TCCR0 |= (1<<CS01) | (1<<CS00);
	TIMSK |= (1<<TOIE0);
}

int tone(unsigned int melody)
{
	BUZZER_PORT |= BV(BUZZER_PIN);
	TCNT0 = f_table[melody];
	return 0;
}

void noTone() 
{
	BUZZER_PORT &= ~BV(BUZZER_PIN);
}
*/