#ifndef UART_H_
#define UART_H_
#include <stdio.h>

void uart_init(void);
void uart_setchar(char c);
char uart_getchar(void);
int usart_putchar_printf(char var, FILE *stream);

#endif // UART_H_

