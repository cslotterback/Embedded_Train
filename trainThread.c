/*
 * trainThread.c
 *
 *  Created on: Mar 25, 2012
 *      Author: slotterback
 */

#include <stdlib.h>
//#include <pthread.h>
//#include <mqueue.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "vtUtilities.h"

#include "trainThread.h"
#include "trainTester.h"

#define trainSTACK_SIZE  	10*configMINIMAL_STACK_SIZE
#define e 					25
#define nodes				15

void pathGet(int s, int dest, int* returner);

void vTrainControlTask(void * sup) {

	msgStruct *queuePtr = (msgStruct *) sup;

	int i, j, count1, count2, size;

	int numTrains = 2;
	int train1dest;
	int train2dest;

	int train1move = 0;
	int train2move = 0;
	int train3move = 0;

	//SAY IM GIVEN THE PATH ROUTE 2 9 5 and 15 13 6
	//int userPath[numTrains + 1][nodes];
	int ** userPath;
	userPath = (int**)malloc(sizeof(int*)*(numTrains+1));
	for(i=0;i<numTrains+1;i++){
		userPath[i]=(int*)malloc(sizeof(int)*nodes);
	}


	//userPath[0]={2,9,5};
	userPath[0][0] = 2;
	userPath[0][1] = 9;
	userPath[0][2] = 5;
	train1dest = 5;
	//userPath[1]={15,13,6};
	userPath[1][0] = 4;
	userPath[1][1] = 10;
	userPath[1][2] = 7;
	train2dest = 7;

	int paths[numTrains + 1][nodes];
	int pathTemp[nodes];

	for (i = 0; i < numTrains; i++) {
		int cur = 0, next = 1;
		//int size = sizeof(userPath[i]) / sizeof(int); //NOT WORKING?

		count1 = 0;
		for (j = 0; j < 2; j++) {
			count2 = 0;
			pathGet(userPath[i][cur], userPath[i][next], pathTemp);

			while (pathTemp[count2] != NULL) {
				paths[i][count1] = pathTemp[count2];
				count2++;
				count1++;
			}

			cur++;
			next++;
		}
		paths[i][count1] = userPath[cur];
		if(i == 0){
			paths[i][count1++] = train1dest;
		}else if(i == 1){
					paths[i][count1++] = train2dest;
		}
	}


	for (i = 0; i < nodes; i++) {
		if (paths[0][i] > 0 && paths[0][i] < 16) {
			printf("%d ", paths[0][i]);
		} else
			paths[0][i] = NULL;
	}
	printf("\n");
	for (i = 0; i < nodes; i++) {
		if (paths[1][i] > 0 && paths[1][i] < 16) {
			printf("%d ", paths[1][i]);
		} else
			paths[1][i] = NULL;
	}
	printf("\n");

	

//	struct mq_attr ma;
//	mode_t mode = 0666;
//	ma.mq_flags = 0;
//	ma.mq_maxmsg = 1;
//	ma.mq_msgsize = 20;
//	ma.mq_flags = 0;
//	mqd_t my_queue1 = mq_open("/toTrainThread", O_CREAT | O_RDWR, mode, &ma);
//	ma.mq_flags = 1;
//	mqd_t my_queue2 = mq_open("/toTrainTester", O_CREAT | O_RDWR, mode, &ma);


//	xQueueHandle queuePtr->inQ2, queuePtr->inQ1;
//	queuePtr->inQ2 = xQueueCreate(1,20);
//	queuePtr->inQ1 = xQueueCreate(1,20);
//	if(queuePtr->inQ2 == 0 || queuePtr->inQ1 == 0){
//		VT_HANDLE_FATAL_ERROR(0);
//	}
//
//	static msgStruct msgQueues;
//	msgQueues.inQ1 = queuePtr->inQ2;
//	msgQueues.inQ2 = queuePtr->inQ1;

//	static msgStruct1 msgQueues1;
//	msgQueues1.inQ1 = xToThread;
//	msgQueues1.inQ2 = xToTester;

	//vStartTrainTester(tskIDLE_PRIORITY, queuePtr);

	char test1[20];
//	char test[] = "testing to Tester";
//
//	mq_receive(my_queue1,&test1[0],20,NULL);
//	mq_send(my_queue2,&test[0],20,0);
//
//	printf("\n%s\n",test1);

	xQueueReceive(queuePtr->inQ2, &test1, portMAX_DELAY);
	xQueueSend(queuePtr->inQ1, "testing to i2c", portMAX_DELAY);
	printf("\n%s\n",test1);

	int train1Index = 0;
	int train2Index = 0;
	int train3Index = 0;

	train1move = 1;

	//char start[20] = "f1";
	//mq_send(my_queue2, &start[0], 20, 0);
	xQueueSend(queuePtr->inQ1, "f1", portMAX_DELAY);
	char sensor[20];
	while (1) {
		printf("train1: index: %d, which is at: %d\n",train1Index,paths[0][train1Index]);

		//AUSTINS CODE HERE:
		//****************
		//****************
		//****************

//		xQueueSend(queuePtr->inQ3, "testing to LCD", portMAX_DELAY);
//		sprintf(test1,"%d,%d,%d",paths[0][train1Index],paths[0][train1Index+1],paths[0][train1Index+2]);
//		xQueueSend(queuePtr->inQ3, test1 , portMAX_DELAY);

		//****************
		//****************
		//****************
		//****************



		//printf("train2: index: %d, which is at: %d\n",train2Index,paths[1][train2Index]);
		int sensorNum;
		xQueueReceive(queuePtr->inQ2, &sensor, portMAX_DELAY);
		//mq_receive(my_queue1, &sensor[0], 20, NULL);
		printf("sensor: %s\n", sensor);
		sensorNum = atoi(sensor);

		//UPDATE TRAIN INDECIES

		if (sensorNum == 13 && paths[0][train1Index + 1] == 14) {
			train1Index++;
		}

		if (sensorNum == 13 && paths[0][train1Index + 2] == 14) {
			train1Index=train1Index+2;
		}

		if (sensorNum == 12 && paths[0][train1Index + 1] == 9) {
			train1Index++;
		}

		if (sensorNum == 12 && paths[0][train1Index + 1] == 9) {
			train1Index++;
		}

		if (sensorNum == 11 && paths[0][train1Index + 1] == 10) {
			train1Index++;
		}

		if (sensorNum == 4 && paths[0][train1Index + 1] == 13) {
			train1Index++;
		}

		if (sensorNum == 2 && paths[0][train1Index + 1] == 7) {
			train1Index++;
		}

		if (sensorNum == 1 && paths[0][train1Index + 1] == 15) {
			train1Index++;
		}

		if (sensorNum == 3 && paths[0][train1Index + 1] == 8) {
			train1Index++;
		}

		if (sensorNum == 6 && paths[0][train1Index + 1] == 6) {
			train1Index++;
		}

		if (sensorNum == 5 && paths[0][train1Index + 2] == 5) {
			train1Index = train1Index + 2;
		}

		if (sensorNum == 6 && paths[0][train1Index + 2] == 6) {
			train1Index = train1Index + 2;
		}

		if (sensorNum == 7 && paths[0][train1Index + 2] == 1) {
			train1Index = train1Index + 2;
		}

		if (sensorNum == 8 && paths[0][train1Index + 2] == 2) {
			train1Index = train1Index + 2;
		}

		if (sensorNum == 9 && paths[0][train1Index + 2] == 3) {
			train1Index = train1Index + 2;
		}

		if (sensorNum == 10 && paths[0][train1Index + 2] == 4) {
			train1Index = train1Index + 2;
		}

		//TRAIN 2 UPDATING !!!
		//!!!
		//!!!

		//
		if(train2move == 1){

		if (sensorNum == 13 && paths[1][train2Index + 1] == 14) {
			train2Index++;
		}

		if (sensorNum == 13 && paths[1][train2Index + 2] == 14) {
			train2Index=train2Index+2;
		}

		if (sensorNum == 12 && paths[1][train2Index + 1] == 9) {
			train2Index++;
		}

		if (sensorNum == 12 && paths[1][train2Index + 1] == 9) {
			train2Index++;
		}

		if (sensorNum == 11 && paths[1][train2Index + 1] == 10) {
			train2Index++;
		}

		if (sensorNum == 4 && paths[1][train2Index + 1] == 13) {
			train2Index++;
		}

		if (sensorNum == 2 && paths[1][train2Index + 1] == 7) {
			train2Index++;
		}

		if (sensorNum == 1 && paths[1][train2Index + 1] == 15) {
			train2Index++;
		}

		if (sensorNum == 3 && paths[1][train2Index + 1] == 8) {
			train2Index++;
		}

		if (sensorNum == 6 && paths[1][train2Index + 1] == 6) {
			train2Index++;
		}

		if (sensorNum == 5 && paths[1][train2Index + 2] == 5) {
			train2Index = train2Index + 2;
		}

		if (sensorNum == 6 && paths[1][train2Index + 2] == 6) {
			train2Index = train2Index + 2;
		}

		if (sensorNum == 7 && paths[1][train2Index + 2] == 1) {
			train2Index = train2Index + 2;
		}

		if (sensorNum == 8 && paths[1][train2Index + 2] == 2) {
			train2Index = train2Index + 2;
		}

		if (sensorNum == 9 && paths[1][train2Index + 2] == 3) {
			train2Index = train2Index + 2;
		}

		if (sensorNum == 10 && paths[1][train2Index + 2] == 4) {
			train2Index = train2Index + 2;
		}


		}
		//

		//TRAIN INDECIES UPDATED, LOGIC TO CONTROL TRAINS

		if (paths[0][train1Index] == train1dest) {
			//char stop1[3] = "s1";
			//mq_send(my_queue2, &stop1[0], 20, 0);
			xQueueSend(queuePtr->inQ1, "s1", portMAX_DELAY);
		}

		if (paths[1][train2Index] == train2dest) {
			//char stop1[3] = "s2";
			//mq_send(my_queue2, &stop1[0], 20, 0);
			xQueueSend(queuePtr->inQ1, "s2", portMAX_DELAY);
		}

		//:TRAIN COLLISONS

		//COLLISION FOR TRAIN 1 AND 2
		if (paths[0][train1Index + 1] == paths[1][train2Index + 1]
				|| paths[0][train1Index + 1] == paths[1][train2Index + 2]) {

			if (train1move == 1 && train2move == 1) {
				//stop train2
				//char stop1[3] = "s2";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s2", portMAX_DELAY);
			}

		} else if (paths[0][train1Index + 2] == paths[1][train2Index + 1]
				|| paths[0][train1Index + 2] == paths[1][train2Index + 2]) {

			if (train1move == 1 && train2move == 1) {
				//stop train2
				//char stop1[3] = "s2";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s2", portMAX_DELAY);
			}
		}

		//COLLISION FOR TRAIN 1 AND 3
		if (paths[0][train1Index + 1] == paths[2][train3Index + 1]
				|| paths[0][train1Index + 1] == paths[2][train3Index + 2]) {

			if (train1move == 1 && train3move == 1) {
				//stop train3
				//char stop1[3] = "s3";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s3", portMAX_DELAY);
			}
		} else if (paths[0][train1Index + 2] == paths[2][train3Index + 1]
				|| paths[0][train1Index + 2] == paths[2][train3Index + 2]) {

			if (train1move == 1 && train3move == 1) {
				//char stop1[3] = "s3";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s3", portMAX_DELAY);
			}
		}

		//COLLISION FOR TRAIN 2 AND 3
		if (paths[1][train2Index + 1] == paths[2][train3Index + 1]
				|| paths[1][train2Index + 1] == paths[2][train3Index + 2]) {

			if (train2move == 1 && train3move == 1) {
				//char stop1[3] = "s3";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s3", portMAX_DELAY);
			}
		} else if (paths[1][train2Index + 2] == paths[2][train3Index + 1]
				|| paths[1][train2Index + 2] == paths[2][train3Index + 2]) {

			if (train2move == 1 && train3move == 1) {
				//char stop1[3] = "s3";
				//mq_send(my_queue2, &stop1[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "s3", portMAX_DELAY);
			}
		}

//		if(train2move == 0 && paths[1][train2Index] != train2dest){
//			if(paths[0][train1Index + 1] != paths[1][train2Index + 1]  &&
//					paths[0][train1Index + 1] != paths[1][train2Index + 2] //&&
////					paths[0][train1Index + 2] != paths[1][train2Index + 1] &&
////					paths[0][train1Index + 2] != paths[1][train2Index + 2] //&&
////					paths[3][train3Index + 1] != paths[2][train2Index + 1]  &&
////					paths[3][train3Index + 1] != paths[2][train2Index + 2] &&
////					paths[3][train3Index + 2] != paths[2][train2Index + 1] &&
////					paths[3][train3Index + 2] != paths[2][train2Index + 2]
//					)
//			{
//
//					xQueueSend(queuePtr->inQ1, "f2", portMAX_DELAY);
//
//			}
//		}

		//REDO THIS FOR TRAIN 3

		if (paths[0][train1Index] == 14 && paths[0][train1Index + 1] == 11) {
			if (train1move == 1) {
				//reverse
				//char start[3] = "r1";
				//mq_send(my_queue2, &start[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "r1", portMAX_DELAY);
			}
		} else if ((paths[0][train1Index] == 6
				&& paths[0][train1Index + 1] == 12)
				|| (paths[0][train1Index] == 5
						&& paths[0][train1Index + 1] == 12)) {
			if (train1move == 1) {
				//reverse
				//char start[3] = "r1";
				//mq_send(my_queue2, &start[0], 20, 0);
				xQueueSend(queuePtr->inQ1, "r1", portMAX_DELAY);
			}
		} else if (paths[0][train1Index] == 13 && train1move == 1) {
			//forward
			//char start[3] = "f1";
			//mq_send(my_queue2, &start[0], 20, 0);
			xQueueSend(queuePtr->inQ1, "f1", portMAX_DELAY);
		}

		//LOGIC TO CONTROL SWITCHES

		if (train1move == 1	) {
				

			if (paths[0][train1Index + 1] == 13) {
				if (paths[0][train1Index + 2] == 14) {
					//send x31
					//char turn1[4] = "x31";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x31", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 7) {
					//send x32
					//char turn1[4] = "x32";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x32", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 12) {
					//send x33
					//char turn1[4] = "x33";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x33", portMAX_DELAY);
				}
			}

			if (paths[0][train1Index + 1] == 12) {
				if (paths[0][train1Index + 2] == 5) {
					//send x22
					//char turn1[4] = "x22";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x22", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 6) {
					//send x21
					//char turn1[4] = "x21";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x21", portMAX_DELAY);
				}
			}

			if (paths[0][train1Index + 1] == 11) {
				if (paths[0][train1Index + 2] == 1) {
					//send x11
					//char turn1[4] = "x11";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x11", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 2) {
					//send x12
					//char turn1[4] = "x12";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x12", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 3) {
					//send x13
					//char turn1[4] = "x13";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x13", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 4) {
					//send x14
					//char turn1[4] = "x14";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x14", portMAX_DELAY);
				}
			}

			if (paths[0][train1Index + 1] == 15) {
				if (paths[0][train1Index + 2] == 8) {
					//send x41
					//char turn1[4] = "x41";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x41", portMAX_DELAY);
				} else if (paths[0][train1Index + 2] == 10) {
					//send x42
					//char turn1[4] = "x42";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x42", portMAX_DELAY);
				}
			}

		}

		//SWITCHES FOR TRAIN 2!!! testing

		if (train1move == 2	) {
				

			if (paths[1][train2Index + 1] == 13) {
				if (paths[1][train2Index + 2] == 14) {
					//send x31
					//char turn1[4] = "x31";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x31", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 7) {
					//send x32
					//char turn1[4] = "x32";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x32", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 12) {
					//send x33
					//char turn1[4] = "x33";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x33", portMAX_DELAY);
				}
			}

			if (paths[1][train1Index + 1] == 12) {
				if (paths[1][train2Index + 2] == 5) {
					//send x22
					//char turn1[4] = "x22";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x22", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 6) {
					//send x21
					//char turn1[4] = "x21";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x21", portMAX_DELAY);
				}
			}

			if (paths[1][train1Index + 1] == 11) {
				if (paths[1][train2Index + 2] == 1) {
					//send x11
					//char turn1[4] = "x11";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x11", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 2) {
					//send x12
					//char turn1[4] = "x12";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x12", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 3) {
					//send x13
					//char turn1[4] = "x13";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x13", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 4) {
					//send x14
					//char turn1[4] = "x14";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x14", portMAX_DELAY);
				}
			}

			if (paths[1][train2Index + 1] == 15) {
				if (paths[1][train2Index + 2] == 8) {
					//send x41
					//char turn1[4] = "x41";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x41", portMAX_DELAY);
				} else if (paths[1][train2Index + 2] == 10) {
					//send x42
					//char turn1[4] = "x42";
					//mq_send(my_queue2, &turn1[0], 20, 0);
					xQueueSend(queuePtr->inQ1, "x42", portMAX_DELAY);
				}
			}

		}


	}
}

