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

#ifndef __LED_H_
#define __LED_H_


#include "config.h"

//-------------------------宏定义LED管脚_ 切记与LED_Init()初始化保持一致-------------------------
#define LED0p       P01       //宏定义LED0 连接的管脚
#define LED1p       P01
#define Beep        P35       //宏定义LED1 连接的管脚    --蜂鸣器


typedef enum
{
  LED0=0,               //在APP_LED.c初始化为 P01，对应控制
  LED1=1,                 //空
  Beep0=2,
  LED_ALL,
}LED_n;

//枚举LED状态
typedef enum
{
  ON=0,                 //亮
  OFF=1,                //灭
  RVS=2,                //翻转
}LED_s;


//-------------------------用户函数声明-------------------------

void GPIO_LED_Init(void);
void LED_Ctrl(LED_n ledp, LED_s sta);
void Test_GPIO_LED(void);


#endif  /*__LED_H_*/
