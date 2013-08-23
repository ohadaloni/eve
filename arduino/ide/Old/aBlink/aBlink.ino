/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {                
  pinMode(12, OUTPUT);     
}

void loop() {
  digitalWrite(12, HIGH);
  delay(200);
  digitalWrite(12, LOW);
  delay(500);
}
