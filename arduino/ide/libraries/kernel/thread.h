/**
  * Arduino kernel - a thread system
  */
/*------------------------------------------------------------*/
#ifndef thread_h
#define thread_h
/*------------------------------------------------------------*/
#include "Arduino.h"
/*------------------------------------------------------------*/
typedef void(*vFctPtr)(void);
/*------------------------------------------------------------*/
class Thread {
	public:
		Thread(vFctPtr loop);
		vFctPtr loop;
		unsigned long sleepUntil;
};
/*------------------------------------------------------------*/
#endif
