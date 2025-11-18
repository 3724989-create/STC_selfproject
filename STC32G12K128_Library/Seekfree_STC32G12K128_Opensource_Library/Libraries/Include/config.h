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
 【SYS CLK】 MAIN_Fosc 使用内部晶振
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef		__TYPE_DEF_H
#define		__TYPE_DEF_H

#include "STC_AI8051U_X.h"
#include <stdlib.h>
#include <stdio.h>



//宏可设置主时钟频率

/*********************************************************/

// #define MAIN_Fosc		40000000L	//定义主时钟频率     //使用40M 作为系统主频

#define MAIN_Fosc		42000000L	//定义主时钟频率     //使用42M 作为系统主频


/*********************************************************/


//========================================================================
//                               类型定义
//========================================================================

typedef unsigned char   u8;         //  8 bits
typedef unsigned int    u16;        // 16 bits
typedef unsigned long   u32;        // 32 bits

typedef signed char     s8;         //  8 bits
typedef signed int      s16;        // 16 bits
typedef signed long     s32;        // 32 bits

typedef signed char     int8;       //  8 bits
typedef signed int      int16;      // 16 bits
typedef signed long     int32;      // 32 bits
typedef signed char     int8_t;     //  8 bits
typedef signed int      int16_t;    // 16 bits
typedef signed long     int32_t;    // 32 bits

typedef unsigned char   uint8;      //  8 bits
typedef unsigned int    uint16;     // 16 bits
typedef unsigned long   uint32;     // 32 bits
typedef unsigned char   uint8_t;    //  8 bits
typedef unsigned int    uint16_t;   // 16 bits
typedef unsigned long   uint32_t;   // 32 bits



//========================================================================

#define	TRUE	            1
#define	FALSE	            0


#define SET		            1
#define RESET		        0

//========================================================================

#define NULLD               0    //类型不一样,避免标准库中的NULL冲突

//========================================================================

#define ENABLE		        1
#define DISABLE		        0

#define SUCCESS		        0
#define FAIL		        -1




//全局中断使能/失能设置
#define Global_IRQ_Enable()    (EA = 1)
#define Global_IRQ_Disable()   (EA = 0)

//========================================================================
#define USE_OLED                   //全局使用OLED显示
//#define USE_LCD                    //全局使用LCD 显示

#endif
