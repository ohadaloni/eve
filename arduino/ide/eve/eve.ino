#include "kernel.h"
#include "blink.h"
#include "strip.h"
#include "ecgData.h"
/*------------------------------------------------------------*/
Kernel kernel = Kernel();
/*------------------------------------------------------------*/
Blink blink9 = Blink(9, 100, &kernel);
Blink blink10 = Blink(10, 200, &kernel);
Blink blink11 = Blink(11, 500, &kernel);
Blink blink12 = Blink(12, 1000, &kernel);
/*------------------------------*/
void loop9() { blink9.loop(); }
void loop10() { blink10.loop(); }
void loop11() { blink11.loop(); }
void loop12() { blink12.loop(); }
/*------------------------------*/
void blinksSetup() {
	kernel.addThread(loop9);
	kernel.addThread(loop10);
	kernel.addThread(loop11);
	kernel.addThread(loop12);
}
/*------------------------------------------------------------*/
int leadFromString(char *leadName) {
	int leadNumbers[] = {
		 LEAD_1, LEAD_2, LEAD_3,
		 aVR, aVL, aVF,
		 V1, V2, V3, V4, V5, V6,
	};
	char *leadNames[] = {
		 "I", "II", "III",
		 "AVR", "AVL", "AVF",
		 "V1", "V2", "V3", "V4", "V5", "V6",
	};
	for(int i=0;i<12;i++)
		if ( strcmp(leadName, leadNames[i]) == 0 )
			return(leadNumbers[i]);
	Serial.print("leadFromString: No lead for: ");
	Serial.println(leadName);
}
/*------------------------------------------------------------*/
int explode(char a[][8], char *line) {
	int numWords = 1;
	int j=0;
	for(int i=0;line[i] != NULL;i++) {
		if ( line[i] != ',' ) {
			a[numWords-1][j] = line[i];
			j++;
			continue;
		}
		a[numWords-1][j] = NULL;
		numWords++;
		j = 0;
	}
	a[numWords-1][j] = NULL;
	return(numWords);
}
/*------------------------------------------------------------*/
int explodeHeader(int *ai, char *line) {
	char exploded[12][8];
	int num = explode(exploded, line);
	Serial.print("explodeHeader: ");
	Serial.println(line);
	for(int i=0;i<num;i++) {
		ai[i] = leadFromString(exploded[i]);
		Serial.println(exploded[i]);
		Serial.println(ai[i]);
	}
	return(num);
}
/*------------------------------------------------------------*/
void explodeData(int *ai, char *line) {
	char exploded[12][8];
	int num = explode(exploded, line);
	Serial.print("explodeData: ");
	Serial.println(line);
	for(int i=0;i<num;i++) {
		ai[i] = atoi(exploded[i]);
		Serial.println(exploded[i]);
		Serial.println(ai[i]);
	}
}
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
Strip strip = Strip(&kernel);
/*------------------------------*/
void ecgLoop() {
	int n = sizeof(ecgData)/sizeof(char *);
	Serial.print("Size of ecgData: ");
	Serial.println(n);
	int explodedData[12];
	static int i = 0;
	explodeData(explodedData, ecgData[i++%n]);
	strip.data(explodedData);
	kernel.msleep(500);
}
/*------------------------------*/
void ecgSetup() {
	/*	randomSeed(analogRead(0));	*/
	int explodedHeader[12];

	strip.setup();
	strip.off();
	kernel.msleep(1000);
	int numLeads = explodeHeader(explodedHeader, ecgHeader);
	strip.header(explodedHeader, numLeads);
	kernel.addThread(ecgLoop);
}
/*------------------------------------------------------------*/
/*------------------------------------------------------------*/
void setup() {
	kernel.setup();
	blinksSetup();
	ecgSetup();
}
/*------------------------------*/
void loop() {
	kernel.loop();
}
/*------------------------------------------------------------*/
