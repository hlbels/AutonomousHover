#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include <Servo.h>

#define OUTPUT_READABLE_YAWPITCHROLL

int trigPin = 13; // D3 LED
int led = 11; 

// MPU control/status vars
MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Servo servo;

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 gy;         // gyroscope sensor measurements  
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  Wire.begin();
  Wire.setWireTimeout(3000, true); 
  Serial.begin(115200);
  TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

  // initialize device
  mpu.initialize();

  // load and configure the DMP
  mpu.dmpInitialize();

  //mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  //mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
  //mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);
   
  //mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  //mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_4);
  mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
  //mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_16); 

  // make sure it worked (returns 0 if so)
  if (mpu.dmpInitialize() == 0) {
    // turn on the DMP, now that it's ready
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    while (1);
  }

  pinMode(trigPin, OUTPUT);
  pinMode(led, OUTPUT); 
  servo.attach(6);

  // configure LED for output
  //pinMode(trigPin, OUTPUT);
  //pinMode(led, OUTPUT); 
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  if(fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    fifoCount = mpu.getFIFOCount();
  } else if(fifoCount >= 42) {
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    
    //track FIFO count here in case there is > 1 packet available
    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    float yaw = ypr[0] * 180/M_PI;
    float pitch = ypr[1] * 180/M_PI;
    float roll = ypr[2] * 180/M_PI;

    mpu.getAcceleration(&aa.x, &aa.y, &aa.z);
    //float accelerationX = aa.x / 16384.0;  // divide by sensitivity scale factor for ±2g
    //float accelerationY = aa.y / 16384.0; 
    //float accelerationZ = aa.z / 16384.0; 
    
    //float accelerationX = aa.x / 8192.0;  // divide by sensitivity scale factor for ±4g
    //float accelerationY = aa.y / 8192.0; 
    //float accelerationZ = aa.z / 8192.0; 

    float accelerationX = aa.x / 4096.0;  // divide by sensitivity scale factor for ±8g
    float accelerationY = aa.y / 4096.0; 
    float accelerationZ = aa.z / 4096.0; 

    //float accelerationX = aa.x / 2048.0;  // divide by sensitivity scale factor for ±16g
    //float accelerationY = aa.y / 2048.0; 
    //float accelerationZ = aa.z / 2048.0; 

    mpu.getRotation(&gy.x, &gy.y, &gy.z); 
    //float gyroX = gy.x / 131.0; // divide by sensitivity scale factor for ±250°/sec
    //float gyroY = gy.y / 131.0; // divide by sensitivity scale factor for ±250°/sec
    //float gyroZ = gy.z / 131.0; // divide by sensitivity scale factor for ±250°/sec

    //float gyroX = gy.x / 65.5; // divide by sensitivity scale factor for ±500°/sec
    //float gyroY = gy.y / 65.5; // divide by sensitivity scale factor for ±500°/sec
    //float gyroZ = gy.z / 65.6; // divide by sensitivity scale factor for ±500°/sec
    
    float gyroX = gy.x / 32.8; // divide by sensitivity scale factor for ±1000°/sec
    float gyroY = gy.y / 32.8; // divide by sensitivity scale factor for ±1000°/sec
    float gyroZ = gy.z / 32.8; // divide by sensitivity scale factor for ±1000°/sec


    //float gyroX = gy.x / 16.4; // divide by sensitivity scale factor for ±2000°/sec
    //float gyroY = gy.y / 16.4; // divide by sensitivity scale factor for ±2000°/sec
    //float gyroZ = gy.z / 16.4; // divide by sensitivity scale factor for ±2000°/sec


    if(yaw < -80 || yaw > 80) { 
      digitalWrite(trigPin, HIGH); 
    } else {
      digitalWrite(trigPin, LOW);
    }

    int servoAngle = map(yaw, -80, 80, 0, 180);
    servo.write(constrain(servoAngle, 0, 180));
    // servo.write(180);

    float absAccelX = abs(accelerationX);
    int ledBrightness = 0;

    if (absAccelX > 0.01) {
      if (absAccelX >= 1.00) {
        ledBrightness = 255;  // LED 100% ON
      } else {
        ledBrightness = map(absAccelX * 100, 1, 100, 0, 255);
      }
    }

    analogWrite(led, ledBrightness);
    
    Serial.print("Yaw: "); Serial.println(yaw);
    Serial.print("Pitch: "); Serial.println(pitch);
    Serial.print("Roll: "); Serial.println(roll);
    Serial.print("AccelerationX: "); Serial.println(accelerationX);
    //Serial.print("AccelerationY: "); Serial.println(accelerationY);
    //Serial.print("AccelerationZ: "); Serial.println(accelerationZ);
    //Serial.print("GyroX: "); Serial.println(gyroX);
    //Serial.print("GyroY: "); Serial.println(gyroY);
    //Serial.print("GyroZ: "); Serial.println(gyroZ);

    delay(1000);
  }
}
