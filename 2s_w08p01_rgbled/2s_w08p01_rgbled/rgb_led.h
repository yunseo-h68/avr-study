/*
 * rgb_led.h
 *
 * Created: 2020-12-09 오전 11:02:15
 *  Author: user
 */ 


#ifndef RGB_LED_H_
#define RGB_LED_H_

#define OCR1_R OCR1C
#define OCR1_G OCR1B
#define OCR1_B OCR1A

void timer_init();
void set_color(int red, int green, int blue);

#endif /* RGB_LED_H_ */