//=================================================================================================
// 文件名称: LCD_ILI9341.H
// 功能说明: 液晶屏模块驱动程序  2.4寸彩色显示屏 240*320 8位并行口模式
// 基于电路: STC32G 实验箱V9.62
// 整理改编: 浦晓明(浦江一水) 2024-08-28...
//=== 硬件连接 ====================================================================================
//=========================================电源接线================================================//
//VDD接DC 3.3V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块数据总线类型为8位并口
//液晶屏模块              STC32G 
// LCD_D0~D7     接       P60~P67        //液晶屏8位数据线
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块              STC32G
// LCD_RS        接        P45           //数据/命令选择控制信号
// LCD_RD        接        P44           //读控制信号
// LCD_RST       接        P43           //复位信号
// LCD_WR        接        P42           //写控制信号
// LCD_CS        接        P34           //片选控制信号
//================================================================================================//
// 本实验测试使用的2.4寸320*240的并口彩屏，是在淘宝店 https://kcer001.taobao.com/ 提供的样品彩屏上测试通过的
//**************************************************************************************************/
#ifndef __LCD_ILI9341_H
#define __LCD_ILI9341_H		
#include "STC32G_SYS.H"

/////////////////////////////////////用户配置区////////////////////////////////////////////	 
//支持横竖屏快速定义切换
#define LCD_DIR  0     //液晶屏顺时针旋转方向0:0度旋转，1:90度旋转，2:180度旋转，3:270度旋转
#define USE_LCM  1     //1:使用LCM模式 0：模拟SPI模式
#define USE_DMA  1     //1:使用LCM+DMA模式 0：模拟SPI模式
///////////////////////////////////////////////////////////////////////////////////////////
extern const unsigned COLOR[]; //RGB彩色表
//IO连接
#define  LCD_Port  P6   //高8位数据口,8位模式下只使用高8位 
sbit LCD_RS = P4^5;  		//数据/命令切换
sbit LCD_WR = P4^2;		  //写控制
sbit LCD_RD = P4^4;		  //读控制
sbit LCD_CS = P3^4;		  //片选	
sbit LCD_RST= P4^3;	    //复位 

//LCD函数说明
void LCM_Config(void);  //LCM模式初始化
void DMA_Config(void);  //DMA模式初始化
void LCD_Init(void);        //初始化
void LCD_CLS(u8 Color);     //指定色清屏
void LCD_WR_CMD(u8 CMD);    //写命令 
void LCD_WR_DAT(u8 DAT);    //写数据
void LCD_SetWindow(u16 SX,u16 SY,u16 EX,u16 EY);//设置显示窗口
void LCD_SetDIR(u8 dir);                        //设置显示方向
void LCD_SetXY(u16 X, u16 Y);                   //指定坐标 
void LCD_Point(u16 X,u16 Y,u8 Color);           //画点
void LCD_LineH(u16 X, u16 Y,u16 W,u8 Color);    //画水平线
void LCD_LineV(u16 X, u16 Y,u16 H,u8 Color);    //画垂直线
void LCD_Line(u16 X0, u16 Y0,u16 X1,u16 Y1,u8 Color);    //两点一直线
void LCD_LineH0(u16 X, u16 Y,u16 L,u8 W,u8 Fc,u8 Bc);    //画水平虚线
void LCD_LineV0(u16 X, u16 Y,u16 L,u8 W,u8 Fc,u8 Bc);    //画垂直虚线
void LCD_Box(u16 X,u16 Y,u16 W,u16 H,u8 C1,u8 C2,u8 BC); //画填色方框
void LCD_Box0(u16 X, u16 Y,u16 W,u16 H,u8 mode,u8 Color);//画一方框
void LCD_BoxF(u16 X, u16 Y,u16 W,u16 H,u8 mode,u8 Color);//填充一圆角框
void LCD_BMP(u16 X,u16 Y,u8 W,u8 H,u8* pic);    //显示BMP图片
void LCD_A08(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc);    //显示单个6*8字符
void LCD_A12(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc);    //显示单个8*12字符
void LCD_A16(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc);    //显示单个8*16字符 
void LCD_A24(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc);    //显示单个12*24字符 
void LCD_HZ16(u16 X,u16 Y,u8 *Hz,u8 Fc,u8 Bc);  //显示单个16点阵汉字
void LCD_HZ24(u16 X,u16 Y,u8 *Hz,u8 Fc,u8 Bc);  //显示单个24点阵汉字
void LCD_Str8(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc); //显示6*8字符串 
void LCD_Str16(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc);//显示16*16字符串 
void LCD_Str24(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc);//显示16*16字符串 
//u16  Color_To_565(u8 r, u8 g, u8 b);
	 
#endif  
	 
	 



