/*
 * trainThread.h
 *
 *  Created on: Mar 25, 2012
 *      Author: slotterback
 */

#ifndef TRAINTHREAD_H_
#define TRAINTHREAD_H_

#include "queue.h"
#include "trainTester.h"


//struct msgStruct {
//  xQueueHandle inQ1;
//	xQueueHandle inQ2;					   	// Queue used to send messages from other tasks to the LCD task to print
//};

void vStartTrainThread(unsigned portBASE_TYPE uxPriority, msgStruct *);




#endif /* TRAINTHREAD_H_ */
