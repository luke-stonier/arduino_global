#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "OK " + random(0,1000));
}

void setup() {
  boolean result = WiFi.softAP("JU54LJS", "18122016");
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
