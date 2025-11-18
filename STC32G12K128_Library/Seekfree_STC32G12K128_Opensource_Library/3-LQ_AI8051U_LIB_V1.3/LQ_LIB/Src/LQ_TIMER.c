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



/********************************************************************************
 * 函数名：void TIM_Init_ms(u8 Tim,u16 time_ms)
 * 描  述：定时器 毫秒定时 初始化配置
 * 参  数：TIM_e tim,u16 ms 定时器，定时时间ms  1~950ms
 * 返回值：无
 * 备  注：AI8051U 6个定时器均为24位，分为8位预分频寄存器和16位定时计数装载寄存器
 * 中断优先级：
 *         (0->3--低->高) Priority_0,Priority_1,Priority_2,Priority_3
 *-----------------------------------------------------------------------------
 *******************************************************************************/
void TIM_Init(u8 Tim,u32 temp)
{
    u8 freq_ps =0,tim_clockT=0;
    u16 Period=0;
//    u32 temp =0;
//    temp = (times * (MAIN_Fosc / 1000));
    
    if( temp*255> 65535)
    {
        tim_clockT = TIM_CLOCK_12T;
        temp = temp / 12;
    }
    else tim_clockT = TIM_CLOCK_1T;
        
    
    freq_ps = (u8)((temp) >> 16);                  //获取8位 分频值
    Period = (u16)(65535 -(temp / (freq_ps+1)));   //获取16位 装载值

   {
    switch(Tim)
    {
        TIM_InitTypeDef	 TIMx_InitStructure;
        
     case Timer0:
     {
        TIMx_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   16位自动重装载模式
        TIMx_InitStructure.TIM_ClkSource = tim_clockT;		    //指定时钟源,     指定时钟源为12T模式,TIM_CLOCK_Ext外部时钟
        TIMx_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
        TIMx_InitStructure.TIM_Value     = Period;              //初值,装载值
        TIMx_InitStructure.TIM_PS        = freq_ps;		        //8位预分频器(n+1), 0~255
        TIMx_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
        Timer_Inilize(Timer0,&TIMx_InitStructure);				//初始化Timer0
        NVIC_Timer0_Init(ENABLE,Priority_1);		            //中断使能,中断优先级设置(空)  ENABLE/DISABLE;
     }break;
     
     case Timer1:
     {
        TIMx_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   16位自动重装载模式
        TIMx_InitStructure.TIM_ClkSource = tim_clockT;		    //指定时钟源,     指定时钟源为12T模式
        TIMx_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
        TIMx_InitStructure.TIM_Value     = Period;              //初值,装载值
        TIMx_InitStructure.TIM_PS        = freq_ps;		        //8位预分频器(n+1), 0~255
        TIMx_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
        Timer_Inilize(Timer1,&TIMx_InitStructure);				//初始化Timer1
        NVIC_Timer1_Init(ENABLE,Priority_1);		            //中断使能,中断优先级设置(空)  ENABLE/DISABLE;
     }break;
 
     case Timer3:
     {
        TIMx_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   16位自动重装载模式
        TIMx_InitStructure.TIM_ClkSource = tim_clockT;		    //指定时钟源,     指定时钟源为12T模式
        TIMx_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
        TIMx_InitStructure.TIM_Value     = Period;              //初值,装载值
        TIMx_InitStructure.TIM_PS        = freq_ps;		        //8位预分频器(n+1), 0~255
        TIMx_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
        Timer_Inilize(Timer3,&TIMx_InitStructure);				//初始化Timer0
        NVIC_Timer3_Init(ENABLE,Priority_1);		            //中断使能,中断优先级设置(空)  ENABLE/DISABLE;
     }break;
     
     case Timer4:
     {
        TIMx_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   16位自动重装载模式
        TIMx_InitStructure.TIM_ClkSource = tim_clockT;		    //指定时钟源,     指定时钟源为12T模式
        TIMx_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
        TIMx_InitStructure.TIM_Value     = Period;              //初值,装载值
        TIMx_InitStructure.TIM_PS        = freq_ps;		        //8位预分频器(n+1), 0~255
        TIMx_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
        Timer_Inilize(Timer4,&TIMx_InitStructure);				//初始化Timer1
        NVIC_Timer4_Init(ENABLE,Priority_1);		            //中断使能,中断优先级设置(空)  ENABLE/DISABLE;
     }break;   
     /*
     定时器11可选择时钟源（定时器0-4默认使用sys系统时钟1T或12T作为时钟源）：
     0：内部SYS时钟；
     1：内部高速IRC时钟；
     2：外部高速晶振；//龙邱核心板因管脚资源有限暂无设计外部晶振
     3：内部低速IRC时钟
     */
     case Timer11:
     {
        TIMx_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   16位自动重装载模式
        TIMx_InitStructure.TIM_ClkSource = tim_clockT;		    //指定时钟源系统时钟,     指定时钟源为12T模式，Timer11这里默认使用系统时钟
        TIMx_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
        TIMx_InitStructure.TIM_Value     = Period;              //初值,装载值
        TIMx_InitStructure.TIM_PS        = freq_ps;		        //8位预分频器(n+1), 0~255
        TIMx_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
        Timer_Inilize(Timer11,&TIMx_InitStructure);				//初始化Timer11
        NVIC_Timer11_Init(ENABLE,Priority_1);		            //中断使能,中断优先级设置(空)  ENABLE/DISABLE;
     }break;     

     default:
        /*  */
        break;
    }
    
   }
}



/********************************************************************************
 * 函数名：void TIM_Init_ms(u8 Tim_x,u16 time_ms)
 * 描  述：定时器 毫秒定时 初始化配置
 * 参  数：TIM_e tim,u16 ms 定时器，定时时间ms  1~950ms
 * 返回值：无
 * 备  注：AI8051U 6个定时器均为24位，分为8位预分频寄存器和16位定时计数装载寄存器
 *******************************************************************************/
void TIM_Init_ms(u8 Tim_x,u16 time_ms)
{
    TIM_Init(Tim_x,(time_ms * (MAIN_Fosc / 1000)));
}
/********************************************************************************
 * 函数名：void TIM_Init_us(u8 Tim_x,u32 time_us)
 * 描  述：定时器 微秒定时 初始化配置
 * 参  数：TIM_e tim,u16 us 定时器，定时单位us 1-9500000 us
 * 返回值：无
 * 备  注：AI8051U 6个定时器均为24位，分为8位预分频寄存器和16位定时计数装载寄存器
 *******************************************************************************/
void TIM_Init_us(u8 Tim_x,u32 time_us)
{
    TIM_Init(Tim_x,(time_us * (MAIN_Fosc / 1000000)));    
}


void Test_Timer()
{
    OLED_Init();                    //OLED初始化
    OLED_CLS();     //加入OLED显示
    OLED_P6x8Str(10, 0, "Timer LED blink...");
    TIM_Init_ms(Timer11,4500);  //4.5s 支持1us~4.8s
//    TIM_Init_us(Timer0,100);  //1us -->>4.8s

	printf("Test timer init OK!\r\n");	//UART发送一个字符串
    while(1)
    {

    }

}

