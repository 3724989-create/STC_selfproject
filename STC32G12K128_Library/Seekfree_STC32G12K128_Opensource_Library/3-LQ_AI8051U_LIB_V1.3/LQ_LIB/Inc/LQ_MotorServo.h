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

#ifndef __LQ_MS_H_
#define __LQ_MS_H_

#include "config.h"


#define Motor_Dir1      P51         //电机驱动方向控制IO1,宏定义，若修改需要同步修改MotorInit()中的初始化
#define Motor_Dir2      P53         //电机驱动方向控制IO2,宏定义，若修改需要同步修改MotorInit()中的初始化

#define USEDRV8701                  //使用不同的驱动，定义不同的宏定义
//#define USE7971                   //使用不同的驱动，定义不同的宏定义

#define Motor_FREQ      12500       //定义电机PWM的频率 12.5K Hz
#define Servo_FREQ      50          //定义舵机和无感无刷驱动控制信号的PWM频率
#define Servo_Median    1500        //定义舵机 中值PWM占空比时间
#define Servo1          0           //舵机编号
#define Servo2          1           //舵机编号


void Motor_Init(u16 freq);                   //电机PWM及方向IO初始化
void Servo_Init(u16 freq);
void BLmotor_Init(u16 freq);


void Motor_Ctrl(int16 Motor1,int16 Motor2);  //电机驱动控函数声明
void Servo_Ctrl(u8 Sx, u16 Sduty);
void BLmotor_Ctrl(u16 BLmotor1,u16 BLmotor2);


void Test_Motor (void);
void Test_Servo(void);
void Test_BLSmotor(void);

#endif
