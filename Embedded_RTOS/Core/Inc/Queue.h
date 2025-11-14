/*
 * Queue.h
 *
 *  Created on: Nov 13, 2025
 *      Author: braden
 */

#ifndef QUEUE_H_
#define QUEUE_H_

typedef struct _Q4uc {
   unsigned char buf[4];
   unsigned char cnt;
} Q4uc;

void Q4ucInit(Q4uc *Q);
unsigned char Q4ucFull(Q4uc Q);
unsigned char Q4ucEmpty(Q4uc Q);
void Q4ucPrint(Q4uc Q);
void Q4ucPush(Q4uc *Q, unsigned char item);
unsigned char Q4ucPop(Q4uc *Q);



#endif /* QUEUE_H_ */
