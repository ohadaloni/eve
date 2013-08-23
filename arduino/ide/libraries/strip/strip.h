/**
  * Arduino RGB LED Strip Utilities
  */
/*------------------------------------------------------------*/
#ifndef strip_h
#define strip_h
/*------------------------------------------------------------*/
#include "Arduino.h"
#include "kernel.h"
/*------------------------------------------------------------*/
#define	NUM_LEDS	32
/*------------------------------------------------------------*/
#define	SDI	2 //Red wire (not the red 5V wire!)
#define	CKI	3 //Green wire
/*------------------------------------------------------------*/
#define	NUM_LEADS	12
// (indices to arrays)
#define LEAD_1		0
#define LEAD_2		1
#define LEAD_3		2
#define aVR			3
#define aVL			4
#define aVF			5
#define V1			6
#define V2			7
#define V3			8
#define V4			9
#define V5			10
#define V6			11
/*------------------------------------------------------------*/
class Strip {
	public:
		Strip(Kernel *kernel_);
		void header(int activeLeads_[], int numActiveLeads_); // initialize set of activeLeads
		void data(int colors[]); // receive and display this array of numActiveLeads numbers
		void off();
		void setup();
		void stripSetup();
		void stripLoop();
		void addRandom();
	private:
		Kernel *kernel;
		int leadMap[NUM_LEADS]; // position of lead on the strip
		int activeLeads[NUM_LEADS]; // list if active leads
		int numActiveLeads;
		long stripColors[NUM_LEDS];
		long map(int grayScaled);
		void push();
};
/*------------------------------------------------------------*/
#endif
