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
task tasks[1];
const unsigned long tasksPeriodGCD = 500;
const unsigned char tasksNum = 1;
const unsigned long periodTemp = 2000;

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


