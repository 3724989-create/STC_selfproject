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


/**************************************************************************************
 * 函数: GPIO_LED_Init(void)
 * 描述: 初始化LED程序. LED0 -->在LQ_LED.h中宏定义
 * 参数: None.
 * 返回: None.
 * 版本: V1.0, 2024-07-25
 * eg:   P4_MODE_OUT_PP(GPIO_Pin_2|GPIO_Pin_3); 将P42和P43设置为推挽输出
 *       对于LED，设置为准双向、推挽输出模式等都是可以的
 **************************************************************************************/
void GPIO_LED_Init(void)
{
//-------------------------初始化方式1--------------------------------
//    GPIO_InitTypeDef GPIO_LED_Initstu;                  //每个端口重新初始胡一个结构体
//
//    GPIO_LED_Initstu.Pin  = GPIO_Pin_1;                 //要初始化的I/O  eg: GPIO_Pin_0|GPIO_Pin_2;
//    GPIO_LED_Initstu.Mode = GPIO_OUT_PP;                //要初始化的I/O 模式 推挽输出
//    GPIO_Inilize(GPIO_P0,&GPIO_LED_Initstu);            //将P10,初始化为推挽输出模式


//-------------------------初始化方式2--------------------------------
//    P0_MODE_OUT_PP(GPIO_Pin_1);      // 初始化P01,P35 推挽输出模式   ，每个端口一个函数
    P3_MODE_OUT_PP(GPIO_Pin_5);
//-------------------------初始化方式3--------------------------------
    GPIO_InitConfig(P0_1,OUT_PP);       // 初始化P01,推挽输出模式   一个函数初始化管脚和模式
    
    GPIO_Write_Pin(LED0p,SET);
    GPIO_Write_Pin(Beep,RESET);           //初始状态设为高,LED低电平点亮
}


/**************************************************************************************
 * 函数：LED_Ctrl
 * 描述：控制LED状态
 * 参数：ledp:定义的LED管脚sta:状态 ON打开 OFF关闭 RVS反转
 * 返回：无
 * 时间：2024-11-01
 * 方法：LED_Ctrl(LED0,ON);
 * 备注：LED管脚在 头文件中宏定义
 **************************************************************************************/
void LED_Ctrl(LED_n ledp, LED_s sta)
{
	switch (ledp)
	{
	case LED0:
		if (sta == ON)			  LED0p = RESET;
		else if (sta == OFF)	  LED0p = SET;
		else if (sta == RVS)     LED0p =!LED0p;//(LED0p)?(LED0p=RESET):(LED0p=SET);
		break;
	case LED1:
		if (sta == ON)  		LED1p = RESET;
		else if (sta == OFF)	LED1p = SET;
		else if (sta == RVS)	LED1p =!LED1p;//(LED1p)?(LED1p=RESET):(LED1p=SET);
		break;
	case Beep0:
		if (sta == ON)  		Beep = RESET;
		else if (sta == OFF)	Beep = SET;
		else if (sta == RVS)	Beep =!Beep;    //(LED1p)?(LED1p=RESET):(LED1p=SET);
		break;
	case LED_ALL:
		if (sta == ON)
		{
			LED0p = RESET;
			LED1p = RESET;
		}
		else if (sta == OFF)
		{
			LED0p = SET;
			LED1p = SET;
		}
		else if (sta == RVS)
		{
			LED0p =!LED0p;//(LED0p)?(LED0p=RESET):(LED0p=SET);
			LED1p =!LED1p;
            Beep =!Beep;  //(LED1p)?(LED1p=RESET):(LED1p=SET);
		}
		break;
	default:;
        break;
	}
}

/**************************************************************************************
 * 函数: Test_LED(void)
 * 描述: LED测试函数，闪灯
 * 参数: None.
 * 返回: None.
 * 版本: V1.0, 2024-11-01
 **************************************************************************************/
void Test_GPIO_LED()
{
	GPIO_LED_Init();                // LED初始化，修改管脚 在LQ_LED.h 宏定义
//    GPIO_Write_Pin(Beep,RESET);   //或LED_Ctrl(Beep,OFF);
	while(1)
	{
        LED_Ctrl(LED0,RVS);
        LED_Ctrl(Beep0,RVS);
        delay_ms(250);
        //或者以下方式直接操作GPIO_Pin --> PnX
//        GPIO_Write_Pin(LED0p,RESET);      //由 #define LED0p  P01而来
//        delay_ms(200);
//        GPIO_Write_Pin(LED0p,SET);        //
//        delay_ms(200);

	}
}
