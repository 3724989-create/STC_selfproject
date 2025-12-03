
#ifndef __STARTASK_H_
#define __STARTASK_H_

#include "FreeRTOS.h"


void StartTask(void * pvParameters);
void Task2(void * pvParameters);
void LED0_Task(void * pvParameters);



#endif 
