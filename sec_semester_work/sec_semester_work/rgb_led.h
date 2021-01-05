/*
 * rgb_led.h
 *
 * Created: 2020-12-09 오전 11:02:15
 *  Author: user
 */ 


#ifndef RGB_LED_H_
#define RGB_LED_H_

// Define for Rainbow color
#define COLOR_RED 0
#define COLOR_ORANGE 1
#define COLOR_YELLOW 2
#define COLOR_GREEN  3
#define COLOR_BLUE   4
#define COLOR_INDIGO 5
#define COLOR_VIOLET 6

#define PIN_R PORTB5
#define PIN_G PORTB6
#define PIN_B PORTB7

#define OCR1_R OCR1A
#define OCR1_G OCR1B
#define OCR1_B OCR1C

void set_color(uint8_t red, uint8_t green, uint8_t blue);
void set_rainbow(uint8_t color);
void rgb_off();
void rgb_init();

#endif /* RGB_LED_H_ */