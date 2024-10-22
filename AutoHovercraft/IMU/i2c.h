#ifndef I2C_H_
#define I2C_H_

#include <inttypes.h>

void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_write(uint8_t data);
uint8_t I2C_readACK(void);
uint8_t I2C_readNACK(void);
uint8_t I2C_status(void);

#endif // I2C_H_

