#include<Wire.h>
#include <Servo.h>

const int MPU_addr=0x68; int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int minVal=265; int maxVal=402;
double x; double y; double z;

Servo rudder;
Servo elevator;

int increment = 1;
int threshold = 20;

int rudderPos = 0;
int rudderDefaultPos = 35;
int rudderMax = 70;
int rudderMin = 0;

int elevatorPos = 0;
int elevatorDefaultPos = 90;
int elevatorMax = 180;
int elevatorMin = 0;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  rudder.attach(9);
  elevator.attach(10);
  
  Serial.begin(9600);
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  
  int xAng = map(AcX,minVal,maxVal,-90,90);
  int yAng = map(AcY,minVal,maxVal,-90,90);
  int zAng = map(AcZ,minVal,maxVal,-90,90);
    
  x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
    
  AdjustAngle(x, y);
  delay(25);
}

void AdjustAngle(int x, int y) {
  if (x > threshold && x < 180) {
    elevatorPos -= increment;
  } else if (x < 360 - threshold && x > 180) {
    elevatorPos += increment;
  } else {
    elevatorPos = elevatorDefaultPos;
  }

  if (y > threshold && y < 180) {
    rudderPos -= increment;
  } else if (y < 360 - threshold && y > 180) {
    rudderPos += increment;
  } else {
    rudderPos = rudderDefaultPos;
  }

  rudderPos = constrain(rudderPos, rudderMin, rudderMax);
  elevatorPos = constrain(elevatorPos, elevatorMin, elevatorMax);
    
  rudder.write(rudderPos);
  elevator.write(elevatorPos);
}
