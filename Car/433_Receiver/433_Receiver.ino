#include <RH_ASK.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 150

RH_ASK driver;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
String lastR, lastG, lastB;

void setup()
{
  //Serial.begin(9600);  // Debugging only
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(25); // Set BRIGHTNESS to about 1/5 (max = 255)
  colorWipe(strip.Color(255,255,255), 1, false);
  
  if (!driver.init()) {}
       //Serial.println("init failed");
  
  //Serial.println("Listening");
}

void loop()
{
    uint8_t buf[12];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      
      //Serial.print("Message: ");
      String message = (char*)buf;
      //Serial.println(message);
      int comma1 = message.indexOf(',');
      String r = message.substring(0, comma1);
      int comma2 = message.indexOf(',', comma1 + 1);
      String g = message.substring(comma1+1, comma2);
      int comma3 = message.indexOf(',', comma2 + 1);
      String b = message.substring(comma2+1, comma3);
      if (r != lastR || g != lastG || b != lastB) {
        lastR = r;
        lastG = g;
        lastB = b;
        colorWipe(strip.Color(r.toInt(),g.toInt(),b.toInt()), 10, true);
      }
    }

    // colorWipe(strip.Color(255,255,255), 1, true);
    // colorWipe(strip.Color(0,0,0), 1, true);
}

void colorWipe(uint32_t color, int wait, bool shouldWait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    if (shouldWait) {
      delay(wait);                           //  Pause for a moment
    }
  }
}
