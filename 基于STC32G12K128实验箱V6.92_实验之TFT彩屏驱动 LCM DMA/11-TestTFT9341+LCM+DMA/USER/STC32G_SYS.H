//********************************************************************
// 文件名称: STC32G_SYS.H 
// 功能说明: STC32G 系统基础头文件: 定义主时钟/数据类型/系统初始化...
// 整理编程: 浦晓明/浦江一水 2023-12-09  (参考STC资料)
//********************************************************************
#ifndef		__STC32G_SYS_H
#define		__STC32G_SYS_H

//**** 数据类型 ******************************************
typedef unsigned char   u8;     //  8 bits 无符号
typedef unsigned int    u16;    // 16 bits 
typedef unsigned long   u32;    // 32 bits 
typedef unsigned char   uint8;  //  8 bits 
typedef unsigned int    uint16; // 16 bits 
typedef unsigned long   uint32; // 32 bits
typedef signed char     int8;   //  8 bits 有符号
typedef signed int      int16;  // 16 bits 
typedef signed long     int32;  // 32 bits

//***** 主时钟定义:仿真设置需要与此一致 *********************
//#define MAIN_Fosc		22118400L	//定义主时钟
//#define MAIN_Fosc		12000000L	//定义主时钟
//#define MAIN_Fosc		11059200L	//定义主时钟
//#define MAIN_Fosc		 5529600L	//定义主时钟
//#define MAIN_Fosc		24000000L	//定义主时钟
#define MAIN_Fosc		  35000000L	//定义主时钟

//***** 必要标准头文件 ***************************************
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC32G.H"
#include "intrins.h"

//***** 基础函数说明 *****************************************
void SYS_Init(void);   //系统初始化
void delay_ms(int count);
void delay_us(int count);

#endif
