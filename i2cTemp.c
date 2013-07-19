//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#include <string.h>
//
//
///* Scheduler include files. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "projdefs.h"
//#include "semphr.h"
//
///* include files. */
//#include "vtUtilities.h"
//#include "vtI2C.h"
//#include "LCDtask.h"
//#include "i2cTemp.h"
//
//
//
//// I have set this to a large stack size because of (a) using printf() and (b) the depth of function calls
////   for some of the LCD operations
//#if PRINTF_VERSION==1
//#define i2cSTACK_SIZE  	8*configMINIMAL_STACK_SIZE
//#else
//#define i2cSTACK_SIZE		4*configMINIMAL_STACK_SIZE
//#endif
//
//// Set the task up to run every second
//#define i2cREAD_RATE_BASE	( ( portTickType ) 10 )
//
///* The i2cTemp task. */
//static portTASK_FUNCTION_PROTO( vi2cTempUpdateTask, pvParameters );
//
///*-----------------------------------------------------------*/
//
//void vStarti2cTempTask( unsigned portBASE_TYPE uxPriority, i2cTempStruct *params)
//{
//	/* Start the task */
//	portBASE_TYPE retval;
//
//	if ((retval = xTaskCreate( vi2cTempUpdateTask, ( signed char * ) "i2cTemp", i2cSTACK_SIZE, (void *) params, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS) {
//		VT_HANDLE_FATAL_ERROR(retval);
//	}
//}
//
//// This is the actual task that is run
//static portTASK_FUNCTION( vi2cTempUpdateTask, pvParameters )
//{
//	portTickType xUpdateRate, xLastUpdateTime;
//	const uint8_t i2cCmdInit[]= {0xAC,0x00};
//	const uint8_t i2cCmdStartConvert[]= {0xEE};
//	const uint8_t i2cCmdStopConvert[]= {0x22};
//	const uint8_t i2cCmdReadVals[]= {0xAA};
//	const uint8_t i2cCmdReadCnt[]= {0xc4};
//	const uint8_t i2cCmdReadSlope[]= {0xc3};
//	float temperature;
//	float countPerC, countRemain;
//	uint8_t temp1, rxLen, status,temp2;
//	// Get the parameters
//	i2cTempStruct *param = (i2cTempStruct *) pvParameters;
//	// Get the I2C device pointer
//	vtI2CStruct *devPtr = param->dev;
//	// Get the LCD information pointer
//	vtLCDStruct *lcdData = param->lcdData;
//	vtLCDMsg lcdBuffer;
//
//	float x=0;
//	float y=0;
//
//	// Assumes that the I2C device (and thread) have already been initialized
//	// 0x4F address of the temperature sensor (DS1621)
//	// Should do something more comprehensive with the msgType for debugging...
//
//	// Temperature sensor configuration sequence (DS1621)
//	if (vtI2CEnQ(devPtr,0x01,0x4F,sizeof(i2cCmdInit),i2cCmdInit,0) != pdTRUE) {
//		VT_HANDLE_FATAL_ERROR(0);
//	}
//	// wait on the result
//	if (vtI2CDeQ(devPtr,0,NULL,&rxLen,&status) != pdTRUE) {
//		VT_HANDLE_FATAL_ERROR(0);
//	}
//
//	// Must wait 10ms after writing to the temperature sensor's configuration registers(per sensor data sheet)
//	//vTaskDelay(10/portTICK_RATE_MS);
//	// Tell it to start converting
//	if (vtI2CEnQ(devPtr,0x02,0x4F,sizeof(i2cCmdStartConvert),i2cCmdStartConvert,0) != pdTRUE) {
//		VT_HANDLE_FATAL_ERROR(0);
//	}
//	// wait on the result
//	if (vtI2CDeQ(devPtr,0,NULL,&rxLen,&status) != pdTRUE) {
//		VT_HANDLE_FATAL_ERROR(0);
//	}
//
//	// Scale the update rate to ensure it really is in ms
//	xUpdateRate = i2cREAD_RATE_BASE / portTICK_RATE_MS;
//
//	/* We need to initialise xLastUpdateTime prior to the first call to vTaskDelayUntil(). */
//	xLastUpdateTime = xTaskGetTickCount();
//	
//	// Like all good tasks, this should never exit
//	for(;;)
//	{
//	
//		/* Ask the RTOS to delay reschduling this task for the specified time */
//		vTaskDelayUntil( &xLastUpdateTime, xUpdateRate );
//
//	   	//Chris's graphing code
//
//
//		//ASK PIC FOR Y CORD
//		if (vtI2CEnQ(devPtr,0x04,0x4F,sizeof(i2cCmdReadCnt),i2cCmdReadCnt,1) != pdTRUE) {
//			VT_HANDLE_FATAL_ERROR(0);
//		}
//
//		if (vtI2CDeQ(devPtr,1,&temp1,&rxLen,&status) != pdTRUE) {
//			VT_HANDLE_FATAL_ERROR(0);
//		}
//
////		y = temp1;
////
////		printf("x:%d y:%d\n",(int)x,(int)y);
////		sprintf((char*)(lcdBuffer.buf),"%d,%d",(int)x,(int)y);
////
////		//really simple, just throw the data into the buffer and throw it at me. i take care of the rest.
////		if (xQueueSend(lcdData->inQ,(void *) (&lcdBuffer),portMAX_DELAY) != pdTRUE) {
////				VT_HANDLE_FATAL_ERROR(0);
////		}
////
////		x=x+1;
////		if(x > 320){
////			x = 0;
////		}
//
//		
//
//		//y=sin(x*(3.14/8.0));
//		//END graphing code
//
//
//		
////		// Read in the values from the temperature sensor
////		// We have three transactions on i2c to read the full temperature 
////		//   we send all three requests to the I2C thread (via a Queue) and *then* we wait on all three responses
////		// Temperature read -- use a convenient routine defined above
////		if (vtI2CEnQ(devPtr,0x03,0x4F,sizeof(i2cCmdReadVals),i2cCmdReadVals,1) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////	//	 Read in the read counter
////		if (vtI2CEnQ(devPtr,0x04,0x4F,sizeof(i2cCmdReadCnt),i2cCmdReadCnt,1) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////		// Read in the slope;
////		if (vtI2CEnQ(devPtr,0x05,0x4F,sizeof(i2cCmdReadSlope),i2cCmdReadSlope,1) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////
////		// wait on the results for the first read (ignore the second byte because we are doing a special computation -- see the sensor datasheet)
////		if (vtI2CDeQ(devPtr,1,&temp1,&rxLen,&status) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////		temperature = temp1;
////		vtITMu8(vtITMPortTempVals,rxLen); // Log the length received
////		
////		// wait on the results of the second read
////		if (vtI2CDeQ(devPtr,1,&temp1,&rxLen,&status) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////	//	countRemain = temp1;
////		vtITMu8(vtITMPortTempVals,rxLen); // Log the length received;
////		// wait on the results of the third read
////		if (vtI2CDeQ(devPtr,1,&temp1,&rxLen,&status) != pdTRUE) {
////			VT_HANDLE_FATAL_ERROR(0);
////		}
////		countPerC = temp1;	
////		vtITMu8(vtITMPortTempVals,rxLen); // Log the length received;
////		// end of i2c temperature read
////		
////
////		 //Do the accurate temperature calculation
////	//	temperature += -0.25 + ((countPerC-countRemain)/countPerC);
////
////		printf("temp2 = %f",temperature);
//// //
//////	    printf("Value from pic %x", temperature);
////
////		#if PRINTF_VERSION == 1
////	//	printf("Temp %f F (%f C)\n",(32.0 + ((9.0/5.0)*temperature)), (temperature));
////	    printf("temp2 = %f",temperature);
////		sprintf((char*)(lcdBuffer.buf),"temp2 = %f",temperature);
////		#else
////		// we do not have full printf (so no %f) and therefore need to print out integers
////		printf("Temp %d F (%d C)\n",lrint(32.0 + ((9.0/5.0)*temperature)), lrint(temperature));
////		sprintf((char*)(lcdBuffer.buf),"T=%d F (%d C)",lrint(32.0 + ((9.0/5.0)*temperature)),lrint(temperature));
////		#endif
////		if (lcdData != NULL) {
////			// Send a message to the LCD task for it to print (and the LCD task must be configured to receive this message)
////			lcdBuffer.length = strlen((char*)(lcdBuffer.buf))+1;
////			if (xQueueSend(lcdData->inQ,(void *) (&lcdBuffer),portMAX_DELAY) != pdTRUE) {
////				VT_HANDLE_FATAL_ERROR(0);
////			}
////		}
//		
//	}
//}
//





