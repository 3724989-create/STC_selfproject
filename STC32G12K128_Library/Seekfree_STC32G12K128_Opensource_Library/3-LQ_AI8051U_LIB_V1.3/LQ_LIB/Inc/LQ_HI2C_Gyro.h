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
#ifndef __LQ_HI2C_Gyro_H
#define __LQ_HI2C_Gyro_H

#include "include.h"

typedef enum
{
	ICM20602 = 0,
	ICM20689,
	MPU9250,
	ICM42605,
	MPU6050,
	errorid
}Dev;

void Test_HardIIC_Gyro(void);		//陀螺仪加速度计测试函数
unsigned char Get_Slave_ID(void);
unsigned char Gyro_Init(void);	//初始化陀螺仪加速度计



unsigned char Hard_IIC_Read_MPU_Byte(unsigned char addr,unsigned char reg);
unsigned char Hard_IIC_Write_MPU_Byte(unsigned char addr,unsigned char reg, unsigned char value);


#endif




















