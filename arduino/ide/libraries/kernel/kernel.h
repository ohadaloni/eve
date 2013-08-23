/**
  * Arduino kernel - a thread system
  */
/*------------------------------------------------------------*/
#ifndef kernel_h
#define kernel_h
/*------------------------------------------------------------*/
#define MAX_THREADS 31
#define KERNEL_PID 32
#define NUM_BLINK_PINS 4
/*------------------------------------------------------------*/
#include "Arduino.h"
/*------------------------------------------------------------*/
#include "thread.h"
/*------------------------------------------------------------*/
typedef void(*vFctPtr)(void);
/*------------------------------------------------------------*/
class Kernel {
	public:
		Kernel();
		void setup();
		int addThread(vFctPtr threadLoop);
		void loop();
		void kill(int pid);
		void exit();
		void sleep(int seconds);
		void msleep(int milliSeconds);
		void printi(String label, int i);
		void printi2(String label1, int i1, String label2, int i2);
		void printi3(String label1, int i1, String label2, int i2, String label3, int i3);
	private:
		void sig();
		void blink(int pin, int times, int stepSpeed);
		int blinkPins[NUM_BLINK_PINS];
		int blinkIsOns[NUM_BLINK_PINS];
		Thread *threads[MAX_THREADS];
		int currentThread;
		unsigned long now;
};
/*------------------------------------------------------------*/
#endif
