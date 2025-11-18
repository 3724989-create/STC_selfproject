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

#ifndef _LQOLED_H
#define _LQOLED_H


void OLED_Init(void);
void OLED_Fill(void);
void OLED_CLS(void);
void OLED_PutPixel(unsigned char x,unsigned char y);
void OLED_ClrDot(unsigned char x,unsigned char y);
void OLED_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char gif);
void OLED_P6x8Str(unsigned char x,unsigned char y, char ch[]);
void OLED_P8x16Str(unsigned char x,unsigned char y, char ch[]);
void OLED_P14x16Str(unsigned char x,unsigned char y,char ch[]);
void OLED_Show_BMP(unsigned char x0,unsigned char y0,unsigned char wide,unsigned char high,const unsigned char * bmp);
void OLED_Show_LQLogo(void);
void OLED_Show_Frame80(void);
void OLED_Show_Frame94(void);
void OLED_Road(unsigned short high, unsigned short wide, unsigned char *Pixle);
void OLED_WrCmd(unsigned char cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void Test_OLED(void);
void OLED_WrDat(unsigned char dat);
void OLED_Set_Pos(unsigned char x, unsigned char y);



#endif


