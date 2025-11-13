#include "ili9341.h"

#define LCD_CTRL_PINS_MASK (0x01 | 0x02 | 0x08)

// --- 1. STC32G SPI 硬件初始化 ---
void self_SPI_Init(void)
{
// 1. 设置 P1M0 的对应位为 1 (完成推挽模式的 '1' 部分)
    P1M0 |= LCD_CTRL_PINS_MASK; 

// 2. 清除 P1M1 的对应位为 0 (完成推挽模式的 '0' 部分)
    P1M1 &= ~LCD_CTRL_PINS_MASK;

    // 2. 配置SPI引脚复用 (SCLK, MOSI, MISO)
    // P2.0 (SCLK) 和 P2.1 (MOSI) 设为推挽输出
    P2M0 |= (1<<0) | (1<<1); 
    P2M1 &= ~((1<<0) | (1<<1));

    // P2.2 (MISO) 设为浮空输入 (默认，但显式写出)
    P2M0 &= ~(1<<2);
    P2M1 &= ~(1<<2);
    
    // 2.1 配置 SPI 引脚复用 (使用 P_SW1 寄存器)
    // 目标: 选择 P2 组引脚 (SCLK:P2.0, MOSI:P2.1, MISO:P2.2, SS:P2.3)
    // 对应 P_SW1: SPI_S1=0 (位3), SPI_S0=1 (位2) -> 0x04
    
    // 步骤一：清除 SPI_S1 位 (P_SW1^3)
    P_SW1 &= ~(1<<3); 
    
    // 步骤二：设置 SPI_S0 位 (P_SW1^2)
    P_SW1 |= (1<<2);

    // 3. 配置SPI寄存器 (主模式, 模式0)
    // STC32G 硬件SPI寄存器：SPCTL, SPSTAT, SPDR

    // SPCTL 寄存器配置：
    // | SPIEN | MSTR | CPOL | CPHA | SPR1 | SPR0 |
    // |   1   |   1  |  0   |  0   |  0   |  0  |  (假设分频比为 Fosc/8)  Fosc表示主频

    SPCTL = 0x50; // 1010 0001 (SPI使能，主模式MSTR=1, CPOL=0, CPHA=0, Fosc/4)
    // 根据您的需求调整分频位 SPRx

    // 确保SPSTAT中的中断标志位被清除（通常通过读写操作）
    (void)SPSTAT;
    SPSTAT &= ~(0x40 | 0x80); // 清除 SPIF (传输完成) 和 WCOL (写冲突)
}

// --- 2. ILI9341 底层通信函数 ---

// SPI 发送一个字节数据
void LCD_Writ_Bus(unsigned char dat) 
{
    SPDAT = dat; // 写入数据
    // 等待传输完成(SPIF=1)
    while(!(SPSTAT & 0x80)); // 等待 SPIF 标志 (传输完成)
    SPSTAT &= ~0x80;          // 清除 SPIF 标志
}

// 写入命令 (DC=0)
void LCD_WR_REG(unsigned char cmd)
{
    LCD_CS_CLR; // 片选
    LCD_DC_CLR; // 命令模式
    LCD_Writ_Bus(cmd);
    LCD_CS_SET; // 取消片选
}

// 写入数据 (DC=1)
void LCD_WR_DATA(unsigned char dat)
{
    LCD_CS_CLR; // 片选
    LCD_DC_SET; // 数据模式
    LCD_Writ_Bus(dat);
    LCD_CS_SET; // 取消片选
}

// 写入16位数据 (用于颜色)
void LCD_WR_DATA_16Bit(unsigned int dat)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    // 写入高八位
    LCD_Writ_Bus(dat >> 8); 
    // 写入低八位
    LCD_Writ_Bus(dat & 0xFF);
    LCD_CS_SET;
}

// --- 3. 屏幕控制函数 ---

// 硬件复位
void LCD_RST(void)
{
    LCD_RST_CLR; // 拉低复位
    system_delay_ms(100);
    LCD_RST_SET; // 释放复位
    system_delay_ms(100);
}

// 设置光标地址
void LCD_SetCursor(unsigned int Xstart, unsigned int Ystart)
{
    // 设置列地址 (X)
    LCD_WR_REG(0x2A); 
    LCD_WR_DATA(Xstart >> 8); // X Start High
    LCD_WR_DATA(Xstart & 0xFF); // X Start Low
    LCD_WR_DATA(Xstart >> 8); // X End High (单点时Xend=Xstart)
    LCD_WR_DATA(Xstart & 0xFF); // X End Low

    // 设置行地址 (Y)
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(Ystart >> 8); // Y Start High
    LCD_WR_DATA(Ystart & 0xFF); // Y Start Low
    LCD_WR_DATA(Ystart >> 8); // Y End High
    LCD_WR_DATA(Ystart & 0xFF); // Y End Low

    // 准备写入 GRAM (0x2C)
    LCD_WR_REG(0x2C);
}

