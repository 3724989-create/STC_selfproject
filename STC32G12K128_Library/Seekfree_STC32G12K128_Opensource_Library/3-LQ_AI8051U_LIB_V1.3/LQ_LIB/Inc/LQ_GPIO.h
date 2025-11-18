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

#ifndef __LQ_GPIO_H_
#define __LQ_GPIO_H_

#include "include.h"

typedef enum
{
	P0_0 = 0x00, P0_1, P0_2, P0_3, P0_4, P0_5, P0_6, P0_7,
	P1_0 = 0x10, P1_1, P1_2, P1_3, P1_4, P1_5, P1_6, P1_7,
	P2_0 = 0x20, P2_1, P2_2, P2_3, P2_4, P2_5, P2_6, P2_7,
	P3_0 = 0x30, P3_1, P3_2, P3_3, P3_4, P3_5, P3_6, P3_7,
	P4_0 = 0x40, P4_1, P4_2, P4_3, P4_4, P4_5, P4_6, P4_7,
	P5_0 = 0x50, P5_1, P5_2, P5_3, P5_4, P5_5, P5_6, P5_7,
	P6_0 = 0x60, P6_1, P6_2, P6_3, P6_4, P6_5, P6_6, P6_7,
	P7_0 = 0x70, P7_1, P7_2, P7_3, P7_4, P7_5, P7_6, P7_7,

}GPIO_Pin_e;

typedef enum
{
	OUT_IN   = 0,			//准双向IO
    OUT_PP   = 1,			//推挽输出
	HighZ_IN = 2,		    //高阻态，浮空（电流既不能流入也不能流出）
	OUT_OD   = 3,			//开漏输出

}GPIO_Mode_e;


typedef struct
{
	u8	GPIO_Modes;		//IO模式
	u8	GPIO_Pins;		//要设置的端口
    u8  Stas;           //管脚状态      SET ,RESET   2024-11-21 添加
} GPIO_InitTypeDefs;

// u8	GPIO_Inits(u8 GPIO_Px, GPIO_InitTypeDefs *GPIO_Tpydefx);  此法代码量多，不建议使用

void GPIO_InitConfig(GPIO_Pin_e Pin_x, GPIO_Mode_e Mode_n);
void GPIO_WritePin(GPIO_Pin_e Pin_x, bit sta);
bit GPIO_ReadPin(GPIO_Pin_e Pin_x);

//--------------------------------一下直接替换P00方式操作，执行消耗时间短------------------------------------------
////I/O电平读取，使用宏函数  也可以直接读取
////eg:GPIO_Write_Pin(P42,RESET); //将P45设为低电平
#define GPIO_Write_Pin(GPIO_PinS,Status)    (GPIO_PinS=Status)
u8 GPIO_Read_Pin(u8 GPIO_PinS);

#define GPIO_SetBits(GPIO_PinS)             (GPIO_PinS=SET)
#define GPIO_ResetBits(GPIO_PinS)           (GPIO_PinS=RESET)


#endif

