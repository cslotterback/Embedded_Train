/*
    FreeRTOS V6.1.1 - Copyright (C) 2011 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS books - available as PDF or paperback  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public 
    License and the FreeRTOS license exception along with FreeRTOS; if not it 
    can be viewed here: http://www.freertos.org/a00114.html and also obtained 
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks
 * (which just exist to test the kernel port and provide an example of how to use
 * each FreeRTOS API function).
 *
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Check" hook -  This only executes fully every five seconds from the tick
 * hook.  Its main function is to check that all the standard demo tasks are
 * still operational.  The status can be viewed using on the Task Stats page
 * served by the WEB server.
 *
 * "uIP" task -  This is the task that handles the uIP stack.  All TCP/IP
 * processing is performed in this task.
 * 
 * "USB" task - Enumerates the USB device as a CDC class, then echoes back all
 * received characters with a configurable offset (for example, if the offset
 * is 1 and 'A' is received then 'B' will be sent back).  A dumb terminal such
 * as Hyperterminal can be used to talk to the USB task.
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "system_LPC17xx.h"

#ifndef   PCONP_PCTIM0
/* MTJ_NOTE: This will not compile properly if you do not delete the old version of */
/*       system_LPC17xx.h from the Keil compiler installation */
You should read the note above.
#endif

/* Define whether or not to start the standard FreeRTOS demo tasks (the code is still included in the project
   unless the files are actually removed from the project */
#define USE_FREERTOS_DEMO 0
// Define whether or not to use my LCD task
#define USE_MTJ_LCD 0
// Define whether to use my temperature sensor read task (the sensor is on the PIC v4 demo board, so if that isn't connected
//   then this should be off
#define USE_MTJ_V4Temp_Sensor 1
// Define whether to use my USB task
#define USE_MTJ_USE_USB 0

#if USE_FREERTOS_DEMO == 0
/* Demo app includes. */
#include "BlockQ.h"
#include "integer.h"
#include "blocktim.h"
#include "flash.h"
#include "semtest.h"
#include "PollQ.h"
#include "GenQTest.h"
#include "QPeek.h"
#include "recmutex.h"
#endif

#include "partest.h"

// Include file for MTJ's LCD & i2cTemp tasks
#include "trainThread.h"
#include "trainTester.h"
#include "vtUtilities.h"
#include "lcdTask.h"
#include "i2cTemp.h"
#include "vtI2C.h"
#include "lookup.h"

/* syscalls initialization -- *must* occur first */
#include "syscalls.h"
#include "extUSB.h"
#include <stdio.h>
/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
tick hook). */

#define mainCHECK_DELAY  					( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY)
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY)
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY)
#define mainUIP_TASK_PRIORITY				( tskIDLE_PRIORITY)
#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY)
#define mainGEN_QUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY)
#define mainFLASH_TASK_PRIORITY				( tskIDLE_PRIORITY)
#define mainLCD_TASK_PRIORITY				( tskIDLE_PRIORITY)
#define mainI2CTEMP_TASK_PRIORITY			( tskIDLE_PRIORITY)
#define mainUSB_TASK_PRIORITY				( tskIDLE_PRIORITY)
#define mainI2CMONITOR_TASK_PRIORITY		( tskIDLE_PRIORITY)

/* The WEB server has a larger stack as it utilises stack hungry string
handling library calls. */
#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 4 )

/* The message displayed by the WEB server when all tasks are executing
without an error being reported. */
#define mainPASS_STATUS_MESSAGE				"All tasks are executing without error."

/*-----------------------------------------------------------*/

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware( void );

/*
 * The task that handles the uIP stack.  All TCP/IP processing is performed in
 * this task.
 */
extern void vuIP_Task( void *pvParameters );

/*
 * The task that handles the USB stack.
 */
extern void vUSBTask( void *pvParameters );

/*
 * Simply returns the current status message for display on served WEB pages.
 */
char *pcGetTaskStatusMessage( void );

/*-----------------------------------------------------------*/

/* Holds the status message displayed by the WEB server. */
static char *pcStatusMessage = mainPASS_STATUS_MESSAGE;


#if USE_MTJ_V4Temp_Sensor == 1
// hold i2c device information for the I2C0 peripheral
static vtI2CStruct vtI2C0;
static train_lookup myLookup[MAXTRAINS];
static i2cTempStruct tempSensorParams;

