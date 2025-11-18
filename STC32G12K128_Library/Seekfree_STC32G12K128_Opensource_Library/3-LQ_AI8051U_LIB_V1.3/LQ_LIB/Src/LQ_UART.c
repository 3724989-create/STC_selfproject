/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32位核心板
 【编    写】龙邱科技
 【E-mail  】chiusir@163.com
 【软件版本】V1.0 版权所有，单位使用请先联系授权
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 --------------------------------------------------------------------------------
 【  IDE  】 keil C251 V5.60
 【Target 】 STC32G/STC8051U/AI8051U 32位模式
 【SYS CLK】 42 MHz使用内部晶振
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"
#include "string.h"





u8 xdata DMA_URT1_Buf[128];	   //收发缓存，同时使用多路串口时每个串口需分别定义缓存，以免相互干扰
u8 xdata DMA_URX1_Buf[128];

/******************** DMA 配置 ********************/
void UART1_DMA_config()
{
	DMA_UART_InitTypeDef		DMA_UART_InitStructure;		    //结构定义

	DMA_UART_InitStructure.DMA_TX_Length = 128;				    //DMA传输总字节数  	(0~65535) + 1
	DMA_UART_InitStructure.DMA_TX_Buffer = (u16)DMA_URT1_Buf;	//发送数据存储地址
	DMA_UART_InitStructure.DMA_RX_Length = 128;				    //DMA传输总字节数  	(0~65535) + 1
	DMA_UART_InitStructure.DMA_RX_Buffer = (u16)DMA_URX1_Buf;	//接收数据存储地址
	DMA_UART_InitStructure.DMA_TX_Enable = ENABLE;		        //DMA使能  	ENABLE,DISABLE
	DMA_UART_InitStructure.DMA_RX_Enable = ENABLE;		        //DMA使能  	ENABLE,DISABLE
	DMA_UART_Inilize(UART1, &DMA_UART_InitStructure);	        //初始化

	NVIC_DMA_UART1_Tx_Init(ENABLE,Priority_0,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0~Priority_3; 总线优先级(低到高) Priority_0~Priority_3
	NVIC_DMA_UART1_Rx_Init(ENABLE,Priority_0,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0~Priority_3; 总线优先级(低到高) Priority_0~Priority_3

	DMA_UR1R_CLRFIFO();		//清空 DMA FIFO
//    DMA_UR4R_CLRFIFO();		//清空 DMA FIFO
}



/**************************************************************************************
 * 函数：void UART_Init(u8 UARTx,u32 BaudRate)
 * 描述：UART初始化
 * 参数：u8 UARTx,u32 BaudRate--》串口号，波特率：115200ul
 * 返回：无
 * 时间：2024-11-14
 * 方法：UART_Init(UART1,115200ul);  //初始化串口1，其他参数配置详见如下
 * 注意：若切换串口默认管脚，需要切换后修改下面TxRx_InitStructure参数，选择对应的管家初始化
 **************************************************************************************/
void UART_Init(u8 UARTx,u32 BaudRate)
{
    GPIO_InitTypeDef	TxRx_InitStructure;		    //结构定义
    COMx_InitDefine		COMx_InitStructure;			//结构定义

    switch(UARTx)
    {
        case UART1:
        {
            //串口1 管脚选择 TX:P30,RX:P31
            UART1_SW(UART1_SW_P30_P31);		            //UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44

            //初始化串口使用收发管脚，需要与实际选择的串口保持一致
            TxRx_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的UART管脚, 默认GPIO_Pin_0 ~ GPIO_Pin_1
            TxRx_InitStructure.Mode = GPIO_PullUp;	                //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
            GPIO_Inilize(GPIO_P3,&TxRx_InitStructure);	            //初始化，结构体配置

            //配置串口通信模式，中断优先级等
            COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
            COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
            COMx_InitStructure.UART_BaudRate  = BaudRate;			//波特率,     110 ~ 115200
            COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
            UART_Configuration(UART1, &COMx_InitStructure);		    //初始化串口 UART1
            NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        }break;

//        case UART2:
//        {
//            //串口2 管脚选择
//            UART2_SW(UART2_SW_P12_P13);		//UART2_SW_P12_P13,UART2_SW_P42_P43
//
//            //初始化串口使用收发管脚，需要与实际选择的串口保持一致
//            TxRx_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;      //指定要初始化的IO,
//            TxRx_InitStructure.Mode = GPIO_PullUp;	                //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
//            GPIO_Inilize(GPIO_P1,&TxRx_InitStructure);	            //初始化，结构体配置
//
//            //配置串口通信模式，中断优先级等
//            COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
//            COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
//            COMx_InitStructure.UART_BaudRate  = BaudRate;			//波特率,     110 ~ 115200
//            COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
//            UART_Configuration(UART2, &COMx_InitStructure);		    //初始化串口 UART2
//            NVIC_UART2_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
//        }break;

        case UART3:
        {
            //串口3 管脚选择
            UART3_SW(UART3_SW_P50_P51);		//UART3_SW_P00_P01,UART3_SW_P50_P51

            //初始化串口使用收发管脚，需要与实际选择的串口保持一致
            TxRx_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;      //指定要初始化的IO
            TxRx_InitStructure.Mode = GPIO_PullUp;	                //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
            GPIO_Inilize(GPIO_P0,&TxRx_InitStructure);	            //初始化，结构体配置

            //配置串口通信模式，中断优先级等
            COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
            COMx_InitStructure.UART_BRT_Use   = BRT_Timer3;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
            COMx_InitStructure.UART_BaudRate  = BaudRate;			//波特率,     110 ~ 115200
            COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
            UART_Configuration(UART3, &COMx_InitStructure);		    //初始化串口 UART3
            NVIC_UART3_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        }break;

        case UART4:
        {
            //串口4 管脚选择
            UART4_SW(UART4_SW_P02_P03);		//UART4_SW_P02_P03,UART4_SW_P52_P53

            //初始化串口使用收发管脚，需要与实际选择的串口保持一致
            TxRx_InitStructure.Pin  = GPIO_Pin_2 | GPIO_Pin_3;      //指定要初始化的IO
            TxRx_InitStructure.Mode = GPIO_PullUp;	                //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
            GPIO_Inilize(GPIO_P0,&TxRx_InitStructure);	            //初始化，结构体配置

            //配置串口通信模式，中断优先级等
            COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,   UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
            COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//选择波特率发生器, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
            COMx_InitStructure.UART_BaudRate  = BaudRate;			//波特率,     110 ~ 115200
            COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
            UART_Configuration(UART4, &COMx_InitStructure);		    //初始化串口UART4
            NVIC_UART4_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        }break;

       default: //非法字段
            break;

    }

}


/**************************************************************************************
 *函数: void Test_UART(void)
 *描述: 串口收发送测试程序.串口接收什么便回发什么
 *参数: None.
 *返回: None.
 *版本: V1.0, 2024-12-21
 *************************************************************************************/
void Test_UART_DMA()
{
 	u8	i,len;
	char strs[40]=("AI8051U UART DMA Test Programme!\r\n");  //UART发送一个字符串
    OLED_P6x8Str(19,2,"LQ OLED Test...");		//字符串显示
	UART_Init(UART1,115200ul);      //初始化串口1 波特率115200 8位数据,选择管脚需要进入该函数
    Global_IRQ_Enable();            //使能全局中断
	UART1_DMA_config();


	DmaTx1Flag = 0;
	DmaRx1Flag = 0;
    
	DMA_UR1R_TRIG();	//触发UART1接收功能  

    len=strlen(strs); 
    for (i = 0; i < len; i++) 
    {
        DMA_URT1_Buf[i]=strs[i];
    }
	DMA_UR1T_TRIG();	//触发UART1发送功能,一次性发送字符串strs


	while (1)
	{
        DmaTx1Flag = 0;
        DmaRx1Flag = 0;
        
        DMA_UR1R_TRIG();	//触发UART1接收功能  
        
        if(DmaRx1Flag)            //接收缓存满了溢出标志位1
        {
            len=strlen(DMA_URX1_Buf); 
//            DMA_UR1R_CLRFIFO();
            for (i = 0; i < len; i++) 
            {
                DMA_URT1_Buf[i]=DMA_URX1_Buf[i];
            }
            DMA_UR1T_TRIG();	//触发UART1发送功能
        }
	}

}


/**************************************************************************************
 *函数: void Test_UART(void)
 *描述: 串口收发送测试程序.串口接收什么便回发什么
 *参数: None.
 *返回: None.
 *版本: V1.0, 2024-10-14
 *************************************************************************************/
void Test_UART()
{
	u8	i;
	UART_Init(UART1,115200ul);      //初始化串口1 波特率115200 8位数据,选择管脚需要进入该函数
    Global_IRQ_Enable();            //使能全局中断
    UART_Init(UART4,115200ul);      //初始化串口1 波特率115200 8位数据,选择管脚需要进入该函数
    UART4_PutStr("UART4 Init OK \n");
	printf("UART1 Init OK \n");
    while(1)
    {
		if(COM1.RX_TimeOut > 0)		//超时计数 COM1.RX_TimeOut ，RX_Cnt等由STC32_UART文件定义
		{
			if(--COM1.RX_TimeOut == 0)
			{
				if(COM1.RX_Cnt > 0)
				{
					for(i=0; i<COM1.RX_Cnt; i++)	UART1_PutBuf(RX1_Buffer[i]);	//收到的数据原样返回
				}
				COM1.RX_Cnt = 0;
			}
		}
        if(COM4.RX_TimeOut > 0)		//超时计数 COM1.RX_TimeOut ，RX_Cnt等由STC32_UART文件定义
		{
			if(--COM4.RX_TimeOut == 0)
			{
				if(COM4.RX_Cnt > 0)
				{
					for(i=0; i<COM4.RX_Cnt; i++)	UART4_PutBuf(RX4_Buffer[i]);	//收到的数据原样返回
				}
				COM4.RX_Cnt = 0;
			}
		}

        LED_Ctrl(LED0,RVS);
        delay_ms(50);
//        printf("LED0 状态设置成功! \n");
    }

}




