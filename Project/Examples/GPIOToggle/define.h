#ifndef __DEFINE_H_
#define __DEFINE_H_



/* Kernel includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "timers.h"
//#include "semphr.h"

/* firmware include */
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "stm32f10x_usart.h"

/* user include */
#include "debug.h"
#include "realtimer.h"


/* Standard includes. */
#include <stdio.h>



/* define */
#define mainQUEUE_RECEIVE_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )
#define	mainQUEUE_SEND_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define mainEVENT_SEMAPHORE_TASK_PRIORITY	( configMAX_PRIORITIES - 1 )

#define mainQUEUE_SEND_PERIOD_MS			( 200 / portTICK_RATE_MS )

#define mainSOFTWARE_TIMER_PERIOD_MS		( 1000 / portTICK_RATE_MS )

#define mainQUEUE_LENGTH					( 1 )




#endif



