#ifndef __TIMER_H_
#define __TIMER_H_

#include "define.h"

typedef struct {
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned int milisecond;
} realTime;

void timerTask(void*);
void printTime(void);


#endif



