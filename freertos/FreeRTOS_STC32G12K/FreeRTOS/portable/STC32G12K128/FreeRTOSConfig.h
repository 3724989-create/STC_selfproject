/*
 * FreeRTOSv202212.01
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "Config.h"
#include "os_pendsv_config.h"




/***************************************************************************************************************/
/*                                        FreeRTOS基础配置配置选项                                             */
/***************************************************************************************************************/

#define configUSE_PREEMPTION                    1                       //1使用抢占式内核，0使用协程
#define configUSE_TIME_SLICING                  1                       //1使能时间片调度(默认式使能的)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0                       //1启用特殊方法来选择下一个要运行的任务
                                                                        //一般是硬件计算前导零指令，如果所使用的
                                                                        //MCU没有这些硬件指令的话此宏应该设置为0！
#define configUSE_TICKLESS_IDLE                 0                       //1启用低功耗tickless模式
#define configUSE_QUEUE_SETS                    0                       //为1时启用队列
#define configCPU_CLOCK_HZ                      (MAIN_Fosc)             //CPU频率
#define configTICK_RATE_HZ                      ((TickType_t)1000)      //时钟节拍频率
#define configMAX_PRIORITIES                    (16)                    //可使用的最大优先级
#define configDEFAULT_PRIORITIES                (tskIDLE_PRIORITY + 1)  //任务默认优先级
#define configMINIMAL_STACK_SIZE                ((unsigned short)256)   //空闲任务使用的堆栈大小
#define configDEFAULT_STACK_SIZE                ((unsigned short)256)   //任务默认堆栈大小
#define configMAX_TASK_NAME_LEN                 (8)                     //任务名字字符串长度
#define configUSE_IDLE_HOOK						0                       //1使用空闲钩子,0不使用 /*void vApplicationIdleHook( void );*/
#define configUSE_TICK_HOOK						0                       //1使用时间片钩子,0不使用 /*void vApplicationTickHook( void );*/
#define configSUPPORT_DYNAMIC_ALLOCATION        1                       //支持动态内存申请
#define configTOTAL_HEAP_SIZE					((size_t)(2*1024))     	// 系统所有总的堆大小

#define configUSE_16_BIT_TICKS                  1                       //系统节拍计数器变量数据类型，
                                                                        //1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD                 1                       //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configUSE_TASK_NOTIFICATIONS            1                       //为1时开启任务通知功能，默认开启
#define configUSE_MUTEXES                       1                       //为1时使用互斥信号量
#define configQUEUE_REGISTRY_SIZE               0                       //不为0时表示启用队列记录，具体的值是可以
                                                                        //记录的队列和信号量最大数目。

#define configUSE_RECURSIVE_MUTEXES             0                       //为1时使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK            0                       //1使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           1                       //为1时使用计数信号量




//运行时间统计功能
#define configGENERATE_RUN_TIME_STATS	        1                      	//为1时启用运行时间统计功能

#if configGENERATE_RUN_TIME_STATS == 1
	void ConfigureTimeForRunTimeStats(void);
	u32 GetFreeRTOSRunTimeTicks(void);
	#define configUSE_TRACE_FACILITY					1                       //为1启用可视化跟踪调试
	#define configUSE_STATS_FORMATTING_FUNCTIONS		1                       /*  与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
                                                                                    prvWriteNameToBuffer(),vTaskList(),
                                                                                    vTaskGetRunTimeStats()*/
	#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() 	ConfigureTimeForRunTimeStats()		//初始化一个硬件定时器
	#define portGET_RUN_TIME_COUNTER_VALUE()			GetFreeRTOSRunTimeTicks()			//获取计数器的值
#else
	#define configUSE_TRACE_FACILITY					0                       //为1启用可视化跟踪调试
	#define configUSE_STATS_FORMATTING_FUNCTIONS		0                       //与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
#endif



/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 			        	0                       //为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES         	(2)                     //协程的有效优先级数目



/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_eTaskGetState                   1

#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_uxTaskGetStackHighWaterMark		0			//栈的历史剩余最小值		
#define INCLUDE_xTaskGetIdleTaskHandle			0			//获取空闲任务句柄

#define configCHECK_FOR_STACK_OVERFLOW          0           //大于0时启用堆栈溢出检测功能，如果使用此功能,用户必须提供一个栈溢出钩子函数 vApplicationStackOverflowHook



#endif /* FREERTOS_CONFIG_H */




