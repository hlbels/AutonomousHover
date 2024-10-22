#include "mpu6050.h"
#include "i2c.h"

#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_SMPLRT_DIV 0x19

void MPU_init(void){
	MPU_write(0x6B,0x00); // wake up MPU6050
}

void MPU_write(uint8_t u8addr, uint8_t u8data){
	I2C_start();
	I2C_write((MPU6050<<1)|0); //write
	I2C_write(u8addr);
	I2C_write(u8data);
	I2C_stop();
}

void MPU_read(uint8_t u8addr, uint16_t *u16data){
	uint8_t u8data;
	I2C_start();
	I2C_write((MPU6050<<1)|0); //write
	I2C_write(u8addr);
	I2C_start();
	I2C_write((MPU6050<<1)|1); //read
	u8data = I2C_readACK(); //High
	*u16data = (u8data<<8);
	u8data = I2C_readNACK(); //Low
	*u16data |= u8data;
	I2C_stop();
}

void MPU_set_accel_range(uint8_t range){
    switch(range) {
        case 2:  MPU_write(MPU6050_ACCEL_CONFIG, 0x00); break; // ±2g
        case 4:  MPU_write(MPU6050_ACCEL_CONFIG, 0x08); break; // ±4g
        case 8:  MPU_write(MPU6050_ACCEL_CONFIG, 0x10); break; // ±8g
        case 16: MPU_write(MPU6050_ACCEL_CONFIG, 0x18); break; // ±16g
    }
}

void MPU_set_gyro_range(uint16_t range){
    switch(range) {
        case 250:  MPU_write(MPU6050_GYRO_CONFIG, 0x00); break; // ±250°/sec
        case 500:  MPU_write(MPU6050_GYRO_CONFIG, 0x08); break; // ±500°/sec
        case 1000: MPU_write(MPU6050_GYRO_CONFIG, 0x10); break; // ±1000°/sec
        case 2000: MPU_write(MPU6050_GYRO_CONFIG, 0x18); break; // ±2000°/sec
    }
}

void MPU_set_sample_rate(uint8_t rate){
    MPU_write(MPU6050_SMPLRT_DIV, rate);
}

