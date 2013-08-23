#include <kernel.h>
/*------------------------------------------------------------*/
#define NTHREADS 4
/*------------------------------------------------------------*/
Kernel kernel = Kernel();
/*------------------------------------------------------------*/
boolean blinkIsOns[NTHREADS];
int blinkIsPinMode[NTHREADS] = { 0, 0, 0, 0 };
/*------------------------------------------------------------*/
int pins[NTHREADS] = { 9, 10, 11, 12 };;
int delayTimes[NTHREADS] =  { 100, 600, 900, 1200 };
/*------------------------------------------------------------*/
void BlinkSetup(int blinkIndex) {
	if ( blinkIsPinMode[blinkIndex] == 0 ) {
		pinMode(pins[blinkIndex], OUTPUT);
		blinkIsPinMode[blinkIndex] = 1;
		digitalWrite(pins[blinkIndex], HIGH);
	}
	blinkIsOns[blinkIndex] = false;
}
/*------------------------------------------------------------*/
void BlinkLoop(int blinkIndex) {
	if ( blinkIsOns[blinkIndex] ) {
		blinkIsOns[blinkIndex] = false;
		digitalWrite(pins[blinkIndex], HIGH);
		kernel.msleep(delayTimes[blinkIndex]);
	} else {
		blinkIsOns[blinkIndex] = true;
		digitalWrite(pins[blinkIndex], LOW);
		kernel.msleep(delayTimes[blinkIndex]);
	}
}
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
void Blink0Setup() { BlinkSetup(0); }
void Blink1Setup() { BlinkSetup(1); }
void Blink2Setup() { BlinkSetup(2); }
void Blink3Setup() { BlinkSetup(3); }
void Blink0Loop() { BlinkLoop(0); }
void Blink1Loop() { BlinkLoop(1); }
void Blink2Loop() { BlinkLoop(2); }
/*	void Blink3Loop() { BlinkLoop(3); }	*/
/*------------------------------------------------------------*/
int blink0pid;
int blink3state;
/*------------------------------*/
#define STATE_RESTING 0
#define STATE_BLINIKING_OFF 1
#define STATE_BLINIKING_ON 2
/*------------------------------*/
void Blink3Loop() {
	switch ( blink3state ) {
		case STATE_RESTING :
				digitalWrite(12, HIGH);
			break;
		case STATE_BLINIKING_OFF :
				blink3state =  STATE_BLINIKING_ON;
				digitalWrite(12, LOW);
			break;
		case STATE_BLINIKING_ON :
				blink3state =  STATE_BLINIKING_OFF;
				digitalWrite(12, HIGH);
			break;
	}
	kernel.msleep(100);
}
/*------------------------------*/
void Blink3ControllerSetup() {
	blink3state =  STATE_RESTING;
}
/*------------------------------*/
void Blink3ControllerLoop() {
	if ( blink3state == STATE_RESTING ) {
		blink3state = STATE_BLINIKING_OFF;
		kernel.msleep(1000);
	} else {
		blink3state = STATE_RESTING;
		kernel.msleep(3000);
	}
}
/*------------------------------------------------------------*/
void Blink0ControllerLoop() {
	if ( blink0pid >= 0 ) {
		kernel.kill(blink0pid);
		blink0pid = -1;
		digitalWrite(9, HIGH);
		kernel.msleep(2000);
	} else {
		blink0pid = kernel.addThread(Blink0Loop);
		/*	kernel.printi("Blink0ControllerLoop: Starting pid", blink0pid);	*/
		kernel.msleep(1000);
	}
}
/*------------------------------------------------------------*/
void setup() {
	kernel.setup();
	/*	Blink0Setup();	*/
	Blink1Setup();
	Blink2Setup();
	Blink3Setup();
	/*	kernel.addThread(Blink0Loop);	*/
	kernel.addThread(Blink1Loop);
	kernel.addThread(Blink2Loop);
	kernel.addThread(Blink3Loop);
	blink0pid = -1;
	kernel.addThread(Blink0ControllerLoop);
}
/*------------------------------*/
void loop() {
	kernel.loop();
}
/*------------------------------------------------------------*/
