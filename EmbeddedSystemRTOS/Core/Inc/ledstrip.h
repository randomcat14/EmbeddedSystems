#ifndef __WS2812_H
#define __WS2812_H

#include "main.h"
#include "math.h"
//#include "tim.h"

#define MAX_LED         8
#define MAX_BRIGHTNESS  1         // 1 = enable brightness correction
#define PI              3.14159265


// External variables

extern uint8_t LED_Data[MAX_LED][4];
extern uint8_t LED_Mod[MAX_LED][4];
extern uint16_t pwmData[(24 * MAX_LED) + 50];
extern uint8_t datasentflag;

extern TIM_HandleTypeDef htim1;

// Function Prototypes

void Set_LED (int LEDnum, int Red, int Green, int Blue);
void Set_Brightness (int brightness);     // 0–45
void WS2812_Send (void);

// HAL callback used for DMA PWM WS2812 timing
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim1);

#endif
