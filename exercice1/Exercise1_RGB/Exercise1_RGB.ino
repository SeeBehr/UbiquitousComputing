#include <WiFiNINA.h>

void setup() {
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  analogWrite(LEDR, 255);
}

void loop() {
  for(int i = 0; i <= 255; i += 2) {
    analogWrite(LEDR, 255-i);
    analogWrite(LEDG, i);
    delay(2);
  }
  for(int i = 0; i <= 255; i += 2) {
    analogWrite(LEDG, 255-i);
    analogWrite(LEDB, i);
    delay(2);
  }
  for(int i = 0; i <= 255; i += 2) {
    analogWrite(LEDB, 255-i);
    analogWrite(LEDR, i);
    delay(2);
  }
}