#include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #include <string.h>
 #include <sys/time.h>
 
/* Scheduler include files. */
 #include "FreeRTOS.h"
 #include "task.h"
 #include "projdefs.h"
 #include "semphr.h"
 
/* include files. */
 #include "vtUtilities.h"
 #include "vtI2C.h"
 #include "LCDtask.h"
 #include "i2cTemp.h"
 
// I have set this to a large stack size because of (a) using printf() and (b) the depth of function calls
 // for some of the LCD operations
 #if PRINTF_VERSION==1
 #define i2cSTACK_SIZE 12*configMINIMAL_STACK_SIZE
 #else
 #define i2cSTACK_SIZE 12*configMINIMAL_STACK_SIZE
 #endif
 
//v Set the task up to run every second
 #define i2cREAD_RATE_BASE ( ( portTickType ) 50 )
 
/* The i2cTemp task. */
 static portTASK_FUNCTION_PROTO( vi2cTempUpdateTask, pvParameters );
 
/*-----------------------------------------------------------*/
 
void vStarti2cTempTask( unsigned portBASE_TYPE uxPriority, i2cTempStruct *params)
{
 /* Start the task */
 	portBASE_TYPE retval;
 
	if ((retval = xTaskCreate( vi2cTempUpdateTask, ( signed char * ) "i2cTemp", i2cSTACK_SIZE, (void *) params, uxPriority, ( xTaskHandle * ) NULL )) != pdPASS) {
 		VT_HANDLE_FATAL_ERROR(retval);
 	}
}
 
