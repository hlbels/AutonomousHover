#ifndef INPUTCAPTURE_TIMER_H_
#define INPUTCAPTURE_TIMER_H_
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SET   1
#define CLEAR 0

#define FOSC      16000000UL
#define baud_rate 4800 // was 9600 
#define baud      FOSC/16/baud_rate-1
#define TICKS_VAL (FOSC/256)
#define F_OC2A    50000L
#define OC2A_PRESCALER 8L
#define OCR2A_VALUE  (float)( (FOSC/(2*OC2A_PRESCALER*F_OC2A)) -1)

typedef enum {INIT_RISING, RISING, FALLING} edge_state_t;

typedef struct {
	edge_state_t current_edge;
	edge_state_t next_edge;

} edge_t;

volatile edge_t edge;

/**Function prototypes*/
void init_input_capture();
void init_timer2();


#endif /* INPUTCAPTURE_TIMER_H_ */

