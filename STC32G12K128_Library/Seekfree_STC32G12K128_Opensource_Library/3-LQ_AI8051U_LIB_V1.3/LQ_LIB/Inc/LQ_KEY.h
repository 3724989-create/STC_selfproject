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

#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

#include "config.h"



//-------------------------宏定义KEY管脚_ 切记与KEY_Init()初始化保持一致-------------------------

#define KEY0p       P27              //母板上按键0
#define KEY1p       P45              //母板上按键1
#define KEY2p       P46              //母板上按键2

#define KEY3p        //母板上按键3（无此按键）

#define DSW0p       P56              //母板上拨码开关SW1
#define DSW1p       P57              //母板上拨码开关SW2

#define Reed_Tubep  P33              //母板上干簧管,建议使用外部中断读取，此处仅作演示


//定义模块号
typedef enum
{
    KEY0 = 0,  	            //母板上按键0
    KEY1 = 1,		        //母板上按键1
    KEY2 = 2,		        //母板上按键2
	KEY3 = 3,		        //无
    DSW0 = 4,               //母板上拨码开关1
    DSW1 = 5,		        //母板上拨码开关2
    Reed_Tube=6,            //干簧管 接口
} KEYn_e;

//定义状态
typedef enum
{
    LOW = 0,  		        //按下
    HIGH = 1, 		        //松开
    FAILED = 0xFF, 	        //错误
} KEYs_e;

typedef enum //
{
    NOKEYDOWN  = 0, 			//没有按键
	KEY0DOWN   = 0x01,  	    //母板上按键0
    KEY1DOWN   = 0x02,		    //母板上按键1
    KEY2DOWN   = 0x04,		    //母板上按键2
    KEY01DOWN  = 0x03,  	    //母板上按键0&1
    KEY02DOWN  = 0x05,		    //母板上按键0&2
    KEY12DOWN  = 0x06,		    //母板上按键1&2
    KEY012DOWN = 0x07,	        //母板上按键0&1&2
    KEYError   = 0xFF,		    //错误
} KEYdown_e;



void GPIO_KEY_Init (void);
unsigned char KEY_Read (KEYn_e KEYno);
unsigned char KEY_Read_All (void);
void Test_GPIO_KEY (void);
//void Test_ComKEY_DisPlay(void);

#endif



