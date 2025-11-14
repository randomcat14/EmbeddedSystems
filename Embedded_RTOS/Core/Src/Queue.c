/*
 * Queue.c
 *
 *  Created on: Nov 13, 2025
 *      Author: braden
 */

#include "Queue.h"
#include "stm32f4xx.h"
#include "main.h"
#include <string.h>
#include "stdio.h"

extern UART_HandleTypeDef huart2;


void Q4ucInit(Q4uc *Q) {
   (*Q).cnt=0;
}

unsigned char Q4ucFull(Q4uc Q) {
   return (Q.cnt == 4);
}

unsigned char Q4ucEmpty(Q4uc Q) {
   return (Q.cnt == 0);
}

void Q4ucPrint(Q4uc Q) {
	char buffer[50];
   int j;
   sprintf(buffer, "Q4uc contents:\r\n");
   HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

   for (j=0; j<4; j++) {
	   sprintf(buffer, "Item %d: %d\r\n", j, Q.buf[j]);
	   HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
   }
}

void Q4ucPush(Q4uc *Q,
                  unsigned char item) {
   if (!Q4ucFull(*Q)) {
	  __disable_irq();
      (*Q).buf[(*Q).cnt] = item;
      (*Q).cnt++;
      __enable_irq();
   }
}

unsigned char Q4ucPop(Q4uc *Q)
{
   int i;
   unsigned char item=0;
   if (!Q4ucEmpty(*Q)) {
	   __disable_irq();
      item = (*Q).buf[0];
      (*Q).cnt--;
      for (i=0; i<(*Q).cnt; i++) {
         // shift fwd
         (*Q).buf[i]=
         (*Q).buf[i+1];
      }
      __enable_irq();
   }
   return(item);
}





