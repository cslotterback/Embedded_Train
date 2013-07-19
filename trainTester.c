/*
 * trainTester.c
 *
 *  Created on: Mar 25, 2012
 *      Author: slotterback
 */

#include "trainTester.h"
//#include <pthread.h>
//#include <mqueue.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "vtUtilities.h"

#define trainSTACK_SIZE  	10*configMINIMAL_STACK_SIZE

void vTimerInteruptEmulater(void * sup){

	msgStruct *queuePtr = (msgStruct *) sup;
//	struct mq_attr ma;
//	mode_t mode = 0666;
//	ma.mq_flags = 0;
//	ma.mq_maxmsg = 1;
//	ma.mq_msgsize = 20;
//	ma.mq_flags = 0;
//
//	mqd_t my_queue2 = mq_open("/toTrainTester", O_CREAT | O_RDWR, mode, &ma);
//
//	while(1){
//
//		sleep(1);
//		char start[2];
//		start[0]=NULL;
//		mq_send(my_queue2, &start[0], 20, 0);
//	}

	char start[2];
	start[0]=NULL;
	while(1){
		
		printf("\npausing 1 second\n");
		vTaskDelay(3000 / portTICK_RATE_MS);
		xQueueSend(queuePtr->inQ1, start, portMAX_DELAY);
	}


	return;
}

