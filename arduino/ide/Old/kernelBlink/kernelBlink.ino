/**
  * Arduino kernel
  */
/*------------------------------------------------------------*/
#define MAX_THREADS 32
// int MAX_THREADS = 32;
typedef void(*fctPtr)(void);
fctPtr akSetups[MAX_THREADS];
fctPtr akLoops[MAX_THREADS];
int akDelayUntils[MAX_THREADS];
int akCurrentThread = -1;
int akNumThreads = 0;
/*------------------------------------------------------------*/
/*
 ** add a thread to the kernel Queue
 *
 * call this from setup() for each of the individual threads.
 * (threadSetup is often NULL)
 */
int akAddThread(void (*threadSetup)(), void (*threadLoop)()) {
  if ( akNumThreads == MAX_THREADS ) {
    // alert: num threads exceeded
    return(-1);
  }
  akSetups[akNumThreads] = threadLoop;
  akLoops[akNumThreads] = threadSetup;
  akNumThreads++;
  return(akNumThreads-1);
}
/*------------------------------------------------------------*/
/**
 * initialize the kernel
 *
 * call this from setup() AFTER adding the threads, to initialize the kernel
 */
void akSetup() {
	for(int i=0;i<akNumThreads;i++) {
		akDelayUntils[i] =  0;
		if ( akSetups[i] != NULL )
			(*akSetups[i])();
	}
}
/*------------------------------------------------------------*/
/**
 * call this from loop as the last (and usually only) step
 * when all processes are delayed the arduino spends its time here
 * looping until some time has passed
 */
void akLoop() {
	for(int i=0;i<akNumThreads;i++) {
		int now = millis();
		if ( now < akDelayUntils[i] )
			continue;
		akCurrentThread = i;
		akLoops[i]();
	}
}
/*------------------------------------------------------------*/
/**
 * delay the currently running thread for ms milliSeconds
 *
 * never call delay() from a thread. rather call akDelay().
 * akDelay() returns immediately and causes the next iteration
 * to occur after the specified delay.
 *
 * (call as the last step in the iteration for clarity.)
 */
void akDelay(int ms) {
	akDelayUntils[akCurrentThread] = millis() + ms;
}
/*------------------------------------------------------------*/
/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void BlinkSetup(int pin) {                
  pinMode(pin, OUTPUT);     
}

void BlinkLoop(int pin, int delayTime) {
  digitalWrite(pin, HIGH);
  akDelay(delayTime);
  digitalWrite(pin, LOW);
  akDelay(delayTime);
}
/*------------------------------------------------------------*/
void Blink1Setup() { BlinkSetup(13); }
void Blink2Setup() { BlinkSetup(12); }
void Blink1Loop() { BlinkLoop(13, 1000); }
void Blink2Loop() { BlinkLoop(12, 500); }
/*------------------------------------------------------------*/
void setup() {
  akAddThread(Blink1Setup, Blink1Loop);
  akAddThread(Blink2Setup, Blink2Loop);
  akSetup();
}
void loop() {
  akLoop();
}
