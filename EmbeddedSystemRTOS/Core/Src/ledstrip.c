#include <stdint.h>
#include <math.h>

#include "main.h"
#include "stm32f4xx_hal.h"
#include "ledstrip.h"
#include "stdio.h"


// Global arrays

uint8_t  LED_Data[MAX_LED][4];
uint8_t  LED_Mod[MAX_LED][4];
uint16_t pwmData[(24 * MAX_LED) + 50];
uint8_t  datasentflag = 0;

//TIM_HandleTypeDef htim1;


// Functions

void Set_LED(int LEDnum, int Red, int Green, int Blue)
{
    LED_Data[LEDnum][0] = LEDnum;
    LED_Data[LEDnum][1] = Green;
    LED_Data[LEDnum][2] = Red;
    LED_Data[LEDnum][3] = Blue;
}

void Set_Brightness(int brightness)
{
	#if USE_BRIGHTNESS
		if (brightness > 45) brightness = 45;

		for (int i = 0; i < MAX_LED; i++)
		{
			LED_Mod[i][0] = LED_Data[i][0];

			for (int j = 1; j < 4; j++)
			{
				float angle = 90 - brightness;
				angle = angle * PI / 180.0;

				LED_Mod[i][j] = LED_Data[i][j] / tan(angle);
			}
		}
	#endif
}

void WS2812_Send(void)
{
    uint32_t indx = 0;
    uint32_t color;

    // Create data stream to output to led strip logic
    for (int i = 0; i < MAX_LED; i++)
    {
	#if USE_BRIGHTNESS
			color = ((LED_Mod[i][1] << 16) |
					 (LED_Mod[i][2] << 8) |
					 (LED_Mod[i][3]));
	#else
			color = ((LED_Data[i][1] << 16) |
					 (LED_Data[i][2] << 8) |
					 (LED_Data[i][3]));
	#endif

        for (int bit = 23; bit >= 0; bit--)
        {
            if (color & (1 << bit))
                pwmData[indx] = 60;  // WS2812 logic 1
            else
                pwmData[indx] = 30;  // WS2812 logic 0

            indx++;
        }
    }

    // Reset signal
    for (int i = 0; i < 50; i++)
        pwmData[indx++] = 0;

    datasentflag = 0;

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);

    while (!DMA2_Stream1_IRQn){};
    datasentflag = 0;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
    HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
    datasentflag = 1;
}