#endif

#if USE_MTJ_LCD == 1
// hold queue information for the LCD
static vtLCDStruct vtLCDdata;
#endif

/*-----------------------------------------------------------*/

//unsigned long u1 =0;
//void vApplicationTickHook(void){
//	u1++;
//}


int main( void )
{
	/* MTJ: initialize syscalls -- *must* be first */
	// syscalls.c contains the files upon which the standard (and portable) C libraries rely 
	init_syscalls();

	// Set up the LED ports and turn them off
	vtInitLED();

	/* Configure the hardware for use by this demo. */
	prvSetupHardware();

	//SLOTTERBACKS CODE

	xQueueHandle xToThread, xToTester, xToLCD;
	xToThread = xQueueCreate(1,20);
	xToTester = xQueueCreate(1,20);
	if(xToThread == 0 || xToTester == 0){
		VT_HANDLE_FATAL_ERROR(0);
	}

	xToLCD = xQueueCreate(1,20);

	static msgStruct msgQueues;
	msgQueues.inQ1 = xToThread;
	msgQueues.inQ2 = xToTester;
	msgQueues.inQ3 = xToLCD;

	//



	#if USE_FREERTOS_DEMO == 1
	/* Start the standard demo tasks.  These are just here to exercise the
	kernel port and provide examples of how the FreeRTOS API can be used. */
	vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
    vCreateBlockTimeTasks();
    vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
    vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
    vStartIntegerMathTasks( mainINTEGER_TASK_PRIORITY );
    vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
    vStartQueuePeekTasks();
    vStartRecursiveMutexTasks();
	vStartLEDFlashTasks( mainFLASH_TASK_PRIORITY );
	#endif

	// Not a standard demo -- but also not one of mine (MTJ)
	/* Create the uIP task.  The WEB server runs in this task. */
    //xTaskCreate( vuIP_Task, ( signed char * ) "uIP", mainBASIC_WEB_STACK_SIZE, ( void * ) NULL, mainUIP_TASK_PRIORITY, NULL );

	// MTJ: My LCD demonstration task
	#if USE_MTJ_LCD == 1
	vtLCDdata.inQ3 = xToLCD;
	vStartLCDTask( mainLCD_TASK_PRIORITY,&vtLCDdata);
	#endif

	// MTJ: My i2cTemp demonstration task
	#if USE_MTJ_V4Temp_Sensor == 1
	vtI2C0.devNum = 0;
	vtI2C0.taskPriority = mainI2CMONITOR_TASK_PRIORITY;
	// Initialize I2C0 
	int retVal;
	if ((retVal = vtI2CInit(&vtI2C0,100000)) != vtI2CInitSuccess) {
		VT_HANDLE_FATAL_ERROR(retVal);
	}

	tempSensorParams.dev = &vtI2C0;
	initTrainData(myLookup);
	tempSensorParams.loconetData = myLookup;

	#if USE_MTJ_LCD == 1
	tempSensorParams.lcdData = &vtLCDdata;
	#else
	tempSensorParams.lcdData = NULL;
	#endif
	tempSensorParams.inQ1=xToThread;
	tempSensorParams.inQ2=xToTester;
	vStarti2cTempTask(mainI2CTEMP_TASK_PRIORITY,&tempSensorParams);
	#endif

    /* Create the USB task. MTJ: This routine has been modified from the original example (which is not a FreeRTOS standard demo) */
	#if USE_MTJ_USE_USB == 1
	initUSB();  // MTJ: This is my routine used to make sure we can do printf() with USB
    xTaskCreate( vUSBTask, ( signed char * ) "USB", configMINIMAL_STACK_SIZE, ( void * ) NULL, mainUSB_TASK_PRIORITY, NULL );
	#endif

	//SLOTTERBACKS CODE

	vStartTrainThread(tskIDLE_PRIORITY,&msgQueues);

	
	/* Start the scheduler. */
	// IMPORTANT: Once you start the scheduler, any variables on the stack from main (local variables in main) can be (will be...) writtenover
	//            because the stack is used by the interrupt handler
	vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle
    task.  The idle task is created within vTaskStartScheduler(). */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
static unsigned long ulTicksSinceLastDisplay = 0;

	/* Called from every tick interrupt as described in the comments at the top
	of this file.

	Have enough ticks passed to make it	time to perform our health status
	check again? */
	ulTicksSinceLastDisplay++;
	if( ulTicksSinceLastDisplay >= mainCHECK_DELAY )
	{
		/* Reset the counter so these checks run again in mainCHECK_DELAY
		ticks time. */
		ulTicksSinceLastDisplay = 0;

#if USE_FREERTOS_DEMO == 1
		/* Has an error been found in any task? */
		if( xAreGenericQueueTasksStillRunning() != pdTRUE )
		{
			pcStatusMessage = "An error has been detected in the Generic Queue test/demo.";
		}
		else if( xAreQueuePeekTasksStillRunning() != pdTRUE )
		{
			pcStatusMessage = "An error has been detected in the Peek Queue test/demo.";
		}
		else if( xAreBlockingQueuesStillRunning() != pdTRUE )
		{
			pcStatusMessage = "An error has been detected in the Block Queue test/demo.";
		}
		else if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
		{
			pcStatusMessage = "An error has been detected in the Block Time test/demo.";
		}
	    else if( xAreSemaphoreTasksStillRunning() != pdTRUE )
	    {
	        pcStatusMessage = "An error has been detected in the Semaphore test/demo.";
	    }
	    else if( xArePollingQueuesStillRunning() != pdTRUE )
	    {
	        pcStatusMessage = "An error has been detected in the Poll Queue test/demo.";
	    }
	    else if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
	    {
	        pcStatusMessage = "An error has been detected in the Int Math test/demo.";
	    }
	    else if( xAreRecursiveMutexTasksStillRunning() != pdTRUE )
	    {
	    	pcStatusMessage = "An error has been detected in the Mutex test/demo.";
	    }
#endif
	}
}
/*-----------------------------------------------------------*/

