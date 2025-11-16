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
int TempSensor(int state); // temperature task
int test1(int state); // change to actual second task

//struct for tasks
typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct) (int);
} task;


// Add the states for your state machines here so main can see it
enum TempStates { INIT, READ };
enum TestStates {INIT1, PRINT}; // for the test task replace with task 2


//global variables
extern Q4uc sharedQueue;
extern task tasks[2]; // increase the number of tasks with each task
extern const unsigned long tasksPeriodGCD;
extern const unsigned char tasksNum;
extern const unsigned long periodTemp;
extern const unsigned long periodTest; // change to actual second task

#endif /* TASKS_H_ */
