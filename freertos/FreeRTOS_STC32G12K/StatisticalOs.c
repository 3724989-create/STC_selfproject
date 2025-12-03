/*-------------------------------------------------------------
StatisticalOs.c
-------------------------------------------------------------*/

#include "StatisticalOs.h"
#include "FreeRTOS.h"
#include "task.h"


#if configUSE_IDLE_HOOK >0

/*-------------------------------------------------------------------- 
空闲任务回调
--------------------------------------------------------------------*/
void vApplicationIdleHook( void )
{
}

#endif //configUSE_IDLE_HOOK




#if configCHECK_FOR_STACK_OVERFLOW > 0
/*-------------------------------------------------------------------- 
堆栈溢出回调
--------------------------------------------------------------------*/
void vApplicationStackOverflowHook( TaskHandle_t xTask,char *pcTaskName )
{
	if(pcTaskName);
	if(xTask);
}

#endif //configCHECK_FOR_STACK_OVERFLOW





//任务运行时间统计功能
#if configGENERATE_RUN_TIME_STATS == 1

u32 FreeRTOSRunTimeTicks;
    
/*-------------------------------------------------------------
定时器3配置, 任务运行时间统计功能使用定时器3时钟
-------------------------------------------------------------*/
#define TIM3_TICKS_PER_SEC    10000
#define TM3PS_VALUE    ( MAIN_Fosc / TIM3_TICKS_PER_SEC / 65536UL )
#define RELOAD_VALUE   ( 65536UL - MAIN_Fosc / (TM3PS_VALUE+1U) / TIM3_TICKS_PER_SEC )
void Timer3_Init(void)		//500微秒@11.0592MHz
{
	T4T3M |= 0x02;			                    //定时器时钟1T模式
	T3L = ( uint8_t )( RELOAD_VALUE );          //设置定时初始值
	T3H = ( uint8_t )( RELOAD_VALUE >> 8 );     //设置定时初始值
    T3IF = 0;                                   //清除标志
    ET3 = 1;                                    //开中断
	T3R = 1;			                        //定时器3开始计时
}


/*-------------------------------------------------------------------- 
Timer3中断
--------------------------------------------------------------------*/
void Timer3_ISR_Handler (void) interrupt TMR3_VECTOR
{
	FreeRTOSRunTimeTicks++;
}

//初始化一个硬件定时器
void ConfigureTimeForRunTimeStats(void)
{
	FreeRTOSRunTimeTicks = 0;
	Timer3_Init();
}

//返回计数器的值
u32 GetFreeRTOSRunTimeTicks(void)
{
	u32 val;
	u8	cpu_sr;
	cpu_sr = portSET_INTERRUPT_MASK_FROM_ISR();
	val = FreeRTOSRunTimeTicks;
	portCLEAR_INTERRUPT_MASK_FROM_ISR(cpu_sr);
	return val;
}

#endif  //configGENERATE_RUN_TIME_STATS