char *pcGetTaskStatusMessage( void )
{
	/* Not bothered about a critical section here. */
	return pcStatusMessage;
}
/*-----------------------------------------------------------*/

void prvSetupHardware( void )
{
	/* Disable peripherals power. */
	SC->PCONP = 0;

	/* Enable GPIO power. */
	SC->PCONP = PCONP_PCGPIO;

	/* Disable TPIU. */
	PINCON->PINSEL10 = 0;

	/*  Setup the peripheral bus to be the same as the PLL output (64 MHz). */
	SC->PCLKSEL0 = 0x05555555;

	/* Configure the LEDs. */
	vParTestInitialise();
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed char *pcTaskName )
{
	/* This function will get called if a task overflows its stack. */

	( void ) pxTask;
	( void ) pcTaskName;

	// MTJ: I have directed this to the fatal error handler
	VT_HANDLE_FATAL_ERROR(0);
	for( ;; );
}
/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2, CTCR_CTM_TIMER = 0x00, TCR_COUNT_ENABLE = 0x01;

	/* This function configures a timer that is used as the time base when
	collecting run time statistical information - basically the percentage
	of CPU time that each task is utilising.  It is called automatically when
	the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
	to 1). */

	/* Power up and feed the timer. */
	SC->PCONP |= 0x02UL;
	SC->PCLKSEL0 = (SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

	/* Reset Timer 0 */
	TIM0->TCR = TCR_COUNT_RESET;

	/* Just count up. */
	TIM0->CTCR = CTCR_CTM_TIMER;

	/* Prescale to a frequency that is good enough to get a decent resolution,
	but not too fast so as to overflow all the time. */
	TIM0->PR =  ( configCPU_CLOCK_HZ / 10000UL ) - 1UL;

	/* Start the counter. */
	TIM0->TCR = TCR_COUNT_ENABLE;
}
/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
	// Here we decide to go to sleep because we *know* that no other higher priority task is ready *and* we
	//   know that we are the lowest priority task (we are the idle task)
	// Important: We are just being *called* from the idle task, so we cannot run a loop or anything like that
	//   here.  We just go to sleep and then return (which presumably only happens when we wake up).
	vtITMu8(vtITMPortIdle,SCB->SCR);
	__WFI(); // go to sleep until an interrupt occurs
	// DO NOT DO THIS... It is not compatible with the debugger: __WFE(); // go into low power until some (not quite sure what...) event occurs
	vtITMu8(vtITMPortIdle,SCB->SCR+0x10);
}