// This is the actual task that is run
 static portTASK_FUNCTION( vi2cTempUpdateTask, pvParameters )
 {
 	portTickType xUpdateRate, 
	xLastUpdateTime;
 	uint8_t rxLen; 
	uint8_t status;
	uint8_t actualRxCount = 0;
	const uint8_t picAddress = 0x4F;
	const uint8_t picrxMsgLength = 3;
	const uint8_t pictxMsgLength = 2;

 // This should be created using the pack function
 // but you don't really need that right now
	const uint8_t pictxMsgData[] = {0x9, 0x1, 0x1};

	uint8_t *txBuf; //message sent to i2c
 	uint8_t *rxBuf; // message received from i2c

 // Get the parameters
 	i2cTempStruct *param = (i2cTempStruct *) pvParameters;

 // Get the I2C device pointer
 	vtI2CStruct *devPtr = param->dev;

	train_lookup *lookupArr = param->loconetData;

 // Get the LCD information pointer
 	vtLCDStruct *lcdData = param->lcdData;
 	vtLCDMsg lcdBuffer;
 
	// Concerning the LocoNet PIC
	uint8_t request_loco[]= {51, 0, 1, 0xBF, 0x00, 0x03, 0x00};
	uint8_t in_use[]= {51, 0, 1, 0xBA, 0x00, 0x00, 0x00};
	uint8_t set_forward[]= {51, 0, 1, 0xA0, 0x02, 0x7E, 0x00};
	uint8_t set_stop[]= {51, 0, 1, 0xA0, 0x02, 0x7E, 0x00};
	uint8_t set_reverse[]= {51, 0, 1, 0xB6, 0, 0, 0};
	uint8_t set_free[] = {51, 0, 1, 0xEF, 0x0E, 0x02, 0x03, 0x03, 0, 0, 0x07, 0, 0, 0, 0, 0x55, 0x7E};
	uint8_t switch_action[] = {51, 0, 1, 0, 0, 0, 0};

// Scale the update rate to ensure it really is in ms
 	xUpdateRate = i2cREAD_RATE_BASE / portTICK_RATE_MS;
 
/* We need to initialise xLastUpdateTime prior to the first call to vTaskDelayUntil(). */
 	xLastUpdateTime = xTaskGetTickCount();

	char test1[20];
	xQueueSend(param->inQ2, "testing to thread", portMAX_DELAY);
	xQueueReceive(param->inQ1, &test1, portMAX_DELAY);
	printf("\n%s\n",test1);

 // Like all good tasks, this should never exit
 	int prev=0;
	int sensor=0;
	char sensorNum[5];


 

 	for(;;)
 	{

		txBuf = (uint8_t *) malloc( 3 );
 		rxBuf = (uint8_t *) malloc( 2 ); //buffer for message received from the pic
 /* Ask the RTOS to delay reschduling this task for the specified time */
 		vTaskDelayUntil( &xLastUpdateTime, xUpdateRate );
 
		
		memcpy(txBuf, pictxMsgData, 2);
 		if (vtI2CEnQ(devPtr, 0x01, picAddress, pictxMsgLength, txBuf, picrxMsgLength) != pdTRUE) {
 			VT_HANDLE_FATAL_ERROR(0);
 		}
 		if (vtI2CDeQ(devPtr, 2, rxBuf, &rxLen, &status) != pdTRUE) {
 			VT_HANDLE_FATAL_ERROR(0);
 		}

		//printf("%x-%x\n",rxBuf[1],rxBuf[0]);

		if(rxBuf[1] == 0x00 && rxBuf[0] != 0x00){
			if(rxBuf[0] == 0x01){
				sensor = 1;
			}else if(rxBuf[0] == 0x02){
				sensor = 2;
			}else if(rxBuf[0] == 0x04){
				sensor = 3;
			}else if(rxBuf[0] == 0x08){
				sensor = 4;
			}else if(rxBuf[0] == 0x10){
				sensor = 5;
			}else if(rxBuf[0] == 0x20){
				sensor = 6;
			}else if(rxBuf[0] == 0x40){
				sensor = 7;
			}else if(rxBuf[0] == 0x80){
				sensor = 8;
			}
		}else if (rxBuf[0] == 0x00 && rxBuf[1] != 0x00){
			if(rxBuf[1] == 0x01){
				sensor = 9;
			}else if(rxBuf[1] == 0x02){
				sensor = 10;
			}else if(rxBuf[1] == 0x04){
				sensor = 11;
			}else if(rxBuf[1] == 0x08){
				sensor = 12;
			}else if(rxBuf[1] == 0x10){
				sensor = 13;
			}
		}else{
		 	sensor = 0;
		}

		if(prev != sensor){
			if(sensor != 0){
				sprintf(sensorNum,"%d",sensor);
			 	xQueueSend(param->inQ2, &sensorNum, portMAX_DELAY);	
			}

		 	prev = sensor;
		}
		//END OF SENSOR CODE

		//BEGINNING OF LOCONET CODE


		if (xQueueReceive(param->inQ1, &test1, 0) == pdTRUE) {
			printf("\ncommand: %s\n",test1);
			if (test1[0] == 's' && test1[1] == '1') {
					// Format speed instruction
					setTrainForward(lookupArr[0].trainSlot, 0, set_stop, 3);
					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_stop, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
				
			}  if (test1 == "s2") {
				if (lookupArr[1].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[1].trainSlot, 0, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}	
				}
			}  if (test1 == "s3") {
				if (lookupArr[2].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[2].trainSlot, 0, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
				}
			}  if (test1 == "r1") {
				if (lookupArr[0].isSet) {
					// Format speed instruction
					setTrainDirection(lookupArr[0].trainSlot, 0, set_reverse, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
					
				}
			}  if (test1 == "r2") {
				if (lookupArr[1].isSet) {
					// Format speed instruction
					setTrainDirection(lookupArr[1].trainSlot, 0, set_reverse, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
					
				}
			}  if (test1 == "r3") {
				if (lookupArr[2].isSet) {
					// Format speed instruction
					setTrainDirection(lookupArr[2].trainSlot, 0, set_reverse, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
				}
			}  if (test1[0] == 'f' && test1[1] == '1') {
				printf("sending f1 command over i2c");
				if (lookupArr[0].isSet) {

					set_forward[3] = 0xA0;
					set_forward[4] = 0x02;
					set_forward[5] = 0x3F;
					set_forward[6] = 0x62;
					vTaskDelay(100/portTICK_RATE_MS);
					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
					vTaskDelay(10/portTICK_RATE_MS);
				}
			}  if (test1 == "f2") {
				if (lookupArr[1].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[1].trainSlot, 126, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
					
				}
			}  if (test1 == "f3") {
				if (lookupArr[2].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[2].trainSlot, 126, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}
					
				}
			}  if (test1 == "h1") {
				if (lookupArr[0].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[0].trainSlot, 60, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}				
				}
			}  if (test1 == "h2") {
				if (lookupArr[1].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[1].trainSlot, 60, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}				
				}
			}  if (test1 == "h3") {
				if (lookupArr[2].isSet) {
					// Format speed instruction
					setTrainForward(lookupArr[2].trainSlot, 60, set_forward, 3);

					if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, set_forward, 0) != pdTRUE) {
						VT_HANDLE_FATAL_ERROR(0);
					}				
				}
			}  if (test1 == "x11") {
				// Format switch command
				setSwitchThrown(0x05, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
				
				// Format switch command
				setSwitchThrown(0x07, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x12") {
				// Format switch command
				setSwitchThrown(0x05, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
				
				// Format switch command
				setSwitchClosed(0x07, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x13") {
				// Format switch command
				setSwitchClosed(0x05, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
				
				// Format switch command
				setSwitchThrown(0x06, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x14") {
				// Format switch command
				setSwitchClosed(0x05, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
				
				// Format switch command
				setSwitchClosed(0x06, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x21") {
				// Format switch command
				setSwitchClosed(0x04, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1[0] == 'x' && test1[1] == '2' &&test1[2] == '2') {
				// Format switch command
				//setSwitchClosed(0x04, switch_action, 3);
				switch_action[3] = 0xB0;
				switch_action[4] = 0x04;
				switch_action[5] = 0x30;
				switch_action[6] = 0x7B;

				// Send message over I2C
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				switch_action[5] = 0x20;
				switch_action[6] = 0x6B;

					// Send message over I2C
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}

			}  if (test1 == "x31") {
				// Format switch command
				setSwitchClosed(0x00, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
			
				// Format switch command
				setSwitchThrown(0x03, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x32") {
				// Format switch command
				setSwitchThrown(0x00, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				
				vTaskDelay(3/portTICK_RATE_MS);
				
				// Format switch command
				setSwitchThrown(0x03, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1[0] == 'x' && test1[1] == '3' &&test1[2] == '3') {
				// Format switch command
				//setSwitchClosed(0x03, switch_action, 3);

				switch_action[3] = 0xB0;
				switch_action[4] = 0x03;
				switch_action[5] = 0x30;
				switch_action[6] = 0x7C;
				
				// Send message over I2C
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}

				
				switch_action[5] = 0x20;
				switch_action[6] = 0x6C;

					// Send message over I2C
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
				vTaskDelay(10/portTICK_RATE_MS);
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x41") {
				// Format switch command
				setSwitchClosed(0x01, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x03, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			}  if (test1 == "x42") {
				// Format switch command
				setSwitchThrown(0x03, switch_action, 3);
				
				// Send message over I2C
				if (vtI2CEnQ(devPtr, 0x01, 0x4E, 7, switch_action, 0) != pdTRUE) {
					VT_HANDLE_FATAL_ERROR(0);
				}
			} 
		}






 // do something with rxBuf here, the format will be as follows (unless you changed it)
 // | msgType | msgCount | msgOwner | data byte 1 | data byte 2|

 
// Example code for printing to the LCD if you want to do that...
 
//sprintf((char*)(lcdBuffer.buf),"", result);
 //really simple, just throw the data into the buffer and throw it at me. i take care of the rest.
 //if (xQueueSend(lcdData->inQ,(void *) (&lcdBuffer),portMAX_DELAY) != pdTRUE) {
 // VT_HANDLE_FATAL_ERROR(0);
 //}

 		free(rxBuf);
 		free(txBuf);
 	}
 }

 void unpack(uint8_t *msgType, uint8_t *count, uint8_t *msgOwner, uint8_t *rxBuf)
 { 
	(*msgType) = rxBuf[0];
 	(*count) = rxBuf[1];
 	(*msgOwner) = rxBuf[2];
}
 
void pack(uint8_t msgType, uint8_t count, uint8_t msgOwner, uint8_t msgLength, const uint8_t *msgBuf, uint8_t *txBuf)
 {
 	int i = 0;

 	txBuf[0] = msgType;
 	txBuf[1] = count;
 	txBuf[2] = msgOwner;

 	for(i = 0; i < msgLength; i++){
 		txBuf[i + 3] = msgBuf[i]; 
	}
}