void vStartTrainThread(unsigned portBASE_TYPE uxPriority, msgStruct *ptr) {

	/* Start the task */
        portBASE_TYPE retval;
        if ((retval = xTaskCreate( vTrainControlTask, ( signed char * ) "TRAIN_CNTRL", trainSTACK_SIZE, (void*) ptr, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS) {
                VT_HANDLE_FATAL_ERROR(retval);
        }
//	pthread_t readThreads[1];
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//
//	pthread_create(&readThreads[0], &attr, vTrainControlTask, (void *) NULL);

}

void pathGet(int s, int dest, int *returner) {

	int i, j, mini, k;
	int n = 15, inf = e * e;

	//int dist[e][e];
	//int visited[e];
	//int prev[e];
	//int d[e];

	int** dist;
	//dist =(int**)malloc(sizeof(int*)*e);
	VTmalloc(dist,sizeof(int *)*e);
	for(i=0;i<e;i++){
		//dist[i]=(int*)malloc(sizeof(int)*e);
		VTmalloc(dist[i],sizeof(int)*e);
	}

	int* visited = (int*)malloc(sizeof(int)*e);
	int* prev = (int*)malloc(sizeof(int)*e);
	//int* d = (int*)malloc(sizeof(int)*e);
	int *d;
	VTmalloc(d,sizeof(int)*e);

//        int* visited;
//        int* prev;
//        int* d;
//        visited = (int*)malloc(e*sizeof(int));
//        prev = (int*)malloc(e*sizeof(int));
//        d = (int*)malloc(e*sizeof(int));
//
//        if(visited == 0 || prev == 0 || d == 0){
//
//        	printf("ERROR: OUT OF MEMORY");
//        	return;
//        }

	for (i = 0; i < e; ++i) {
		for (j = 0; j < e; ++j) {
			dist[i][j] = 0;
		}
	}

	//hard code time!
	dist[1][11] = 1;
	dist[2][11] = 1;
	dist[3][11] = 1;
	dist[4][11] = 1;
	dist[11][14] = 1;
	dist[14][9] = 1;
	dist[9][10] = 1;
	dist[10][13] = 1;
	dist[13][12] = 1;
	dist[12][6] = 1;
	dist[12][5] = 1;
	dist[5][12] = 1;
	dist[6][12] = 1;
	dist[12][13] = 1;
	dist[13][7] = 1;
	dist[7][15] = 1;
	dist[15][10] = 1;
	dist[15][8] = 1;
	dist[8][7] = 1;
	dist[13][14] = 1;
	dist[14][11] = 1;
	dist[11][1] = 1;
	dist[11][2] = 1;
	dist[11][3] = 1;
	dist[11][4] = 1;

	for (i = 1; i <= n; ++i) {
		d[i] = inf;
		prev[i] = -1;
		visited[i] = 0;

	}
	d[s] = 0;
	for (k = 1; k <= n; ++k) {
		mini = -1;
		for (i = 1; i <= n; ++i) {
			if (!visited[i] && ((mini == -1) || (d[i] < d[mini]))) {
				mini = i;
			}
		}

		visited[mini] = 1;

		for (i = 1; i <= n; ++i) {
			if (dist[mini][i]) {
				if (d[mini] + dist[mini][i] < d[i]) {
					d[i] = d[mini] + dist[mini][i];
					prev[i] = mini;
				}
			}
		}
	}

	int tempDest = dest;
	i = 0;
	while (prev[tempDest] != -1) {
		tempDest = prev[tempDest];
		returner[i] = tempDest;
		i++;
	}
	returner[i] = s;

	int swap, a, b = i;
	for (a = 0; a < --b; a++) {
		swap = returner[a];
		returner[a] = returner[b];
		returner[b] = swap;
	}

	for (j = 0; j < i + 1; j++) {
		printf("%d ", returner[j]);
	}
	returner[j - 1] = NULL;
	for (; j < sizeof(returner) / sizeof(int); j++) {
		returner[j] = NULL;
	}
	//printf("%d ",dest);
	printf("\n\n\n");

//        free(visited);
//        free(prev);
//        free(d);
//        for(i=0;i<e;i++){
//        	free(dist[i]);
//        }
//        free(dist);

	for (i = 0; i < e; i++) {
		free(dist[i]);
	}
	free(dist);
	free(visited);
	free(prev);
	free(d);

	return;
}
