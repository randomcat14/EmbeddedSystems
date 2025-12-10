/*
 * Tasks.h
 *
 *  Created on: Nov 13, 2025
 *      Author: braden
 */

#ifndef TASKS_H_
#define TASKS_H_

#include "Queue.h"

// ISR and tasks
void TimerISR();
int TempSensor(int state);
int VoltageSensorLEDs(int state);
int VoltageSensorFan(int state);
int LightLEDs(int state);
int FanControl(int state);

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct) (int);
} task;

enum TempStates { INIT, READ, LIGHT, SETSTATE };



//global variables
extern Q4uc sharedQueue;
extern task tasks[5];
extern const unsigned long tasksPeriodGCD;
extern const unsigned char tasksNum;
extern const unsigned long periodTemp;
extern const unsigned long periodVolts;
extern const unsigned long periodLEDs;
extern const unsigned long periodFan;

#endif /* TASKS_H_ */
