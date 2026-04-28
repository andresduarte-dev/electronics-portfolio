// Project 02 - LED Brightness Control with PWM
// Author: Andrés Duarte Soza
// Date: 2026-04-27

const int potPin = A0;
const int ledPin = 9;
int potValue = 0;
int ledValue = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);
  ledValue = map(potValue, 0, 1023, 0, 255);
  analogWrite(ledPin, ledValue);
  Serial.println(ledValue);
  delay(15);
}