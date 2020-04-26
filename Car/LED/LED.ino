#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
SoftwareSerial HM10(2, 3); // RX = 2, TX = 3

#define LED_PIN 6
#define LED_COUNT 150

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
char appData;  
String inData = "";
String combinedData = "";

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(25); // Set BRIGHTNESS to about 1/5 (max = 255)
  colorWipe(strip.Color(255,255,255), 1, false);
  delay(1000);
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
}

void loop() {
  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) {   // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData);  // save the data in string format
    if (inData.indexOf(';') > -1) {
      GetLEDData(combinedData);
      inData = "";
      combinedData = "";
    } else {
      combinedData += inData;
    }
  }
}

void GetLEDData(String data) {
  Serial.println("GOT: " + data);
  int _buffer = 0;
  int _index = 0;
  _index = data.indexOf(',', _buffer);
  String r = data.substring(_buffer, _index);
  _buffer = _index;
  _index = data.indexOf(',', _buffer + 1);
  String g = data.substring(_buffer + 1, _index);
  _buffer = _index;
  _index = data.indexOf(',', _buffer + 1);
  String b = data.substring(_buffer + 1, _index); 
  colorWipe(strip.Color(r.toInt(),g.toInt(),b.toInt()), 10, true);
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
