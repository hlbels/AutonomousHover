/*
 * main.c
 *
 */

#include "iCapture_timer.h"
#include "USART.h"
//#include <util/delay.h>  

#define DEBUG_ON

#define SPEED_OF_SOUND_IN_CM_S (331/10) // using speed of sound 

volatile float temp = 0;
volatile float distance = 0;
volatile uint16_t dist_whole = 0;
volatile uint16_t dist_dec = 0;
volatile uint16_t ticks_t1 = 0;
volatile uint16_t ticks_t2 = 0;
volatile uint16_t elapsed_time = 0;
volatile static uint16_t tick_count = 0;
char value_buf[7] = { 0 };
char dec_val_buf[7] = { 0 };

void init_input_capture() {
	TIMSK1 = ((1 << ICIE1)); /* Enable ICP Interrupt */
	TCCR1B = ((1 << ICES1) | (1 << ICNC1) | (1 << CS12)); /* Enable rising edge detection,
	 noise cancellation,
	 clock Pre-scaler 256*/
	edge.current_edge = INIT_RISING;
	edge.next_edge = INIT_RISING;
}

void init_timer2() {
	TIFR2 = 1 << OCF2A; /* Clear Output compare match flag  */
	TIMSK2 = 1 << OCIE2A; /* Timer 2 compare match is enabled*/
	TCCR2A = 1 << WGM21; /* CTC Mode*/
	TCCR2B = ((1 << CS21)); /* clock Prescaler 8 */
}

// PWM 
void init_PWM(){

  DDRD |= (1 << PD3); // PD6 as output 
  TCCR0A |= (1 << COM0A1) | (1 << WGM21) | (1 << WGM00); //Fast PWM mode
  TCCR0B |= (1 << CS00); // No prescalar 

}

void update_PWM(uint8_t duty_cycle) {
  OCR2A = duty_cycle; 
}

int main() {
  //sei(); 
  
	DDRC = 0xFF;
	PORTC = 0x00;   //Enable internaRDl pullups on PORTC PINS  SDA(PC4) ,SCL(PC5)

	DDRB = 0b00100000; /*PB0 as INPUT to receive Echo, PB5 as output for Trigger*/
	PORTB = (1 << PB0) | (1 << PB5);
  // PINB = value 

  //DDRD |= (1 << PD5) | (1 << PD6); // set PD5 and PD6 as output

	OCR2A = (uint8_t) OCR2A_VALUE;  /*Timer that generates an interrupt every 20uS*/

	usart_init(BAUD_VAL);

	/**Initialize Timer and ICP**/
	init_input_capture();
	init_timer2();
  //init_PWM(); 

	sei();

	while (1) {
         //update_PWM(128); 

         //Control yellow LED 
         //PORTD |= (1 << PD5); 

        // delay to observe LED 
        //_delay_ms(500); 

        // Turn off yellow LED 
        //PORTD &= ~(1 << PD5); 

        //_delay_ms(500);
		 if (edge.current_edge == FALLING) {
			cli();
			if (ticks_t2 > ticks_t1)
				temp = (float) (ticks_t2 - ticks_t1) / (float) TICKS_VAL;
			else
				temp = (float) ((65535 - ticks_t1) + ticks_t2)
						/ (float) TICKS_VAL;
			temp *= 1000000; 

			elapsed_time = (uint16_t) temp;

      // Printing the pulse length 
      char pulse_length_str[10];
      sprintf(pulse_length_str, "%u", elapsed_time); 
      USART_Tx_string(pulse_length_str); 
      USART_Tx_string("\n"); // Add a newline character for readability   
          
			distance = ((float) SPEED_OF_SOUND_IN_CM_S * (float) elapsed_time)
					/ (float) 2000;
			dist_whole = (uint16_t) distance; //Characteristic part of Number
			dist_dec = (uint16_t) (((float) distance - (float) dist_whole)
					* 1000); //Mantissa of the number
			sei();
			sprintf(dec_val_buf, "%1u", dist_dec);
			sprintf(value_buf, "%u", dist_whole);
			strcat(value_buf, ".");
			strcat(value_buf, dec_val_buf);
      
      // Control LED brightness 
      /*uint8_t duty_cycle = 0; 
      if (dist_whole <= 12) 
        duty_cycle = 255; 
        else if (dist_whole >= 42)
          duty_cycle = 0; 
        else 
        duty_cycle = 255 * ( 42 - dist_whole) / (42 - 12);
      
      update_PWM(duty_cycle);

      // Control yellow LED
     if (dist_whole < 12 || dist_whole > 42)
       PORTD |= (1 << PB5); // Turn on Yellow LED 
     else 
       PORTD &= ~ (1 << PB5); // Turn off yellow LED */  

     
      //Sends the data over USART
      USART_Tx_string(value_buf); 

		}

//#ifdef DEBUG_ON
		//USART_Tx_string(value_buf);
//#endif

		}

	return 0;
}

ISR(TIMER1_CAPT_vect) {

	cli();

	switch (edge.next_edge) {
	case INIT_RISING: {

		ticks_t1 = ICR1L;
		ticks_t1 |= (ICR1H << 8);
		TCCR1B &= ~(1 << ICES1); //Next Interrupt on Falling edge
		edge.next_edge = FALLING; 

	}
		break;

	case RISING: {
		ticks_t1 = (uint16_t) ICR1L;
		ticks_t1 |= (uint16_t) (ICR1H << 8);
		TCCR1B &= ~(1 << ICES1); //Next Interrupt on Falling edge
		edge.current_edge = RISING;
		edge.next_edge = FALLING;  

	}
		break;

	case FALLING: {
		ticks_t2 = (uint16_t) ICR1L;
		ticks_t2 |= (uint16_t) (ICR1H << 8);
		TCCR1B |= (1 << ICES1); //Next Interrupt on Rising edge
		edge.current_edge = FALLING;
		edge.next_edge = RISING;
	}
		break;

	default:
		break;
	}
	sei();

}

ISR(TIMER2_COMPA_vect) {
	cli();
	tick_count++;

	if (tick_count == 1) {
		PORTB |= 1 << PB5; // Trigger High 
  // Change the prescaler for 10us delay 
  TCCR2B = ((1 << CS21) | (1 << CS20)); 
  OCR2A = 5; // 10us delay with 16Mhz clock and prescaler 32

	}

	if (tick_count == 2) {
		PORTB &= ~(1 << PB5); // Trigger low 
  // Reset timer prescaler and OCR2A
  TCCR2B = (1 << CS21); // Prescaler 8  
  OCR2A = OCR2A_VALUE; // Original OCR2A value 

	}

	if (tick_count == 3000 ) { // Best is 3000  
		tick_count = 0;
	}

	sei(); 
}

