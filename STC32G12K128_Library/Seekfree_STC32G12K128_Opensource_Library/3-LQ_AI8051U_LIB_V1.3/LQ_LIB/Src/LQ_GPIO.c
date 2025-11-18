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
#include "LQ_GPIO.h"

/*************************************************************************
*  函数名称：void GPIO_InitConfig(GPIO_Pin_e Pin_x, Port_Mode mode)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2024年11月1日
*  举    例： GPIO_InitConfig(P0_0, GPIO_OUT_PP);  //P0_0初始化推挽输出
*************************************************************************/
void GPIO_InitConfig(GPIO_Pin_e Pin_x, GPIO_Mode_e Mode_e)
{
	u8 GPIO_Port_n, GPIO_Pin_n;

	GPIO_Port_n = (Pin_x&0xF0);    //Pport_num.x
	GPIO_Pin_n  = (Pin_x&0x0F);    //Px.pin_num

	switch(GPIO_Port_n)
	{
		case 00:
			if(Mode_e == OUT_IN)    {P0M1 &= ~(1<<GPIO_Pin_n);	P0M0 &= ~(1<<GPIO_Pin_n);}  //准双向口      00
			if(Mode_e == OUT_PP)    {P0M1 &= ~(1<<GPIO_Pin_n);	P0M0 |=  (1<<GPIO_Pin_n);}  //推挽输出      01
			if(Mode_e == HighZ_IN)  {P0M1 |=  (1<<GPIO_Pin_n);	P0M0 &= ~(1<<GPIO_Pin_n);}  //高阻态浮空    10
			if(Mode_e == HighZ_IN)  {P0M1 |=  (1<<GPIO_Pin_n);	P0M0 |=  (1<<GPIO_Pin_n);}  //开漏输出      11
			break;

		case 10:
			if(Mode_e == OUT_IN)    {P1M1 &= ~(1<<GPIO_Pin_n);	P1M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P1M1 &= ~(1<<GPIO_Pin_n);	P1M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P1M1 |=  (1<<GPIO_Pin_n);	P1M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P1M1 |=  (1<<GPIO_Pin_n);	P1M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 20:
			if(Mode_e == OUT_IN)    {P4M1 &= ~(1<<GPIO_Pin_n);	P4M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P4M1 &= ~(1<<GPIO_Pin_n);	P4M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P4M1 |=  (1<<GPIO_Pin_n);	P4M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P4M1 |=  (1<<GPIO_Pin_n);	P4M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 30:
			if(Mode_e == OUT_IN)    {P3M1 &= ~(1<<GPIO_Pin_n);	P3M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P3M1 &= ~(1<<GPIO_Pin_n);	P3M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P3M1 |=  (1<<GPIO_Pin_n);	P3M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P3M1 |=  (1<<GPIO_Pin_n);	P3M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 4:
			if(Mode_e == OUT_IN)    {P4M1 &= ~(1<<GPIO_Pin_n);	P4M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P4M1 &= ~(1<<GPIO_Pin_n);	P4M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P4M1 |=  (1<<GPIO_Pin_n);	P4M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P4M1 |=  (1<<GPIO_Pin_n);	P4M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 50:
			if(Mode_e == OUT_IN)    {P5M1 &= ~(1<<GPIO_Pin_n);	P5M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P5M1 &= ~(1<<GPIO_Pin_n);	P5M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P5M1 |=  (1<<GPIO_Pin_n);	P5M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P5M1 |=  (1<<GPIO_Pin_n);	P5M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 60:
			if(Mode_e == OUT_IN)    {P6M1 &= ~(1<<GPIO_Pin_n);	P6M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P6M1 &= ~(1<<GPIO_Pin_n);	P6M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P6M1 |=  (1<<GPIO_Pin_n);	P6M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P6M1 |=  (1<<GPIO_Pin_n);	P6M0 |=  (1<<GPIO_Pin_n);}
			break;

		case 70:
			if(Mode_e == OUT_IN)    {P7M1 &= ~(1<<GPIO_Pin_n);	P7M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == OUT_PP)    {P7M1 &= ~(1<<GPIO_Pin_n);	P7M0 |=  (1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P7M1 |=  (1<<GPIO_Pin_n);	P7M0 &= ~(1<<GPIO_Pin_n);}
			if(Mode_e == HighZ_IN)  {P7M1 |=  (1<<GPIO_Pin_n);	P7M0 |=  (1<<GPIO_Pin_n);}
			break;
	}

}

u8 GPIO_Read_Pin(u8 GPIO_PinS)
{
    return GPIO_PinS;
}

/*************************************************************************
*  函数名称：void GPIO_WritePin(GPIO_Pin_e Pin_x, bit sta)
*  功能说明：GPIO状态写
*  参数说明：pin   ： 管脚标号    lq_gpio.h中定义
*           sta   ： 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2025年01月03日
*  举    GPIO_WritePin(P0_0, SET);  //P0_0设置为高电平
*************************************************************************/
void GPIO_WritePin(GPIO_Pin_e Pin_x, bit sta)
{
	switch(Pin_x&0xF0)
	{
		case P0_0:(sta == SET)?(P0|=(1<<(Pin_x&0x0F))):(P0&=~(1<<(Pin_x&0x0F))); break;
		case P1_0:(sta == SET)?(P1|=(1<<(Pin_x&0x0F))):(P1&=~(1<<(Pin_x&0x0F))); break;
		case P2_0:(sta == SET)?(P2|=(1<<(Pin_x&0x0F))):(P2&=~(1<<(Pin_x&0x0F))); break;
		case P3_0:(sta == SET)?(P3|=(1<<(Pin_x&0x0F))):(P3&=~(1<<(Pin_x&0x0F))); break;
		case P4_0:(sta == SET)?(P4|=(1<<(Pin_x&0x0F))):(P4&=~(1<<(Pin_x&0x0F))); break;
		case P5_0:(sta == SET)?(P5|=(1<<(Pin_x&0x0F))):(P5&=~(1<<(Pin_x&0x0F))); break;
		case P6_0:(sta == SET)?(P6|=(1<<(Pin_x&0x0F))):(P6&=~(1<<(Pin_x&0x0F))); break;
		case P7_0:(sta == SET)?(P7|=(1<<(Pin_x&0x0F))):(P7&=~(1<<(Pin_x&0x0F))); break;
	}
}
/*************************************************************************
*  函数名称：bit GPIO_ReadPin(GPIO_Pin_e Pin_x)
*  功能说明：GPIO状态读
*  参数说明：pin   ： 管脚标号    lq_gpio.h中定义
*          
*  函数返回：1：高电平  0：低电平
*  修改时间：2025年01月03日
*  举    GPIO_ReadPin(P0_0);  //读取P0_0管脚的电平
*************************************************************************/
bit GPIO_ReadPin(GPIO_Pin_e Pin_x)
{
	switch(Pin_x&0xF0)
	{
		case P0_0: return (P0&(1<<(Pin_x&0x0F)))?1:0;
		case P1_0: return (P1&(1<<(Pin_x&0x0F)))?1:0;
		case P2_0: return (P2&(1<<(Pin_x&0x0F)))?1:0;
		case P3_0: return (P3&(1<<(Pin_x&0x0F)))?1:0;
		case P4_0: return (P4&(1<<(Pin_x&0x0F)))?1:0;
		case P5_0: return (P5&(1<<(Pin_x&0x0F)))?1:0;
		case P6_0: return (P6&(1<<(Pin_x&0x0F)))?1:0;
		case P7_0: return (P7&(1<<(Pin_x&0x0F)))?1:0;
	}
	return 0;		//若一直返回0，可能管脚出现问题
}




