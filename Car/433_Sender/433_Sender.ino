#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;
int incomingByte = 0; // for incoming serial data

void setup()
{
  Serial.begin(9600);
  driver.init();
}

void loop()
{
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    Serial.println(">> " + incomingByte);
    const char *msg = "255,0,0,1";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
  }
}
