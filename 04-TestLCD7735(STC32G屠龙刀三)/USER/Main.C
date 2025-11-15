//===========================================================
// OLED 1.8" 彩色显示屏 160*128 实验程序
// 驱动: ST7735 
// 基于: STC32G12K128芯片实验板编程 屠龙刀三核心实验板 
// 整理:编程: 浦晓明(浦江一水) For 国芯论坛 2024-06-04 PXM
//===========================================================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "STC32G.h"
#include "LCD_7735.H"
#include "PIC.H"

unsigned char k;
char S[20];

void Delay1ms(void)	//@22.1184MHz
{ unsigned char data i, j;
//_nop_();_nop_();
	i = 22;	j = 128;
	do
	{	while (--j);
	} while (--i);
}
void Delayms(unsigned int count)
{ unsigned int i;
  for(i=0;i<count;i++)       
  Delay1ms();             
}
//主程序入口
void main(void)
{ unsigned char i;
  unsigned int wait=0;
  //IO端口初始化
  WTST  = 0; //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
  EAXFR = 1; //扩展寄存器(XFR)访问使能
  CKCON = 0; //提高访问XRAM速度
  //IO端口初始化//全部为双向口
  P0M1 = 0x00;   P0M0 = 0x04;   //设置为准双向口: P0.2推挽输出控制背光
  P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
  P2M1 = 0x00;   P2M0 = 0x00;   //设置为准双向口
  P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
  P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
  P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
  LCD_Init();
  while(1)
  {  
    LCD_CLS(1);             //蓝色清屏
	  LCD_Point(5,5,15);      //画点测试 
	  LCD_LineH(5,8,50,10);   //画水平线
	  LCD_LineV(5,8,50,10);   //画垂直线
	  LCD_Line(5,8,55,58,12); //两点一线
	  LCD_Box(10,95,100,28,15,0,4);         //画矩形方框(可指定边框色和填充色)
	  LCD_A08(20,15,"A08_012345",15,1,10);  //显示6*8字符串
	  LCD_A12(20,30,"A12_012345",15,1,10);  //显示8*12字符串
	  LCD_A16(20,45,"A16_012345",15,1,10);  //显示8*16字符串
	  LCD_A24(20,64,"A24_0123",15,1,8);     //显示12*24字符串
	  LCD_H16(24,102,"单片机STC",14,4);     //16点阵中西文混合显示字符串 
	  LCD_H24( 8,130,"单片机STC",12,1);     //24点阵中西文混合显示字符串
    Delayms(2000); 
	  LCD_Box(0,130,127,28,1,1,1);          //局部清屏
    LCD_BMP( 0, 5,64,64,P_SET);           //彩色图标显示
    LCD_BMP(64, 5,64,64,P_ZER);
    LCD_BMP( 0,69,64,64,P_XSW);
    LCD_BMP(64,69,64,64,P_SYS);
	  LCD_A16(0,140,"LCD_ST7735_DEMO.",10,0,16);
    Delayms(2000);
    LCD_CLS(0);
    for(i=0;i<16;i++)LCD_Box(0, i*10,63,10,7,7,i);          //显示彩色条(色号0..15)
    for(i=0;i<16;i++)LCD_Box(64,i*10,63,10,7,7,(unsigned char)(i+16)); //显示彩色条(色号16..31)
    Delayms(2000);          //
  }
}
