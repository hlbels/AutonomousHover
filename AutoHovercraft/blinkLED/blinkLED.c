#include <avr/io.h>
#include <util/delay.h> 

int main(void) {
  DDRB |= 0b00000001; /* Data Direction Register B: 
                         writing a 1 to the bit enables output. */ 

  // ----- Event loop ----- // 
  while (1) {
//    PORTB = 0b00000001; /* Turn on the first LED bit/pin in PORTB */ 
    PORTB = 0b01010101; 
    _delay_ms(10);    /* wait */ 

    //PORTB = 0b00000000; /* Turn off all B pins, including LED */ 
    _delay_ms(10); 

  }
  return 0; 
}

