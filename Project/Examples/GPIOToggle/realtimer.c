#include "realtimer.h"

realTime mytime = {0,0,0,0};

void timerTask(void * input){
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10;

	while (1){
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime,xFrequency);
		
		mytime.milisecond = 0;
		mytime.second ++;
		if (mytime.second == 60){
			mytime.second = 0;
			mytime.minute++;
			if (mytime.minute == 60){
				mytime.minute = 0;
				mytime.hour ++;
			}
		}

		printTime();

		
	}

}

void printTime(void){
	printf("Time: %d : %d : %d\r\n", mytime.hour, mytime.minute, mytime.second);
	
}
