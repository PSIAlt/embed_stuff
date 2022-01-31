#include <Arduino.h>

#define LIGHT_PIN PD2

// Period in seconds
#define ON_PERIOD 14
#define OFF_PERIOD 10

void setup() {
  pinMode(LIGHT_PIN, OUTPUT);
}

void loop() {
  // Loop to manage the light
  digitalWrite(LIGHT_PIN, HIGH);
  delay(ON_PERIOD * 1000);
  digitalWrite(LIGHT_PIN, LOW);
  delay(OFF_PERIOD * 1000);
}
