
#ifndef __STATISTICALOS_H_
#define __STATISTICALOS_H_

#include "FreeRTOS.h"


#if configGENERATE_RUN_TIME_STATS == 1
extern u32 FreeRTOSRunTimeTicks;							//统计计数器
void ConfigureTimeForRunTimeStats(void);					//初始化一个硬件定时器
u32 GetFreeRTOSRunTimeTicks(void);							//返回统计计数器的值
#endif 


void UartRxGet(void);



#endif 
