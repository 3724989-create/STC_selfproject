//========================================================
// TFT 1.8" 彩色显示屏 160*128
// 驱动:ST7735 
// 基于:STC32G12K128芯片 屠龙刀三核心实验板
// 编程:浦晓明 2024-06-01 PXM
//========================================================
#include "bsp.h"

#ifndef __LCD_7735_H_
#define __LCD_7735_H_

//========================================================
//函数说明
void LCD_Init(void);                       //显示屏初始化 
void LCD_CLS(unsigned char Bcolor);        //清屏函数.调用参数:背景色
void LCD_SetBox(unsigned char X,unsigned char Y,unsigned char W,unsigned char H);     //设置窗口,XY坐标WH宽高
void LCD_MoveTo(unsigned int X,unsigned int Y);                                       /* 移动点坐标(转换成显存地址) */
void LCD_Point(unsigned int X,unsigned int Y,unsigned char Color);                    /* 单独画彩色一点函数 */
void LCD_Box(unsigned int X,unsigned int Y,unsigned int H,unsigned int V,unsigned char C1,unsigned char C2,unsigned char BC);  /* 画彩色矩形方框函数 */
void LCD_LineH(unsigned int X,unsigned int Y,unsigned int H,unsigned char Color);     /* 画水平彩色直线函数 */
void LCD_LineV(unsigned int X,unsigned int Y,unsigned int V,unsigned char Color);     /* 画垂直彩色直线函数 */
void LCD_Line(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2,unsigned char Color); /* 任意两点画直线函数 */
void LCD_A08(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len); /* 显示单个6*8点阵西文字符串 */
void LCD_A12(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len); /* 12点阵西字符串显示   */
void LCD_A16(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len); /* 16点阵西文字符串显示 */
void LCD_A24(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len); /* 24点阵西文字符串显示 */
void LCD_H16(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor); /* 16点阵中西文混合字符串显示 */
void LCD_H24(unsigned int X,unsigned int Y,char *str,unsigned char Fcolor,unsigned char Bcolor); /* 24点阵中西文混合字符串显示 */
void LCD_BMP(unsigned char X,unsigned char Y,unsigned char W,unsigned char H,unsigned char * pic); //显示BMP图片

#endif
//========================================================
