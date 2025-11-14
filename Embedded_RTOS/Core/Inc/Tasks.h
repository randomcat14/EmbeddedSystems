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

//struct for tasks
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct) (int);
} task;


// Add the states for your state machines here so main can see it
enum TempStates { INIT, READ };


//global variables
extern Q4uc sharedQueue;
extern task tasks[1]; // increase the number of tasks with each task
extern const unsigned long tasksPeriodGCD;
extern const unsigned char tasksNum;
extern const unsigned long periodTemp;

#endif /* TASKS_H_ */
