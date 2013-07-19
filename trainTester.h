/*
 * trainTester.h
 *
 *  Created on: Mar 25, 2012
 *      Author: slotterback
 */

#ifndef TRAINTESTER_H_
#define TRAINTESTER_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

struct possiblePaths{
  int dest1;
	int dest2;
	int switched; //assumed to be straight at beginning
	int train;
	int travelTime;
};

struct trainState{
	int node1;
	int node2;
	int dir;
	int traveled;
};

typedef struct __msgStruct {
	xQueueHandle inQ1;
	xQueueHandle inQ2;					   	// Queue used to send messages from other tasks to the LCD task to print
	xQueueHandle inQ3;
} msgStruct;

void vStartTrainTester(unsigned portBASE_TYPE uxPriority, msgStruct *);


#endif /* TRAINTESTER_H_ */	
