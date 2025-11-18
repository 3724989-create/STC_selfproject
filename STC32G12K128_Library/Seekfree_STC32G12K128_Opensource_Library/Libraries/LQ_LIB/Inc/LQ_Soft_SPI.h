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
#ifndef __LQ_Soft_SPI_H_
#define __LQ_Soft_SPI_H_

#include "include.h"

#define Soft_SPI_PORT           GPIO_P4
//#define Soft_SPI_SCK_Pin        GPIO_Pin_3      //Soft_SPI_Init()直接初始化P4端口低四位 为准双向模式
//#define Soft_SPI_MISO_Pin       GPIO_Pin_2
//#define Soft_SPI_MOSI_Pin       GPIO_Pin_1
//#define Soft_SPI_CS_Pin         GPIO_Pin_0
#define Soft_SPI_Pin           GPIO_Pin_LOW        //Soft_SPI_SCK_Pin|Soft_SPI_MOSI_Pin|Soft_SPI_MISO_Pin|Soft_SPI_CS_Pin;

//宏定义软件SPI管脚
#define SPI_SCK      P43      //SCK
#define SPI_MISO     P42      //单片机MISO接模块--SDO
#define SPI_MOSI     P41      //单片机MOSI接模块--SDI
#define SPI_CS       P40      //CS

void Soft_SPI_Init(void);

void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u16 len);

void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);

void LQ_SPI_Write(unsigned char reg, unsigned char value);

#endif
