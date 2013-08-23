/**
  * Arduino kernel
  */
/*------------------------------------------------------------*/
#include "Arduino.h"
#include "kernel.h"
#include "strip.h"
/*------------------------------------------------------------*/
Strip::Strip(Kernel *kernel_) {
	kernel = kernel_;
	leadMap[LEAD_1] = 8;
	leadMap[LEAD_2] = 12;
	leadMap[LEAD_3] = 16;
	leadMap[aVR] = 22;
	leadMap[aVL] = 30;
	leadMap[aVF] = 14;
	leadMap[V1] = 1;
	leadMap[V2] = 2;
	leadMap[V3] = 3;
	leadMap[V4] = 4;
	leadMap[V5] = 5;
	leadMap[V6] = 6;
}
/*------------------------------------------------------------*/
void Strip::setup() {
  pinMode(SDI, OUTPUT);
  pinMode(CKI, OUTPUT);
  
  for(int x = 0 ; x < NUM_LEDS ; x++)
    stripColors[x] = 0;
}
/*------------------------------------------------------------*/
void Strip::header(int activeLeads_[], int numActiveLeads_) {
	kernel->printi("Strip:header: numActiveLeads_", numActiveLeads_);
	kernel->printi3("Strip:header: activeLeads_: 0", activeLeads_[0], "1", activeLeads_[1], "2", activeLeads_[2]);
	numActiveLeads = numActiveLeads_;
	for(int i=0;i<numActiveLeads;i++)
		activeLeads[i] = activeLeads_[i];
	pinMode(SDI, OUTPUT);
	pinMode(CKI, OUTPUT);
}
/*------------------------------------------------------------*/
void Strip::off() {
	for(int i=0;i<NUM_LEDS;i++)
		stripColors[i] = 0;
	push();
}
/*------------------------------------------------------------*/
void Strip::data(int colors[]) {
	for(int i=0;i<NUM_LEDS;i++)
		stripColors[i] = 0;
	for(int i=0;i<numActiveLeads;i++) {
		int lead = activeLeads[i];
		int led = leadMap[lead];
		int color = colors[i];
		long mapped = map(color);
		stripColors[led] = mapped;
		/*	kernel->printi3("Strip::data: i", i, "lead", lead, "led", led);	*/
		/*	kernel->printi("Strip::data: color", color);	*/
		/*	Serial.print("mapped ");	*/
		/*	Serial.println(mapped);	*/
		/*	stripColors[leadMap[activeLeads[i]]] = map(colors[i]);	*/
	}
	push();
}
/*------------------------------------------------------------*/
/*
 * in comes a grayScaled number ranging from -256 to 255
 * denoting a semi calibrated ecg sample
 * return the rgb value: Positive is red, negative is blue.
 */
long Strip::map(int grayScaled) {
	long val = (long)((grayScaled > 0)?grayScaled:-grayScaled);
	long color = (grayScaled > 0) ? ( val << 16 ) : val ;
	return(color);
}
/*------------------------------------------------------------*/
void Strip::push() {
	//Each LED requires 24 bits of data
	//MSB: R7, R6, R5..., G7, G6..., B7, B6... B0 
	//Once the 24 bits have been delivered, the IC immediately relays these bits to its neighbor
	//Pulling the clock low for 500us or more causes the IC to post the data.

  for(int LED_number = 0 ; LED_number < NUM_LEDS ; LED_number++) {
    long this_led_color = stripColors[LED_number]; //24 bits of color data
	
    for(byte color_bit = 23 ; color_bit != 255 ; color_bit--) {
      //Feed color bit 23 first (red data MSB)
      
      digitalWrite(CKI, LOW); //Only change data when clock is low
      
      long mask = 1L << color_bit;
      //The 1'L' forces the 1 to start as a 32 bit number, otherwise it defaults to 16-bit.
      
      if(this_led_color & mask) 
        digitalWrite(SDI, HIGH);
      else
        digitalWrite(SDI, LOW);
  
      digitalWrite(CKI, HIGH); //Data is latched when clock goes high
    }
  }

  //Pull clock low to put strip into reset/post mode
  digitalWrite(CKI, LOW);
  delayMicroseconds(500); //Wait for 500us to go into reset
}
/*------------------------------------------------------------*/
