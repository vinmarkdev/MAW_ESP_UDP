#include <Arduino.h>
#include <WiFi.h>

const int LED_PIN = LED_BUILTIN; // or 8 for C3 DevKitC
const bool LED_ACTIVE_LOW = true;
const int LED_ON = LED_ACTIVE_LOW ? LOW : HIGH;
const int LED_OFF = LED_ACTIVE_LOW ? HIGH : LOW;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LED_OFF);
}

void loop() {
  digitalWrite(LED_PIN, LED_ON);
  delay(500);
  digitalWrite(LED_PIN, LED_OFF);
  delay(500);
}