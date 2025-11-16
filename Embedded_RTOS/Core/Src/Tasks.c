/*
 * Tasks.h
 *
 *  Created on: Nov 13, 2025
 *      Author: braden
 */
#include "Tasks.h"
#include "Queue.h"
#include "dht11_driver.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
extern UART_HandleTypeDef huart2;


// Actual definitions only here!
Q4uc sharedQueue;
task tasks[2];
const unsigned long tasksPeriodGCD = 500; // we will change timer 7 and this to divide evenly the periods
const unsigned char tasksNum = 2; // change to number of tasks until it gets 4.
const unsigned long periodTemp = 2000; // temp period
const unsigned long periodTest = 500; // change to task 2 period you want must not be lower than GCD


//timer ISR that is called in the main uses timer isr as global interrupt.
void TimerISR() {
    unsigned char i;
    for (i = 0; i < tasksNum; ++i) {
        if (tasks[i].elapsedTime >= tasks[i].period) {
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0;
        }
        tasks[i].elapsedTime += tasksPeriodGCD;
    }
}

// this is the temperature sensor state machine
int TempSensor(int state)
{
	char buf[50];
    switch (state)
    {
        case INIT:
        	sprintf(buf, "TempSensor: INIT\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            state = READ;
            break;
        case READ:
        {
        	sprintf(buf, "TempSensor: READ\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            dht11_data data = dht11_data_read();   // read sensor
            if (!data.error) {
            	if (!Q4ucFull(sharedQueue)) {
					float tempToPush = (data.temperature * 9) / 5 + 32;
					uint8_t value = (uint8_t)tempToPush;
					Q4ucPush(&sharedQueue, value);
            	}
            }
            break;
        }
        default:
            state = INIT;
            break;
    }
    return state;
}

//task 1 will be fan with the voltage sensor and temp input from queue this is just a test
int test1(int state)
{
	char buf[50];
	switch(state)
	{
	case INIT1:
		sprintf(buf, "Testing: INIT\n\r");
		HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
		state = PRINT;
		break;
	case PRINT:
		sprintf(buf, "Testing: READ\n\r");
		HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
		break;
	}
	return state;
}




