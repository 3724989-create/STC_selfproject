/*-------------------------------------------------------------
StarTask.c
-------------------------------------------------------------*/

#include "StarTask.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "UART1.H"


void Task2(void * pvParameters);


QueueHandle_t 		testBinary; //二值信号量
xSemaphoreHandle 	testSem;    //计数信号量
xSemaphoreHandle 	Uart1Mutex; //RTOS 互斥信号量

void StartTask(void * pvParameters)
{
	UART1_Init(115200);
	
	vSemaphoreCreateBinary( testBinary );           //创建信号量
	
	testSem = xSemaphoreCreateCounting(255,0);      //创建信号量
	
	Uart1Mutex = xSemaphoreCreateMutex();           //创建信号量
	
	xTaskCreate((TaskFunction_t ) Task2,
                (const char*    ) "task2",
                (uint16_t       ) 256,
                (void*          ) NULL,			//
                (UBaseType_t    ) 4,			//优先级
                (TaskHandle_t*  ) NULL  );
	
	while(1)
	{
        u8 uart_dat[16],len;
        
        xSemaphoreTake(Uart1Mutex,portMAX_DELAY);
        do{
            len = UART1_Receive(uart_dat,16);   
            UART1_Send(uart_dat,len);           //原路返回串口收到的数据
        }while(len==16);
        xSemaphoreGive(Uart1Mutex);

		vTaskDelay(20);
	}
	
	if(pvParameters);
}




/*-------------------------------------------------------------------- 
任务2
--------------------------------------------------------------------*/
void Task2(void * pvParameters) 
{
	while(1)
	{
		static u8 xdata Visualization[512];
        
        xSemaphoreTake(Uart1Mutex,portMAX_DELAY);
        
        vTaskList(Visualization);   //获取任务信息 
		printf("任务信息：\r\n");    //
		printf(Visualization);      //
        printf("\r\n");
        
		printf("任务运行时间统计信息：\r\n");  //
		vTaskGetRunTimeStats(Visualization);    //获取任务运行时间统计信息
		printf(Visualization);					//输出到串口
        
        xSemaphoreGive(Uart1Mutex);
		
		vTaskDelay(1000);
        
		P46 = !P46;
	}
	if(pvParameters);
}


void LED0_Task(void * pvParameters) 
{
	while(1)
	{
        P46 = !P46;
        vTaskDelay(500);
        printf("read\r\n");
        //UART1_Send(1,1);
	}
	if(pvParameters);
}



