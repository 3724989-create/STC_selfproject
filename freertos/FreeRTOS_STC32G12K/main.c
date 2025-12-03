/*---------------------------------------------------------
MAIN.C
----------------------------------------------------------*/

#include "Config.h"
#include "STC32G_GPIO.h"
#include "StarTask.h"
#include "task.h"


void XOSCClkConfig(u8 div);


/*---------------------------------------------------------
GPIO_config
----------------------------------------------------------*/
void GPIO_config(void)
{
	P0_MODE_IO_PU(GPIO_Pin_All);		//P0 设置为准双向口
	P1_MODE_IO_PU(GPIO_Pin_All);		//P1 设置为准双向口
	P2_MODE_IO_PU(GPIO_Pin_All);		//P2 设置为准双向口
	P3_MODE_IO_PU(GPIO_Pin_All);		//P3 设置为准双向口
	P4_MODE_IO_PU(GPIO_Pin_All);		//P4 设置为准双向口
	P5_MODE_IO_PU(GPIO_Pin_All);		//P5 设置为准双向口
	P6_MODE_IO_PU(GPIO_Pin_All);		//P6 设置为准双向口
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 设置为准双向口
	P7_MODE_IO_PU(GPIO_Pin_All);		//P7 设置为准双向口
    
    P4_MODE_OUT_PP(GPIO_Pin_6); //LED
}


/*---------------------------------------------------------
main
----------------------------------------------------------*/
void main(void)
{
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)访问使能 
	CKCON = 0;      //提高访问XRAM速度
    
    GPIO_config();  //GPIO 初始化
    
    #if USE_Extern_Fosc
        XOSCClkConfig(1);       //切换时钟
    #endif
    
//	/* 创建首个任务 */
//	xTaskCreate((TaskFunction_t )LED0_Task,
//                (const char*    )"LED0_Task",
//                (uint16_t       )256,
//                (void*          )NULL,
//                (UBaseType_t    )5,
//                (TaskHandle_t*  )NULL );
//	
//	vTaskStartScheduler();	/* 启动任务调度器开始任务调度 */
    printf("123\r\n");

}





