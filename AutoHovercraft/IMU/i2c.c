#include "i2c.h"
#include <avr/io.h>

void I2C_init(void){
	TWBR = 0x5C;// TWI clock 100kHz, SCL frequency = F_CPU/(16 + 2(TWBR)*(TWSR prescale))
	TWCR = (1 << TWEN);
}

void I2C_start(void){
	TWCR = (1 << TWEN) | (1 << TWINT) |(1 << TWSTA);
	while ((TWCR & (1 << TWINT)) == 0);
}

void I2C_stop(void){
	TWCR = (1 << TWEN) | (1 << TWINT) |(1 << TWSTO);
}

void I2C_write(uint8_t data){
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ((TWCR & (1 << TWINT)) == 0);
}

uint8_t I2C_readACK(){
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWEA);//with acknowledge
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

uint8_t I2C_readNACK(){
	TWCR = (1 << TWINT)|(1 << TWEN);//without acknowledge
	while ((TWCR & (1 << TWINT)) == 0);
	return TWDR;
}

uint8_t I2C_status(void){
	uint8_t status;
	status = TWSR & 0xF8;
	return status;
}