// 设置显示窗口
void LCD_SetWindows(unsigned int Xstart, unsigned int Ystart, unsigned int Xend, unsigned int Yend)
{
    // 设置列地址 (0x2A)
    LCD_WR_REG(0x2A); 
    LCD_WR_DATA(Xstart >> 8); 
    LCD_WR_DATA(Xstart & 0xFF);
    LCD_WR_DATA(Xend >> 8); 
    LCD_WR_DATA(Xend & 0xFF);

    // 设置行地址 (0x2B)
    LCD_WR_REG(0x2B);
    LCD_WR_DATA(Ystart >> 8);
    LCD_WR_DATA(Ystart & 0xFF);
    LCD_WR_DATA(Yend >> 8);
    LCD_WR_DATA(Yend & 0xFF);

    // 准备写入 GRAM
    LCD_WR_REG(0x2C);
}


// --- 4. ILI9341 初始化序列 ---
void LCD_Init(void)
{
    LCD_RST(); // 硬件复位

    // --- ILI9341 常用初始化代码序列 ---
    LCD_WR_REG(0xCF);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0xC1); 
    LCD_WR_DATA(0x30); 

    LCD_WR_REG(0xED);  
    LCD_WR_DATA(0x64); 
    LCD_WR_DATA(0x03); 
    LCD_WR_DATA(0x12); 
    LCD_WR_DATA(0x81); 

    // ... 省略其他数十条命令 (应参考ILI9341数据手册补全)

    // Power Control 1 (0xC0)
    LCD_WR_REG(0xC0);  
    LCD_WR_DATA(0x23);

    // Power Control 2 (0xC1)
    LCD_WR_REG(0xC1);  
    LCD_WR_DATA(0x10);

    // VCOM Control 1 (0xC5)
    LCD_WR_REG(0xC5);  
    LCD_WR_DATA(0x3E); 
    LCD_WR_DATA(0x28); 

    // VCOM Control 2 (0xC7)
    LCD_WR_REG(0xC7);  
    LCD_WR_DATA(0x86); 

    // Memory Access Control (0x36) - 设置方向
    // 0x48: 竖屏(PORTRAIT), 0x28: 横屏(LANDSCAPE)
    LCD_WR_REG(0x36);  
    LCD_WR_DATA(0x48); // 默认竖屏

    // Pixel Format Set (0x3A) - 16位颜色 (RGB565)
    LCD_WR_REG(0x3A);  
    LCD_WR_DATA(0x55); // 0x55 for 16bit / 0x66 for 18bit

    // Frame Rate Control (0xB1)
    LCD_WR_REG(0xB1);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x18); 

    // Display Function Control (0xB6)
    LCD_WR_REG(0xB6);  
    LCD_WR_DATA(0x08); 
    LCD_WR_DATA(0xA2); 
    LCD_WR_DATA(0x27);

    // Sleep Out (0x11)
    LCD_WR_REG(0x11);
    system_delay_ms(120);

    // Display ON (0x29)
    LCD_WR_REG(0x29); 

    // 默认清屏为黑色
    LCD_Clear(BLACK);
}


// --- 5. 绘图函数实现 ---

// 清屏
void LCD_Clear(unsigned int Color)
{
    unsigned int i, j;
    LCD_SetWindows(0, 0, LCD_W - 1, LCD_H - 1);
    
    LCD_CS_CLR;
    LCD_DC_SET; // 批量写入数据，保持DC高电平
    
    for(i = 0; i < LCD_H; i++)
    {
        for(j = 0; j < LCD_W; j++)
        {
            // 写入高八位
            LCD_Writ_Bus(Color >> 8); 
            // 写入低八位
            LCD_Writ_Bus(Color & 0xFF);
        }
    }
    LCD_CS_SET;
}

// 画点
void LCD_DrawPoint(unsigned int x, unsigned int y, unsigned int color)
{
    if(x >= LCD_W || y >= LCD_H) return;
    
    // 设置单个点的窗口
    LCD_SetWindows(x, y, x, y);
    // 写入16位颜色
    LCD_WR_DATA_16Bit(color);
}

// 填充矩形
void LCD_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color)
{
    unsigned int i, j;
    unsigned int total_points = (xend - xsta + 1) * (yend - ysta + 1);

    if(xend >= LCD_W || yend >= LCD_H) return;
    
    LCD_SetWindows(xsta, ysta, xend, yend);
    
    LCD_CS_CLR;
    LCD_DC_SET; // 批量写入数据，保持DC高电平

    for(i = 0; i < total_points; i++)
    {
        // 写入高八位
        LCD_Writ_Bus(color >> 8); 
        // 写入低八位
        LCD_Writ_Bus(color & 0xFF);
    }
    LCD_CS_SET;
}