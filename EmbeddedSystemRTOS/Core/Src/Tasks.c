/*
 * Tasks.h
 *
 *  Created on: Nov 13, 2025
 *      Author: braden
 */
#include "Tasks.h"
#include "Queue.h"
#include "dht11_driver.h"
#include "ina219.h"
#include "ledstrip.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
extern UART_HandleTypeDef huart2;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim8;


// Actual definitions only here!
Q4uc sharedQueue;
task tasks[5];
const unsigned long tasksPeriodGCD = 500;
const unsigned char tasksNum = 5;
const unsigned long periodTemp = 2000;
const unsigned long periodVolts = 2000;
const unsigned long periodLEDs = 2000;
const unsigned long periodFan = 2000;


static uint16_t duty74 = (uint16_t) 48495;


INA219_t inaLEDs = {
    .i2c = &hi2c1,
    .address = INA219_ADDRESS
};

INA219_t inaFan = {
    .i2c = &hi2c2,
    .address = INA219_ADDRESS
};


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


// Task state machines

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
					//uint8_t value = (uint8_t)tempToPush;
					Q4ucPush(&sharedQueue, tempToPush);
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

int VoltageSensorLEDs(int state)
{
	char buf[50];
    switch (state)
    {
        case INIT:
        	sprintf(buf, "VoltageSensorLEDs: INIT\n\r");
        	INA219_Init(&inaLEDs);
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            state = READ;
            break;
        case READ:
        {
        	sprintf(buf, "VoltageSensorLEDs: READ\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
        	if (!Q4ucFull(sharedQueue)) {
        		float voltsToPush = INA219_ReadBusVoltage_V(&inaLEDs);
        		//uint8_t value = (uint8_t)voltsToPush;
        		Q4ucPush(&sharedQueue, voltsToPush);
        	    }
            break;
        }
        default:
            state = INIT;
            break;
    }
    return state;
}

int VoltageSensorFan(int state)
{
	char buf[50];
    switch (state)
    {
        case INIT:
        	sprintf(buf, "VoltageSensorFan: INIT\n\r");
        	INA219_Init(&inaFan);
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            state = READ;
            break;
        case READ:
        {
        	sprintf(buf, "VoltageSensor: READ\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
        	if (!Q4ucFull(sharedQueue)) {
        		float ampsToPush = INA219_ReadCurrent_mA(&inaFan);
        		Q4ucPush(&sharedQueue, ampsToPush);
        	    }
            break;
        }
        default:
            state = INIT;
            break;
    }
    return state;
}


int LightLEDs(int state)
{
	char buf[50];
    switch (state)
    {
        case INIT:
        	sprintf(buf, "LightLEDs: INIT\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            state = LIGHT;
            break;
        case LIGHT:
        {
        	sprintf(buf, "LightLEDs: LIGHT\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
        	// Hardcoded due to esp issues
        	  Set_LED(0, 0, 0, 255);
			  Set_LED(1, 0, 0, 255);
			  Set_LED(2, 0, 0, 255);
			  Set_LED(3, 0, 0, 255);
			  Set_LED(4, 0, 0, 255);
			  Set_LED(5, 0, 0, 255);
			  Set_LED(6, 0, 0, 255);
			  Set_LED(7, 0, 0, 255);
			  Set_LED(8, 0, 0, 255);
			  Set_LED(9, 0, 0, 255);
			  Set_LED(10, 0, 0, 255);
			  Set_Brightness(5);
			  WS2812_Send();
            break;
        }
        default:
            state = INIT;
            break;
    }
    return state;
}

int FanControl(int state)
{
	char buf[50];

	// Hardcoded due to esp issues
	uint8_t rx;
	rx = '1';

    switch (state)
    {
        case INIT:
        	sprintf(buf, "FanControl: INIT\n\r");
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            state = SETSTATE;
            break;
        case SETSTATE:
        {
        	sprintf(buf, "FanControl: SETSTATE\n\r");
        	//if (HAL_UART_Receive(&huart2, &rx, 1, 10) == HAL_OK)

		  //{
		  if (rx == '1')
		  {
			  // Motor ON at 74% duty
			  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, duty74);
			  //char msg[] = "Motor ON (74%)\r\n";
			  //HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, 10);
		  }
		  else if (rx == '0')
		  {
			  // Motor OFF (0% duty)
			  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 0);
			  //char msg[] = "Motor OFF\r\n";
			  //HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, 10);
		  }
		  //}
        	HAL_UART_Transmit(&huart2, (uint8_t*)buf, strlen(buf), HAL_MAX_DELAY);
            break;
        }
        default:
            state = INIT;
            break;
    }
    return state;
}

