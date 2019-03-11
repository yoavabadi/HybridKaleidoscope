// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
int incomingByte = 0;   // for incoming serial data

const int redPin = 5;
const int greenPin = 6;
const int bluePin = 10;

const int ASCII_BASE = 48;
const int RED = ASCII_BASE + 0;
const int GREEN = ASCII_BASE + 1;
const int BLUE = ASCII_BASE + 2;
const int WHITE = ASCII_BASE + 3;
const int PURPLE = ASCII_BASE + 4;
const int YELLOW = ASCII_BASE + 5;
const int NONE = ASCII_BASE + 6;

bool isStart = 0;
unsigned long StartTime;
unsigned long CurrentTime;
const unsigned long minInMillis = 20000;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; // declare accellerometer and gyro variables

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void setup(){
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Wire.begin(); // initiate i2c system
  Wire.beginTransmission(MPU_addr); // be sure we talk to our MPU vs some other device
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true); // done talking over to MPU device, for the moment
  Serial.begin(9600); // initialize serial port to 9600 bps so you can see your debug messages in Arduino IDE via debug channel
  StartTime = millis();
}
void loop(){ // main program loop

  
  Wire.beginTransmission(MPU_addr); // get ready to talk to MPU again
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false); // done talking to MPU for the time being
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  // all the fancy <<8| stuff is to bit shift the first 8 bits to
  // the left & combine it with the next 8 bits to form 16 bits
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
//  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
//  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
//  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
//  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  // the above lines have gathered Accellerometer values for X, Y, Z
  //  as well as Gyroscope values for X, Y, Z
  
  if((millis() - StartTime >= minInMillis) && (AcX > 0 || AcY > 0 || AcZ > 0) && (!isStart)) {
    setColor(255,255,255);  // White
    StartTime = millis();
    delay(2500);
    setColor(0,0,0);  // None
    isStart = true;
  }
  
  Serial.print(""); Serial.print(AcX); // share accellerometer values over debug channel 
  Serial.print(","); Serial.print(AcY);
  Serial.print(","); Serial.println(AcZ);
//  delay(100);
      // send data only when you receive data:
    if (Serial.available() > 0) {
            // read the incoming byte:
            incomingByte = Serial.read();
            
            if(incomingByte == RED){
               setColor(255,0,0);
              }            
              else if(incomingByte == GREEN){
               setColor(0,255,0);
              }              
              else if(incomingByte == BLUE){
               setColor(0,0,255);
              }
              else if(incomingByte == WHITE){
               setColor(255,255,255);
              }
              else if(incomingByte == PURPLE){
               setColor(255,0,255);
              }
              else if(incomingByte == YELLOW){
               setColor(255,100,0);
              }
              else if(incomingByte == NONE){
               setColor(0,0,0);
              }
    } 
}
