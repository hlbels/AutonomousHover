#define FOSC_FOR_USART        16000000UL
#define BAUD_RATE     4800UL // was 9600 
#define BAUD_VAL      FOSC_FOR_USART/16/BAUD_RATE-1


void USART_Tx_byte(char* c );

void usart_init(unsigned int );

void USART_Tx_string( char*);
