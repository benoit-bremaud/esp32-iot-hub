#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Ready!");
}

void loop() {
  delay(1000);
}
