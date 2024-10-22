#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include "UART.h"
#include "i2c.h"
#include "mpu6050.h"

FILE usart_str = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

// void servo_init() {
//     // Set PB1 as output
//     DDRB |= (1 << PB1);
// 
//     TCCR1A |= (1<<COM1A1) | (1<<WGM11);
//     TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11); 
//     ICR1 = 39999; 
//  
//     while(1){
//       OCR
//     }
//     // Set TOP value (20ms period)
//     //ICR1 = 20000;
// }

int main(void){
	stdout = &usart_str; // assign printf to usart

	uart_init(); // UART Initialization
	I2C_init(); // I2C Initialization
	MPU_init(); // MPU Initialization
  // servo_init(); // servo Initialization
  MPU_set_accel_range(2); // set accelerometer range to ±2g
	MPU_set_gyro_range(250); // set gyroscope range to ±250°/sec
	MPU_set_sample_rate(0); // set sample rate to the maximum possible


	while(1){
		uint16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
		MPU_read(0x3B,&AcX);
		MPU_read(0x3D,&AcY);
		MPU_read(0x3F,&AcZ);
		MPU_read(0x41,&Tmp);
		MPU_read(0x43,&GyX);
		MPU_read(0x45,&GyY);
		MPU_read(0x47,&GyZ);
    
    // servo_set_position(90); 

		printf("AX=%d\tAY=%d\tAZ=%d\tGX=%d\tGY=%d\tGZ=%d\r\n",AcX,AcY,AcZ,GyX,GyY,GyZ);
		_delay_ms(1000);
	}

return 0; 
}

