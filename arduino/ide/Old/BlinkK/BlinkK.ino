/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {                
  pinMode(9, OUTPUT);     
}

void loop() {
  digitalWrite(9, HIGH);   // set the LED on
  delay(1000);              // wait for a second
  digitalWrite(9, LOW);    // set the LED off
  delay(1000);              // wait for a second
}

