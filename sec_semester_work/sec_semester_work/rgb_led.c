/*
 * rgb_led.c
 *
 * Created: 2020-12-09 오전 11:00:42
 *  Author: Yunseo Hwang
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "rgb_led.h"

#define BV(n) (1 << (n))

static void timer_init()
{
	// Fast PWM 8bit
	// WGM13(0), WGM12(1), WGM11(0), WGM10(1)
	// COM1A1/COM1B1/COM1C1 : 1, COM1A0/COM1B0/COM1C0 : 0
	TCCR1A = BV(WGM10) | BV(COM1A1) | BV(COM1B1) | BV(COM1C1);
	TCCR1B = BV(WGM12) | BV(CS11);
}

void rgb_off()
{
	set_color(0, 0, 0);
}

void rgb_init()
{
	DDRB |= BV(PIN_R) | BV(PIN_G) | BV(PIN_B);
	timer_init();
	PORTB &= ~(BV(PIN_R) | BV(PIN_G) | BV(PIN_B));
}

void set_color(uint8_t red, uint8_t green, uint8_t blue)
{
	OCR1_R = red   & 255;
	OCR1_G = green & 255;
	OCR1_B = blue  & 255;
}

void set_rainbow(uint8_t color)
{
	const uint8_t rainbow_rgb[7][3] = {
		{ 255,   0,   0 },  // red
		{ 255,  50,   0 },  // orange
		{ 255, 255,   0 },  // yellow
		{   0, 255,   0 },  // green
		{   0,   0, 255 },  // blue
		{   0,   5, 255 },  // indigo
		{ 100,   0, 255 }   // violet
	};
	set_color(rainbow_rgb[color % 7][0], rainbow_rgb[color % 7][1], rainbow_rgb[color % 7][2]);
}