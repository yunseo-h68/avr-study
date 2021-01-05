/*
 * buzzer.h
 *
 * Created: 2020-05-28 오후 3:59:23
 *  Author: user
 */ 

/*
#ifndef BUZZER_H_
#define BUZZER_H_

#define ON      1
#define OFF     2

#define BUZZER_DDR  DDRB
#define BUZZER_PORT PORTB
#define BUZZER_PIN  4

#define DOL 0
#define RE  1
#define MI  2
#define FA  3
#define SOL 4
#define LA  5
#define SI  6
#define DOH 7

void buzzerInit();
int tone(unsigned int melody);
void noTone();


#endif /* BUZZER_H_ */