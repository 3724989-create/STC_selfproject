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


/**************************************************************************
 * 函 数 名：short Read_Encoder(u8 encno)
 * 函数功能：单位时间读取编码器计数
 * 入口参数：编码器编号  定时器3/4 计数中断管脚唯一固定（STC32G可有两组可选）
 * 返 回 值：脉冲数，注意读取之间周期内，不要超过16位最大值
 * 调用方式：Enc1 = Read_Encoder(1)；
 **************************************************************************/
short Read_Encoder(u8 encno)
{
  int32 tm=0;

  if(encno==Enc_1)
  {
		tm = T3H;
		if(P05)             //编码器1计数A:P04  DIR:P05
			tm=(tm<<8)|T3L;
		else
			tm=0-((tm<<8)|T3L);

		T4T3M &= ~(1<<3);
		T3H = 0;
		T3L = 0;
		T4T3M |= (1<<3);
  }
  else if(encno==Enc_2)
  {
		T4T3M &= ~(1<<7);
		tm = T4H;
		if(P07)             //编码器２计数A:P06  DIR:P07
			tm=(tm<<8)|T4L;
		else
			tm=0-((tm<<8)|T4L);


		T4H = 0;
		T4L = 0;
		T4T3M |= (1<<7);
  }
  return tm;
}

/*************************************************************************
*  函数名称：void Timer_EncInit(u8 tim_x)
*  功能说明：编码器初始化函数
*  参数说明：使用哪个定时器的作为外部技术输入使用
*  函数返回：无
*  修改时间：2024年12月28日
*  备    注：驱动个编码器，注意初始化哪个定时，选择对应的输入管脚
*************************************************************************/
void Timer_EncInit(u8 tim_x)
{
//    if(tim_x == Timer0)
//	{
//        T3L = 0; T3H = 0;
//        T4T3M |= 0x0C;
//    }
//    else if(tim_x == Timer1)
//    {
//        T4L = 0; T4H = 0;
//        T4T3M |= 0xC0;
//    }
//    else if(tim_x == Timer2)  //常被用作串口波特率发生器 
//    {
//        T4L = 0; T4H = 0;
//        T4T3M |= 0xC0;
//    } 
    
    if(tim_x == Timer3)
	{
        T3L = 0; T3H = 0;
        T4T3M |= 0x0C;
    }
    else if(tim_x == Timer4)
    {
        T4L = 0; T4H = 0;
        T4T3M |= 0xC0;
    }
    else if(tim_x == (Timer3|Timer4))
    {
        T3L = 0; T3H = 0;
        T4L = 0; T4H = 0;
        T4T3M |= 0xCC;
    }
    
//1100 1100 定时器4开始计数  定时器4计数模式  12分频  关闭时钟输出
//          定时器3开始计数  定时器3计数模式  12分频  关闭时钟输出

}

/*************************************************************************
 *  函数名称：void TestEncoder(void)
 *  功能说明：测试程序，OLED显示
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2024年11月15日
 *  备    注：
 *************************************************************************/

void Test_Encoder(void)
{
    char txt[16];
    short duty = 20;
    short enc1,enc2;

    OLED_Init();
    OLED_CLS();   	                         	//清屏
    OLED_P6x8Str(0, 0, "Test Encoder");        //字符串显示
    Timer_EncInit(Timer3|Timer4);               //两个方向编码器接口初始化， 定时器3/4做脉冲计数输入，管脚固定
//    Motor_Init();
    while (1)
    {
//        if (KEY_Read(KEY0) == 0)      //按下KEY0键，占空比减小
//        {
//            if (duty > -3600) 
//                duty -= 50;
//        }
//        if (KEY_Read(KEY2) == 0)      //按下KEY2键，占空比加大
//        {
//            if (duty < 3600)
//                duty += 50;
//        }
//        if (KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
//        {
//            duty = 20;
//        }
//        Motor_Ctrl(duty,duty);

        /* 获取编码器值 */
        enc1 = Read_Encoder(Enc_1); 			        //左电机 母板上编码器1，小车前进为负值
        enc2 = Read_Encoder(Enc_2); 			        //右电机 母板上编码器2，小车前进为正值

        sprintf(txt, "Enc1: %04d  ", enc1);
        OLED_P6x8Str(0, 3, txt);			        //字符串显示
		UART1_PutStr(txt);      //UART1 打印
        sprintf(txt, "PWM: %05d;", duty);
        OLED_P6x8Str(0, 4, txt);   //字符串显示
        sprintf(txt, "Enc2: %04d  ", enc2);
        OLED_P6x8Str(0, 5, txt);			        //字符串显示
		UART1_PutStr(txt);
        UART1_PutStr("\n");

        LED_Ctrl(LED0, RVS);       		        //电平翻转,LED闪烁
        delay_ms(50);							   //延时等待
    }

}


