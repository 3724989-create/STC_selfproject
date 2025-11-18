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
#ifndef __LQ_XINT_H
#define __LQ_XINT_H

#include "config.h"

//--------------------------------定义变量-----------------------------------
//      7           6          5           4            3           2           1           0     --》
//  10000000    010000000   00100000    00010000    00001000    00000100    00000010    00000001  --》1<<Pin
//    0x80        0x40        0x20        0x10        0x08        0x04        0x02        0x01    --》位 Pin   管脚
//GPIO端口中断使能配置   24-11-18 编写
//#define  P0_ENABLE_IOINT(Pin)	        {P0INTE |= (Pin);}  //1使能
//#define  P1_ENABLE_IOINT(Pin)	        {P1INTE |= (Pin);}  //1使能
//#define  P2_ENABLE_IOINT(Pin)	        {P2INTE |= (Pin);}  //1使能
//#define  P3_ENABLE_IOINT(Pin)	        {P3INTE |= (Pin);}  //1使能
//#define  P4_ENABLE_IOINT(Pin)	        {P4INTE |= (Pin);}  //1使能
//#define  P5_ENABLE_IOINT(Pin)	        {P5INTE |= (Pin);}  //1使能
//#define  P6_ENABLE_IOINT(Pin)	        {P6INTE |= (Pin);}  //1使能
//#define  P7_ENABLE_IOINT(Pin)	        {P7INTE |= (Pin);}  //1使能


//#define  P0_DISENABLE_IOINT(Pin)	    {P0INTE &= ~(Pin);}  //0失能
//#define  P1_DISENABLE_IOINT(Pin)	    {P1INTE &= ~(Pin);}  //0失能
//#define  P2_DISENABLE_IOINT(Pin)	    {P2INTE &= ~(Pin);}  //0失能
//#define  P3_DISENABLE_IOINT(Pin)	    {P3INTE &= ~(Pin);}  //0失能
//#define  P4_DISENABLE_IOINT(Pin)	    {P4INTE &= ~(Pin);}  //0失能
//#define  P5_DISENABLE_IOINT(Pin)	    {P5INTE &= ~(Pin);}  //0失能
//#define  P6_DISENABLE_IOINT(Pin)	    {P6INTE &= ~(Pin);}  //0失能
//#define  P7_DISENABLE_IOINT(Pin)	    {P7INTE &= ~(Pin);}  //0失能

/*
//端口中断优先级设置
#define  PO_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x01), PINIPL &= ~(0x01); \
                                 if(n == 1) PINIPH &= ~(0x01), PINIPL |=  (0x01); \
								 if(n == 2) PINIPH |=  (0x01), PINIPL &= ~(0x01); \
								 if(n == 3) PINIPH |=  (0x01), PINIPL |=  (0x01); \
								}while(0)

#define  P1_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x02), PINIPL &= ~(0x02); \
                                 if(n == 1) PINIPH &= ~(0x02), PINIPL |=  (0x02); \
								 if(n == 2) PINIPH |=  (0x02), PINIPL &= ~(0x02); \
								 if(n == 3) PINIPH |=  (0x02), PINIPL |=  (0x02); \
								}while(0)

#define  P2_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x04), PINIPL &= ~(0x04); \
                                 if(n == 1) PINIPH &= ~(0x04), PINIPL |=  (0x04); \
								 if(n == 2) PINIPH |=  (0x04), PINIPL &= ~(0x04); \
								 if(n == 3) PINIPH |=  (0x04), PINIPL |=  (0x04); \
								}while(0)

#define  P3_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x08), PINIPL &= ~(0x08); \
                                 if(n == 1) PINIPH &= ~(0x08), PINIPL |=  (0x08); \
								 if(n == 2) PINIPH |=  (0x08), PINIPL &= ~(0x08); \
								 if(n == 3) PINIPH |=  (0x08), PINIPL |=  (0x08); \
								}while(0)
#define  P4_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x10), PINIPL &= ~(0x10); \
                                 if(n == 1) PINIPH &= ~(0x10), PINIPL |=  (0x10); \
								 if(n == 2) PINIPH |=  (0x10), PINIPL &= ~(0x10); \
								 if(n == 3) PINIPH |=  (0x10), PINIPL |=  (0x10); \
								}while(0)
#define  P5_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x20), PINIPL &= ~(0x20); \
                                 if(n == 1) PINIPH &= ~(0x20), PINIPL |=  (0x20); \
								 if(n == 2) PINIPH |=  (0x20), PINIPL &= ~(0x20); \
								 if(n == 3) PINIPH |=  (0x20), PINIPL |=  (0x20); \
								}while(0)
#define  P6_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x40), PINIPL &= ~(0x40); \
                                 if(n == 1) PINIPH &= ~(0x40), PINIPL |=  (0x40); \
								 if(n == 2) PINIPH |=  (0x40), PINIPL &= ~(0x40); \
								 if(n == 3) PINIPH |=  (0x40), PINIPL |=  (0x40); \
								}while(0)
#define  P7_INT_Priority(n)	 do{ if(n == 0) PINIPH &= ~(0x80), PINIPL &= ~(0x80); \
                                 if(n == 1) PINIPH &= ~(0x80), PINIPL |=  (0x80); \
								 if(n == 2) PINIPH |=  (0x80), PINIPL &= ~(0x80); \
								 if(n == 3) PINIPH |=  (0x80), PINIPL |=  (0x80); \
								}while(0)

*/


#define	PxINT_MODE_Fall		0	//下降沿中断
#define	PxINT_MODE_Rise     1	//上升沿中断
#define	PxINT_MODE_LOW		2	//低电平中断
#define	PxINT_MODE_HIGH     3	//高电平中断

u8 GPIO_INT_InitE(u8 GPIO, GPIO_InitTypeDef *GPIOx);      //此法孔家开销大，暂未完善

//IO中断用户函数
void GPIO_PxINT_Init(void);
void Test_GPIO_PxINT(void);         //GPIO中断


//--------------------------------------------------------------
// 外部中断用户函数
void Exti_Init(u8 ExInt_e);
void Test_ExINT(void);      //传统外部中断测试

#endif

