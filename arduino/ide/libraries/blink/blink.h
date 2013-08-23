/**
  * Arduino kernel - a thread system
  */
/*------------------------------------------------------------*/
#ifndef blink_h
#define blink_h
/*------------------------------------------------------------*/
#include "Arduino.h"
/*------------------------------------------------------------*/
class Blink {
	public:
		Blink(int pin_, int delay_, Kernel *kernel_);
		void loop();
	private:
		int pin;
		int delay;
		boolean isOn;
		Kernel *kernel;
};
/*------------------------------------------------------------*/
#endif
