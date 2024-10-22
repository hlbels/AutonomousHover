#ifndef MPU6050_H_
#define MPU6050_H_
#include <stdint.h>

#define MPU6050 0x68

void MPU_init(void);
void MPU_write(uint8_t u8addr, uint8_t u8data);
void MPU_read(uint8_t u8addr, uint16_t *u16data);
void MPU_set_accel_range(uint8_t range);
void MPU_set_gyro_range(uint16_t range);
void MPU_set_sample_rate(uint8_t rate);

#endif // MPU6050_H_

