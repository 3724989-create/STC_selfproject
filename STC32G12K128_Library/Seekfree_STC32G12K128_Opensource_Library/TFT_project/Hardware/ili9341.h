#ifndef __ILI9341_H
#define __ILI9341_H

#include "bsp.h" // 假设的STC32G寄存器定义文件

// --- 1. 屏幕尺寸定义 ---
#define LCD_W 240
#define LCD_H 320

// --- 2. 颜色定义 (RGB565格式) ---
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0

// --- 3. 硬件引脚配置 (根据您的接线修改) ---
// 假设 CS, DC, RST 都是连接在 P1 组
#define LCD_CS_PORT     P1
#define LCD_DC_PORT     P1
#define LCD_RST_PORT    P1

#define LCD_CS_PIN      P10 // P1.0 作为 CS
#define LCD_DC_PIN      P11 // P1.1 作为 DC (A0)
#define LCD_RST_PIN     P13 // P1.3 作为 RST

// CS操作宏
#define LCD_CS_CLR      (LCD_CS_PIN = 0) // CS拉低：选中
#define LCD_CS_SET      (LCD_CS_PIN = 1) // CS拉高：去选中

// DC操作宏
#define LCD_DC_CLR      (LCD_DC_PIN = 0) // DC拉低：命令
#define LCD_DC_SET      (LCD_DC_PIN = 1) // DC拉高：数据

// RST操作宏
#define LCD_RST_CLR     (LCD_RST_PIN = 0) // RST拉低：复位
#define LCD_RST_SET     (LCD_RST_PIN = 1) // RST拉高：正常

// --- 4. 函数声明 ---

// 底层通信
void self_SPI_Init(void);
void LCD_Writ_Bus(unsigned char dat);
void LCD_WR_REG(unsigned char cmd);
void LCD_WR_DATA(unsigned char dat);

// 核心驱动
void LCD_RST(void);
void LCD_Init(void);
void LCD_SetCursor(unsigned int Xstart, unsigned int Ystart);
void LCD_SetWindows(unsigned int Xstart, unsigned int Ystart, unsigned int Xend, unsigned int Yend);

// 绘图函数
void LCD_Clear(unsigned int Color);
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color);
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);

#endif