void vTrainTesterTask(void * sup) {

	msgStruct *queuePtr = (msgStruct *) sup;

	struct trainState train1;
	train1.dir = 0;
	train1.traveled = 0;
	train1.node1 = 1;
	struct trainState train2;
	train2.dir = 0;
	train2.traveled = 0;
	struct trainState train3;
	train3.dir = 0;
	train3.traveled = 0;

	struct possiblePaths pathList[16];
	int i;
	for (i = 0; i < 16; i++) {
		pathList[i].dest2 = 0;
		pathList[i].train = 0;

	}

	pathList[1].dest1 = 11;
	pathList[1].travelTime = 2;
	pathList[1].switched = -1;
	pathList[2].dest1 = 11;
	pathList[2].travelTime = 2;
	pathList[2].switched = -1;
	pathList[3].dest1 = 11;
	pathList[3].travelTime = 2;
	pathList[3].switched = -1;
	pathList[4].dest1 = 11;
	pathList[4].travelTime = 2;
	pathList[4].switched = -1;

	pathList[5].dest1 = 12;
	pathList[5].travelTime = 2;
	pathList[5].switched = -1;
	pathList[6].dest1 = 12;
	pathList[6].travelTime = 2;
	pathList[6].switched = -1;

	pathList[7].dest1 = 15;
	pathList[7].travelTime = 4;
	pathList[7].switched = -1;

	pathList[8].dest1 = 7;
	pathList[8].travelTime = 8;
	pathList[8].switched = -1;

	pathList[9].dest1 = 10;
	pathList[9].travelTime = 7;
	pathList[9].switched = -1;

	pathList[10].dest1 = 13;
	pathList[10].travelTime = 2;
	pathList[10].switched = -1;

	pathList[11].dest1 = 14;
	pathList[11].dest2 = 16; //for 16 extra logic required
	pathList[11].travelTime = 2;
	pathList[11].switched = 1;

	pathList[12].dest1 = 13;
	pathList[12].dest2 = 16; //for 16 extra logic required
	pathList[12].travelTime = 2;
	pathList[12].switched = 0;

	pathList[13].dest1 = 7;
	pathList[13].dest2 = 16; //for 16 extra logic required
	pathList[13].travelTime = 4;
	pathList[13].switched = 0;

	pathList[14].dest1 = 9;
	pathList[14].dest2 = 11;
	pathList[14].travelTime = 2;
	pathList[14].switched = 0;

	pathList[15].dest1 = 8;
	pathList[15].dest2 = 10;
	pathList[15].travelTime = 4;
	pathList[15].switched = 1;

	pathList[2].train = 1;
	//int train1Index = 1;

//	struct mq_attr ma;
//	mode_t mode = 0666;
//	ma.mq_flags = 0;
//	ma.mq_maxmsg = 1;
//	ma.mq_msgsize = 20;
//	ma.mq_flags = 0;
//	mqd_t my_queue1 = mq_open("/toTrainThread", O_CREAT | O_RDWR, mode, &ma);
//	mqd_t my_queue2 = mq_open("/toTrainTester", O_CREAT | O_RDWR, mode, &ma);

//	xQueueHandle xToThread, xToTester;
//	if(xToThread == 0 || xToTester == 0){
//		VT_HANDLE_FATAL_ERROR(0);
//	}


	char test1[20];
//	char test[] = "testing to Thread";

	
	xQueueSend(queuePtr->inQ2, "testing to Thread", portMAX_DELAY);
	xQueueReceive(queuePtr->inQ1, &test1, portMAX_DELAY);
	printf("\n%s\n",test1);
//
//	mq_send(my_queue1,&test[0],20,0);
//	mq_receive(my_queue2,&test1[0],20,NULL);

//	pthread_t readThreads[1];
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//	pthread_create(&readThreads[0], &attr, vTimerInteruptEmulater, (void *) NULL);

	portBASE_TYPE retval;
    if ((retval = xTaskCreate(vTimerInteruptEmulater, ( signed char * ) "TRAIN_TIMER", trainSTACK_SIZE/2, (void*)queuePtr, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL )) != pdPASS) {
    	VT_HANDLE_FATAL_ERROR(retval);
    }


	while (1) {

		xQueueReceive(queuePtr->inQ1, &test1, portMAX_DELAY);
		printf("msg: %s\n", test1);
		if (test1[0] != NULL) {
			if (test1[0] == 'f') {
				if (test1[1] == '1') {
					train1.dir = 1;
				} else if (test1[1] == '2') {
					train2.dir = 1;
				} else if (test1[1] == '3') {
					train3.dir = 1;
				}
			} else if (test1[0] == 's') {
				if (test1[1] == '1') {
					train1.dir = 0;
				} else if (test1[1] == '2') {
					train2.dir = 0;
				} else if (test1[1] == '3') {
					train3.dir = 0;
				}
			} else if (test1[0] == 'r') {
				if (test1[1] == '1') {
					train1.dir = 2;
				} else if (test1[1] == '2') {
					train2.dir = 2;
				} else if (test1[1] == '3') {
					train3.dir = 2;
				}
			} else if (test1[0] == 'x') {
				if (test1[1] == '1') { //RIGHT CLUSTER
					if (test1[2] == '1') {
						pathList[11].switched = 1; //straight turn
					} else if (test1[2] == '2') {
						pathList[11].switched = 2; //straight straight
					} else if (test1[2] == '3') {
						pathList[11].switched = 3; //turn straight
					} else if (test1[2] == '4') {
						pathList[11].switched = 4; //turn turn
					}
				} else if (test1[1] == '2') { //LEFT CLUSTER
					if (test1[2] == '2') {
						pathList[12].switched = 1; //turn
						if (test1[0] != NULL) {
						} else if (test1[2] == '1') {
							pathList[12].switched = 0; //straight
						}
					}
				} else if (test1[1] == '3') { //BOTTOM
					if (test1[2] == '1') {
						pathList[13].switched = 0; //both straight
					} else if (test1[2] == '2') {
						pathList[13].switched = 1; //straight into turn
					} else if (test1[2] == '3') {
						pathList[13].switched = 2; //first turn
					}
				} else if (test1[1] == '4') { //TOP
					if (test1[2] == '1') {
						if (test1[0] != NULL) {
							pathList[15].switched = 0; //straight
						} else if (test1[2] == '2') {
							pathList[15].switched = 1; //turn
						}
					}
				}
			}

		}

		else{

			if (train1.dir != 0) {
				if (train1.traveled < pathList[train1.node1].travelTime) {
					train1.traveled++;
				} else {
					if (pathList[train1.node1].switched == -1) { //NO ASSOCIATED SWITCHES
						train1.node1 = pathList[train1.node1].dest1;
						train1.traveled = 0;
						if (train1.node1 == 1) {
							//send sensor 7
							//char test2[20] = "7";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "7", portMAX_DELAY);
						} else if (train1.node1 == 2) {
							//send sensor 8
							//char test2[20] = "8";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "8", portMAX_DELAY);
						} else if (train1.node1 == 3) {
							//send sensor 9
							//char test2[20] = "9";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "9", portMAX_DELAY);
						} else if (train1.node1 == 4) {
							//send sensor 10
							//char test2[20] = "10";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "10", portMAX_DELAY);
						} else if (train1.node1 == 5) {
							//send sensor 5
							//char test2[20] = "5";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "5", portMAX_DELAY);
						} else if (train1.node1 == 6) {
							//send sensor 6
							//char test2[20] = "6";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "6", portMAX_DELAY);
						} else if (train1.node1 == 13) {
							//send sensor 4
							//char test2[20] = "4";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "4", portMAX_DELAY);
						} else if (train1.node1 == 14) {
							//send sensor 13
							//char test2[20] = "13";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "13", portMAX_DELAY);
						} else if (train1.node1 == 7) {
							//send sensor 2
							//char test2[20] = "2";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "2", portMAX_DELAY);
						} else if (train1.node1 == 9) {
							//send sensor 12
							//char test2[20] = "12";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "12", portMAX_DELAY);
						} else if (train1.node1 == 15) {
							//send sensor 1
							//char test2[20] = "1";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "1", portMAX_DELAY);
						} else if (train1.node1 == 10) {
							//send sensor 11
							//char test2[20] = "11";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "11", portMAX_DELAY);
						} else if (train1.node1 == 8) {
							//send sensor 3
							//char test2[20] = "3";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "3", portMAX_DELAY);
						}
					} else {
						if (train1.node1 == 14) {
							if (train1.dir == 2) {
								train1.node1 = pathList[train1.node1].dest2;
								train1.traveled = 0;
							} else if (train1.dir == 1) {
								train1.node1 = pathList[train1.node1].dest1;
								train1.traveled = 0;
								//char test2[20] = "12";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "12", portMAX_DELAY);
							}
						}

						else if (train1.node1 == 13) {
							if (pathList[13].switched == 0) {
								train1.node1 = 14;
								train1.traveled = 0;
								//char test2[20] = "13";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "13", portMAX_DELAY);
							} else if (pathList[13].switched == 1) {
								train1.node1 = 7;
								train1.traveled = 0;
								//char test2[20] = "2";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "2", portMAX_DELAY);
							} else if (pathList[13].switched == 2) {
								train1.node1 = 12;
								train1.traveled = 0;
							}
						}

						else if (train1.node1 == 15) {
							if (pathList[15].switched == 0) {
								train1.node1 = pathList[train1.node1].dest1;
								train1.traveled = 0;
								//char test2[20] = "3";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "3", portMAX_DELAY);
							} else {
								train1.node1 = pathList[train1.node1].dest2;
								train1.traveled = 0;
								//char test2[20] = "11";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "11", portMAX_DELAY);
							}
						}

						else if (train1.node1 == 12 && train1.dir == 1) {
							if (pathList[12].switched == 0) {
								train1.node1 = 6;
								train1.traveled = 0;
								//char test2[20] = "6";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "6", portMAX_DELAY);
							} else {
								train1.node1 = 5;
								train1.traveled = 0;
								//char test2[20] = "5";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "5", portMAX_DELAY);
							}
						} else if (train1.node1 == 12 && train1.dir == 2) {
							train1.node1 = 13;
							train1.traveled = 0;
							//char test2[20] = "4";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "4", portMAX_DELAY);
						}

						else if (train1.node1 == 11 && train1.dir == 2) {
							if (pathList[11].switched == 1) {
								train1.node1 = 1;
								train1.traveled = 0;
								//char test2[20] = "7";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "7", portMAX_DELAY);
							} else if (pathList[11].switched == 2) {
								train1.node1 = 2;
								train1.traveled = 0;
								//char test2[20] = "8";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "8", portMAX_DELAY);
							} else if (pathList[11].switched == 3) {
								train1.node1 = 3;
								train1.traveled = 0;
								//char test2[20] = "9";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "9", portMAX_DELAY);
							} else if (pathList[11].switched == 4) {
								train1.node1 = 4;
								train1.traveled = 0;
								//char test2[20] = "10";
								//mq_send(my_queue1, &test2[0], 20, 0);
								xQueueSend(queuePtr->inQ2, "10", portMAX_DELAY);
							}
						} else if (train1.node1 == 11 && train1.dir == 1) {
							train1.node1 = 14;
							train1.traveled = 0;
							//char test2[20] = "13";
							//mq_send(my_queue1, &test2[0], 20, 0);
							xQueueSend(queuePtr->inQ2, "13", portMAX_DELAY);
						}
					}
				}
			}
		}
	}

	return;
}

void vStartTrainTester(unsigned portBASE_TYPE uxPriority, msgStruct *ptr) {

	portBASE_TYPE retval;
        if ((retval = xTaskCreate( vTrainTesterTask, ( signed char * ) "TRAIN_TESTER", trainSTACK_SIZE, (void*) ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS) {
                VT_HANDLE_FATAL_ERROR(retval);
        }

//	pthread_t readThreads[1];
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//
//	pthread_create(&readThreads[0], &attr, vTrainTesterTask, (void *) NULL);

}
