/**
  * Blink Utilities
  */
/*------------------------------------------------------------*/
#include "Arduino.h"
#include "kernel.h"
#include "blink.h"
/*------------------------------------------------------------*/
Blink::Blink(int pin_, int delay_, Kernel *kernel_) {
	pin = pin_;
	delay = delay_;
	kernel = kernel_;
	pinMode(pin, OUTPUT);
	isOn = false;
}
/*------------------------------------------------------------*/
void Blink::loop() {
	if ( kernel == NULL ) {
		Serial.println("Blink: No Kernel");
		return;
	}
	if ( isOn ) {
		isOn = false;
		digitalWrite(pin, HIGH); // turn it off
		kernel->msleep(delay);
	} else {
		isOn = true;
		digitalWrite(pin, LOW);
		kernel->msleep(delay);
	}
}
/*------------------------------------------------------------*/
