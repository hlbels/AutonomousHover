#include "UART.h"
#include <avr/io.h>
#include <stdio.h>

#define F_CPU 16000000
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD)-1)

void uart_init(void){
	UBRR0L = BRC;
	UBRR0H = (BRC >> 8); //Baud rate setting
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); //RxTx enable
	//UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8bit char
}

void uart_setchar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

char uart_getchar(void) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

int usart_putchar_printf(char var, FILE *stream) {
    uart_setchar(var);
    return 0;
}

