//=================================================================================================
// 文件名称: LCD_ILI9341.C
// 功能说明: 液晶屏模块驱动程序  2.4寸彩色显示屏 240*320 8位并行口模式
// 基于电路: STC32G 实验箱V9.62 
// 实验目的: 采用LCM+DMA模式清屏和显示BMP图片 
// 整理改编: 浦晓明(浦江一水) 2024-08-27
//=== 硬件连接 ====================================================================================
//=========================================电源接线================================================//
//VDD接DC 3.3V电源
//GND接地
//=======================================液晶屏数据线接线==========================================//
//本模块数据总线类型为8位并口
//液晶屏模块               STC32G
// LCD_D0~D7     接       P60~P67        //液晶屏8位数据线
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块               STC32G
// LCD_RS        接        P45           //数据/命令选择控制信号
// LCD_RD        接        P44           //读控制信号
// LCD_RST       接        P43           //复位信号
// LCD_WR        接        P42           //写控制信号
// LCD_CS        接        P34           //片选控制信号
//================================================================================================//
// 本实验测试使用的2.4寸320*240的并口彩屏，淘宝店 https://kcer001.taobao.com/提供的样品彩屏上测试通过
//**************************************************************************************************/
#include "STC32G.H"
#include "LCD9341.H"
#include "ASCII.H"
#include "LCD_HZK.H"

//定义LCD尺寸(原始0方向)全局变量
unsigned int LCD_W=240;
unsigned int LCD_H=320;

#define DMA_AMT_LEN  2047    //n+1 = 2048  //DAM每次发送的字长

u16 DMA_TxCount;  //DMA发送次数 
u16 xdata LCD_Buf[DMA_AMT_LEN];//DMA数据发送缓存区 LCD_Buf(16位字缓存区)
bit LcmFlag;      //LCM标志

/* 彩色表色号定义 */
const unsigned int COLOR[]={
  0x0000,   //0黑色  00000 000000 00000
  0x0008,   //1蓝色  00000 000000 01000
  0x0200,   //2绿色  00000 010000 00000
  0x0208,   //3青色  00000 010000 01000
  0x5042,   //4红色  01010 000010 00010
  0x5052,   //5紫色  01010 000010 10010
  0xA484,   //6橙色  10100 100100 00100
  0x8410,   //7灰色  10000 100000 10000
  0xC618,   //8亮灰  11000 110000 11000
  0x001F,   //9亮蓝  00000 000000 11111 
  0x07E0,   //10亮绿 GREEN
  0x07FF,   //11亮青 00000 111111 11111
  0xF800,   //12亮红 RED
  0xF81F,   //13亮紫 MAGENTA
  0xFFE0,   //14黄色 YELLOW
  0xFFFF,   //15白色
  0x2104,   //16深灰
  0x0006,   //17暗蓝
  0x79EF,   //18浅紫
  0x8408,   //19土黄
  0x0180,   //20暗绿 0000 0001 1000 0000 
  0x041F,   //21河蓝
  0x0210,   //22蓝黑
  0x801F,   //23紫色
  0x8200,   //24咖啡
  0xF81F,   //25亮紫
  0xFFF0,   //26浅黄
  0x0700,   //27翠绿 0000 0111 0000 0000
  0xC7FF,   //28亮青 1100 0111 1111 1111
  0x841F,   //29青灰
  0xF810,   //30殷红
  0xFC08,   //31橙色
  0x001F,   //BLUE
  0xF81F,   //BRED     
  0xFFE0,   //GRED        
  0x07FF,   //GBLUE
  0x7FFF,   //CYAN
  0xBC40,   //BROWN 棕色
  0xFC07,   //BRRED 棕红色
  0x8430,   //GRAY  灰色
  0x01CF,   //DARKBLUE  深蓝色
  0x7D7C,   //LIGHTBLUE 浅蓝色  
  0x5458,   //GRAYBLUE   灰蓝色
  0x841F,   //LIGHTGREEN 浅绿色
  0xC618,   //LGRAY 浅灰色(PANNEL),窗体背景色
  0xA651,   //LGRAYBLUE 浅灰蓝色(中间层颜色)
  0x2B12,   //LBBLUE    浅棕蓝色(选择条目的反色)
};

/*****************************************************************************
 * @name       :void LCM_Config(void)
 * @date       :2018-11-13 
 * @function   :Config LCM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void LCM_Config(void)
{
	LCMIFCFG  = 0x84;	//bit7 1:Enable Interrupt, bit1 0:8bit mode; bit0 0:8080,1:6800
	LCMIFCFG2 = 0x09;	//RS:P45,RD:P44,WR:P42; Setup Time=2,HOLD Time=1
	LCMIFSTA  = 0x00;
}

/*****************************************************************************
 * @name       :void DMA_Config(void)
 * @date       :2020-12-09 
 * @function   :Config DMA
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void DMA_Config(void)
{
  DMA_LCM_AMT  = (u8)DMA_AMT_LEN;          //设置传输总字节数(低8位)：n+1
  DMA_LCM_AMTH = (u8)(DMA_AMT_LEN>>8);     //设置传输总字节数(高8位)：n+1
  DMA_LCM_TXAH = (u8)((u16)&LCD_Buf >> 8); //发送区地址高位字节
  DMA_LCM_TXAL = (u8)((u16)&LCD_Buf);      //发送区地址低位字节
//  DMA_LCM_RXAH = (u8)((u16)&Buffer >> 8);
//  DMA_LCM_RXAL = (u8)((u16)&Buffer);
  DMA_LCM_CFG = 0x82;   //使能DMA,较高优先级 
  DMA_LCM_STA = 0x00;   //状态字
  DMA_LCM_CR  = 0x00;   //初始控制寄存器
}

//内部函数: 延时Nms
void delayms(int Nms)  // /* x1ms */
{ u16 i;
  do
  { i = MAIN_Fosc / 6000;
    while(--i);
  } while(--Nms);
}
//内部函数: 延时Nus
void delayus(int Nus)  // /* x1us */
{ int i,j;
  for(i=0;i<Nus;i++)
  for(j=0;j<1;j++);
}
/*****************************************************************************
 * 函数: void LCD_WR_CMD(u8 CMD)  
 * 功能: Write an 8-bit command to the LCD screen  写命令
******************************************************************************/
void LCD_WR_CMD(u8 CMD)   
{  
#if (USE_LCM==1)
  LCMIFDATL = CMD;
  LCD_CS=0;
  LCMIFCR = 0x84;   //Enable interface, write command out
  //while(!LCD_CS);
  while(LcmFlag);
  LCD_CS = 1 ;
#else
  LCD_RS = 0;      //写命令
  LCD_CS = 0;
  LCD_WR = 0;
  LCD_Port = CMD;
  LCD_WR = 1;
  LCD_CS = 1;
#endif 
} 
/*****************************************************************************
 * 函数: void LCD_WR_DAT(u8 CMD)  
 * 功能: Write an 8-bit data to the LCD screen  写数据
******************************************************************************/
void LCD_WR_DAT(u8 DAT)
{
#if (USE_LCM==1)
  LCMIFDATL = DAT;
  LCD_CS=0;
  LCMIFCR = 0x85;    //Enable interface, write data out
  //while(!LCD_CS);
  while(LcmFlag);
  LCD_CS = 1 ;
#else
  LCD_RS = 1;       //写数据
  LCD_CS = 0;
  LCD_WR = 0;
  LCD_Port = DAT;
  LCD_WR = 1;
  LCD_CS = 1;
#endif 
}
/*****************************************************************************
 * 函数: void LCD_WR_DAT(u16 CMD)  
 * 功能: Write an 8-bit data to the LCD screen  写16位数据
******************************************************************************/
void LCD_WR_DAT_16Bit(u16 Data)
{
#if (USE_LCM==1)
  LCD_WR_DAT((u8)(Data>>8));
  LCD_WR_DAT((u8)Data);
#else
  LCD_RS = 1;     //写数据 
  LCD_CS = 0;
  LCD_WR = 0;
  LCD_Port = Data >> 8;
  LCD_WR = 1;
  LCD_WR = 0;
  LCD_Port = Data&0xFF;
  LCD_WR = 1;
  LCD_CS = 1;
#endif
}

u16 Color_To_565(u8 r, u8 g, u8 b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}
/*****************************************************************************
 * 函数: LCD_SetWindow(u16 SX,u16 SY,u16 EX,u16 EY);
 * 功能: 设置显示窗口
 * 参数: u16 SX,u16 SY,u16 EX,u16 EY 起始XY坐标,结束XY坐标
******************************************************************************/
void LCD_SetWindow(u16 SX,u16 SY,u16 EX,u16 EY)
{
  LCD_WR_CMD(0x2A);    //定X水平起止坐标
  LCD_WR_DAT((u8)(SX>>8));
  LCD_WR_DAT(0xFF&SX);    
  LCD_WR_DAT((u8)(EX>>8));
  LCD_WR_DAT(0xFF&EX);
  LCD_WR_CMD(0x2B);     //定Y垂直起止坐标 
  LCD_WR_DAT((u8)(SY>>8));
  LCD_WR_DAT(0xFF&SY);    
  LCD_WR_DAT((u8)(EY>>8));
  LCD_WR_DAT(0xFF&EY);  
  LCD_WR_CMD(0x2C);     //开始连续写入
}
/*****************************************************************************
 * @name       :void LCD_Set_GPIO(void)
 * @date       :2018-11-13 
 * @function   :Set the gpio to push-pull mode
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void LCD_Set_GPIO(void)
{
  //P0.4,P0.5口设置成高阻输入
  P0M1=0x30;P0M0=0x00;
  //P1.4,P1.5口设置成高阻输入
  P1M1=0x30;P1M0=0x00;
  //P2口设置成准双向口
  P2M1=0x00;P2M0=0x00;
  //P3.4口设置成推挽输出
  P3M1=0x00;P3M0=0x10;
  //P4.2~P4.5口设置成推挽输出
  P4M1=0x00;P4M0=0x3C;
  //P6口设置成推挽输出
  P6M1=0x00;P6M0=0xFF;
}

/*****************************************************************************
 * @name       :void LCD_RST(void)
 * @date       :2018-08-09 
 * @function   :RST LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/  
void LCDRST(void)
{
  delayms(50);  
  LCD_RST=0;
  delayms(50);
  LCD_RST=1;
  delayms(50);
}

/*****************************************************************************
 * @name       :void LCD_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/      
void LCD_Init(void)
{
  LCD_Set_GPIO();
  LCDRST(); //初始化之前复位
//#if (USE_LCM==1)    //void LCM_Config(void)
//  LCMIFCFG = 0x84;  //bit7 1:Enable Interrupt, bit1 0:8bit mode; bit0 0:8080,1:6800
//  LCMIFCFG2= 0x09;  //RS:P45,RD:P44,WR:P42; Setup Time=2,HOLD Time=1
//  LCMIFSTA = 0x00;
//  EA = 1;  //开中断
//#endif
//#if (USE_DMA==1)    //void DMA_Config(void)
//  DMA_Config(); 
//#endif
  //*************2.4inch ILI9341初始化**********//  
  LCD_WR_CMD(0x28); //display off 关显示 
  delayms(20);
  LCD_WR_CMD(0xCF); LCD_WR_DAT(0x00); LCD_WR_DAT(0xD9); LCD_WR_DAT(0x30); //0xD9/0xC1 
  LCD_WR_CMD(0xED); LCD_WR_DAT(0x64); LCD_WR_DAT(0x03); LCD_WR_DAT(0x12); LCD_WR_DAT(0x81); 
  LCD_WR_CMD(0xE8); LCD_WR_DAT(0x85); LCD_WR_DAT(0x10); LCD_WR_DAT(0x7A); 
  LCD_WR_CMD(0xCB); LCD_WR_DAT(0x39); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x00); LCD_WR_DAT(0x34); LCD_WR_DAT(0x02); 
  LCD_WR_CMD(0xF7); LCD_WR_DAT(0x20); 
  LCD_WR_CMD(0xEA); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); 
  LCD_WR_CMD(0xC0); LCD_WR_DAT(0x1B); //Power control //VRH[5:0] 
  LCD_WR_CMD(0xC1); LCD_WR_DAT(0x12); //Power control //SAP[2:0];BT[3:0] 0x01
  LCD_WR_CMD(0xC5); LCD_WR_DAT(0x08); LCD_WR_DAT(0x26);//VCM control //30 //30
  LCD_WR_CMD(0xC7); LCD_WR_DAT(0XB7); //VCM control2 
  LCD_WR_CMD(0x36); LCD_WR_DAT(0x08); //Memory Access Control 
  LCD_WR_CMD(0x3A); LCD_WR_DAT(0x55); 
  LCD_WR_CMD(0xB1); LCD_WR_DAT(0x00); LCD_WR_DAT(0x1A); 
  LCD_WR_CMD(0xB6); LCD_WR_DAT(0x0A); LCD_WR_DAT(0xA2); // Display Function Control 
  LCD_WR_CMD(0xF2); LCD_WR_DAT(0x00); //3Gamma Function Disable 
  LCD_WR_CMD(0x26); LCD_WR_DAT(0x01); //Gamma curve selected 
  LCD_WR_CMD(0xE0); LCD_WR_DAT(0x0F); LCD_WR_DAT(0x1D); LCD_WR_DAT(0x1A); LCD_WR_DAT(0x0A);  //Set Gamma 
                    LCD_WR_DAT(0x0D); LCD_WR_DAT(0x07); LCD_WR_DAT(0x49); LCD_WR_DAT(0X66); 
                    LCD_WR_DAT(0x3B); LCD_WR_DAT(0x07); LCD_WR_DAT(0x11); LCD_WR_DAT(0x01); 
                    LCD_WR_DAT(0x09); LCD_WR_DAT(0x05); LCD_WR_DAT(0x04);      
  LCD_WR_CMD(0xE1); LCD_WR_DAT(0x00); LCD_WR_DAT(0x18); LCD_WR_DAT(0x1D); LCD_WR_DAT(0x02); //Set Gamma 
                    LCD_WR_DAT(0x0F); LCD_WR_DAT(0x04); LCD_WR_DAT(0x36); LCD_WR_DAT(0x13); 
                    LCD_WR_DAT(0x4C); LCD_WR_DAT(0x07); LCD_WR_DAT(0x13); LCD_WR_DAT(0x0F); 
                    LCD_WR_DAT(0x2E); LCD_WR_DAT(0x2F); LCD_WR_DAT(0x05); 
  LCD_WR_CMD(0x2A); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0xEF); //宽度  
  LCD_WR_CMD(0x2B); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0x01); LCD_WR_DAT(0x3F); //高度
  LCD_WR_CMD(0x11);    //Exit Sleep 退出睡眠
  delayms(120);
  LCD_WR_CMD(0x29);    //display on 开显示 
  //设置LCD属性参数
  LCD_SetDIR(LCD_DIR); //设置LCD显示方向 (设置窗口,包含宽高度)
  //LCD_BL=1;          //点亮背光   
}
/*****************************************************************************
* 函数:void LCD_SetDIR(u8 dir)
* 参数: 0原始 1:转90度 2:转180 3:转270度 (实验箱V9.62)
******************************************************************************/ 
void LCD_SetDIR(u8 dir)
{ 
  switch(dir)
  { case 0:
      LCD_W=240; LCD_H=320;
      LCD_WR_CMD(0x36);
      LCD_WR_DAT(0x08);  //(1<<3));
      break;
    case 1:
      LCD_W=320; LCD_H=240;
      LCD_WR_CMD(0x36);
      LCD_WR_DAT(0xB8);  //((1<<3)|(1<<7)|(1<<5)|(1<<4));
      break;
    case 2:
      LCD_W=240; LCD_H=320;  
      LCD_WR_CMD(0x36);
      LCD_WR_DAT(0xD8);  //((1<<3)|(1<<4)|(1<<6)|(1<<7));
      break;
    case 3:
      LCD_W=320; LCD_H=240;
      LCD_WR_CMD(0x36);
      LCD_WR_DAT(0x68);  //((1<<3)|(1<<5)|(1<<6));
      break;  
    default: break;
  }
  LCD_SetWindow(0,0,LCD_W,LCD_H);  //   
}
/*****************************************************************************
* 函数: void LCD_SetXY(u16 X, u16 Y)  指定当前坐标点
* 参数: X水平坐标 Y垂直坐标. 当前方向范围内
******************************************************************************/ 
void LCD_SetXY(u16 X, u16 Y)
{ LCD_SetWindow(X,Y,X,Y);
  LCD_SetWindow(X,Y,LCD_W,LCD_H);  
} 
/*****************************************************************************
* 函数:void LCD_DrawPoint(u16 X,u16 Y,u8 Color)
* 参数:X:水平坐标 Y:垂直坐标 Color:色号
******************************************************************************/  
void LCD_Point(u16 X,u16 Y,u8 Color)
{ //LCD_SetXY(X,Y); //设置光标位置 
  LCD_SetWindow(X,Y,X,Y);  
  LCD_WR_DAT_16Bit(COLOR[Color]); //画一色点
}    
/*****************************************************************************
* 函数:void LCD_LineH(u16 X, u16 Y,u16 W,u8 Color)
* 参数: 0原始 1:转90度 2:转180 3:转270度 (实验箱V9.62)
******************************************************************************/ 
void LCD_LineH(u16 X, u16 Y,u16 W,u8 Color)
{ u16 i;
  LCD_SetWindow(X,Y,X+W,Y);  
  for(i=0;i<W;i++)LCD_WR_DAT_16Bit(COLOR[Color]); //连续画色点
}
/*****************************************************************************
* 函数:void LCD_LineV(u16 X, u16 Y,u16 W,u8 Color)
* 参数: 0原始 1:转90度 2:转180 3:转270度 (实验箱V9.62)
******************************************************************************/ 
void LCD_LineV(u16 X, u16 Y,u16 H,u8 Color)
{ u16 i;
  LCD_SetWindow(X,Y,X,Y+H);
  for(i=0;i<H;i++)LCD_WR_DAT_16Bit(COLOR[Color]); //连续画色点
  LCD_SetWindow(X,Y,LCD_W,LCD_H);  
}
/*****************************************************************************
* 函数:void LCD_LineH(u16 X0, u16 Y0,u16 X1, u16 Y1,u8 Color)
* 参数: X0起点 Y0起点 X1终点 Y1终点 Color: 色号
******************************************************************************/ 
void LCD_Line(u16 X0, u16 Y0,u16 X1, u16 Y1,u8 Color)
{ 
  u16 x,y;
  u16 XX,YY;
  u16 dx,dy;
  
  if(X0==X1){ LCD_LineH(X0, Y0,X1-X0,Color); return; }  //画水平线
  if(Y0==Y1){ LCD_LineV(X0, Y0,Y1-Y0,Color); return; }  //画垂直线
  if(X0>X1){ x=X0;X0=X1;X1=x; y=Y0;Y0=Y1;Y1=y; }        //为从左到右而交换坐标.
  dx=X1-X0;
  if(Y1>Y0)dy=Y1-Y0;else dy=Y0-Y1;
  if(dx>dy)
  { for(x=0;x<dx;x++ )
    { XX=X0+x;
      if(Y1>Y0)YY=Y0+(float)x*(float)dy/dx;
      else     YY=Y0-(float)x*(float)dy/dx;
      LCD_Point(XX,YY,Color);
    }
  }     
  else  
  { for(y=0;y<dy;y++ )
    { if(Y0<Y1)YY=Y0+y;
      else     YY=Y0-y;
      XX=X0+(float)y*(float)dx/dy;
      LCD_Point(XX,YY,Color);
    }
  }
}
/*****************************************************************************
* 函数:void LCD_Line0(u16 X, u16 Y,u16 L,u8 Fc,u8 Bc);   //画一水平虚线
* 参数: X起点 Y起点 L长度 Fc:Bc 色号  w:间隔
******************************************************************************/ 
void LCD_LineH0(u16 X, u16 Y,u16 L,u8 w,u8 Fc,u8 Bc)
{ u16 i;
  LCD_SetWindow(X,Y,X+L,Y);
  for(i=0;i<L;i++)
  { if(i%w==0)LCD_WR_DAT_16Bit(COLOR[Fc]); //前景色点
    else      LCD_WR_DAT_16Bit(COLOR[Bc]); //背景色点
  }
}
/*****************************************************************************
* 函数:void LCD_Line0(u16 X, u16 Y,u16 L,u8 Fc,u8 Bc);   //画一垂直虚线
* 参数: X起点 Y起点 L长度 Fc:Bc 色号  w:间隔
******************************************************************************/ 
void LCD_LineV0(u16 X, u16 Y,u16 L,u8 w,u8 Fc,u8 Bc)
{ u16 i;
  LCD_SetWindow(X,Y,X,Y+L);
  for(i=0;i<L;i++)
  { if(i%w==0)LCD_WR_DAT_16Bit(COLOR[Fc]); //前景色点
    else      LCD_WR_DAT_16Bit(COLOR[Bc]); //背景色点
  }
}
/***********************************************************************************
* 函数: 画彩色线方框程序 LCD_Box(u16 X,u16 Y,u16 W,u16 H,u8 C1,u8 C2,u8 BC);
* 参数: 起始点X列 Y行,宽度w 高度h, 左上边色C1,右下边色C2,背景色BC(约定80为透明)
************************************************************************************/
void LCD_Box(u16 X,u16 Y,u16 W,u16 H,u8 C1,u8 C2,u8 BC)
{ u16 l; 
  LCD_LineH(X,Y,W,C1);  LCD_LineV(X,Y,H,C1);
  LCD_LineH(X,Y+H,W,C2);LCD_LineV(X+W,Y,H+1,C2);
  if(BC!=80)
  for(l=1;l<H;l++)LCD_LineH(X+1,Y+l,W-1,BC);
}
/*****************************************************************************
* 函数:void LCD_Box(u16 X, u16 Y,u16 W,u16 H,u8 Color)//画一方框
* 参数: X起点 Y起点 W宽度 H高度 Color: 色号 
*       mode: 0:方框 1:上圆角框 2:下圆角框 3:上下圆角框
******************************************************************************/ 
void LCD_Box0(u16 X, u16 Y,u16 W,u16 H,u8 mode,u8 Color)
{ 
  switch(mode)
  { case 0: LCD_LineH(X,Y,W,Color);      LCD_LineV(X,Y,H,Color);
            LCD_LineH(X,Y+H-1,W,Color);  LCD_LineV(X+W-1,Y,H,Color);
            break;
    case 1: LCD_LineH(X+3,Y,W-6,Color);  LCD_LineV(X,Y+3,H-3,Color);
            LCD_Point(X+2,Y+1,Color);    LCD_Point(X+W-3,Y+1,Color);
            LCD_Point(X+1,Y+2,Color);    LCD_Point(X+W-2,Y+2,Color);
            LCD_LineH(X,Y+H-1,W,Color);  LCD_LineV(X+W-1,Y+3,H-3,Color); 
            break;
    case 2: LCD_LineH(X,Y,W,Color);      LCD_LineV(X,Y,H-3,Color); 
            LCD_Point(X+1,Y+H-3,Color);  LCD_Point(X+W-2,Y+H-3,Color);
            LCD_Point(X+2,Y+H-2,Color);  LCD_Point(X+W-3,Y+H-2,Color);
            LCD_LineH(X+3,Y+H-1,W-6,Color);LCD_LineV(X+W-1,Y,H-3,Color);     
            break;
    case 3: LCD_LineH(X+3,Y,W-6,Color);  LCD_LineV(X,Y+3,H-6,Color); 
            LCD_Point(X+2,Y+1,Color);    LCD_Point(X+W-3,Y+1,Color);
            LCD_Point(X+1,Y+2,Color);    LCD_Point(X+W-2,Y+2,Color);
            LCD_Point(X+1,Y+H-3,Color);  LCD_Point(X+W-2,Y+H-3,Color);
            LCD_Point(X+2,Y+H-2,Color);  LCD_Point(X+W-3,Y+H-2,Color);
            LCD_LineH(X+3,Y+H-1,W-6,Color);LCD_LineV(X+W-1,Y+3,H-6,Color); 
            break;
  }
}
/*****************************************************************************
* 函数:void LCD_FillRBox(u16 X, u16 Y,u16 W,u16 H,u8 Color) //填充圆角方框
* 参数: X起点 Y起点 W宽度 H高度 Color: 色号 mode 圆角分类(便于做下拉菜单)
*       mode: 0:方框 1:上圆角框 2:下圆角框 3:上下圆角框
******************************************************************************/ 
void LCD_BoxF(u16 X, u16 Y,u16 W,u16 H,u8 mode,u8 Color)
{ u16 n;
  switch(mode)
  { case 0: for(n=0;n<H;n++)LCD_LineH(X,Y+n,W,Color);   break;            //矩形无圆角
    case 1: LCD_LineH(X+2,Y+0,W-4,Color);   LCD_LineH(X+1,Y+1,W-2,Color); //仅上圆角
            for(n=2;n<H;n++)LCD_LineH(X,Y+n,W,Color);   break; 
    case 2: LCD_LineH(X+1,Y+H-1,W-2,Color); LCD_LineH(X+2,Y+H,W-4,Color); //仅下圆角
            for(n=0;n<H-1;n++)LCD_LineH(X,Y+n,W,Color); break; 
    case 3: LCD_LineH(X+2,Y+0,W-4,Color);   LCD_LineH(X+1,Y+1,W-2,Color); //上下圆角
            LCD_LineH(X+1,Y+H-1,W-2,Color); LCD_LineH(X+2,Y+H,W-4,Color);
            for(n=2;n<H-1;n++)LCD_LineH(X,Y+n,W,Color); break;
  }
}
/*==============================================================================================================
* 在液晶显示屏显示一个5*7点阵ASC字符函数(占用6*8点阵)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_A08(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc)
{ unsigned char i,n,buf,A;
  A=Ch-32;                    /* 取字符的ASC码 */
  for(i=0;i<8;i++)            /* 要写8行    */
  { buf = ASC8[A][i];         /* 取字模     */
    LCD_SetXY(X,Y+i);         /* 定显示地址 */
    for(n=0;n<6;n++)          /* 横向显示6点*/
    { if((buf&0x80)==0x80)LCD_WR_DAT_16Bit(COLOR[Fc]); /* 字模点为1显示前景色 */
      else                                             /* 字模点为0显示原底色或背景色 */
      { if(Bc==80){ LCD_SetXY(X+n,Y+i); }              /* 透明显示模式:仅做地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);              /* 正常显示模式:背景色 */
      }
      buf = buf << 1;          /* 水平移位取点 */
    }                          /* 水平6点显示毕*/
  } /* 一个6*8点阵西文字符显示完毕 */
}   /* P8()   */
/*==============================================================================================================
* 在液晶显示屏显示一个8*12点阵ASC字符函数(占用8*12点阵)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_A12(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc)
{ unsigned char i,n,buf,A;
  A=Ch-32;                    /* 取字符的ASC码 */
  for(i=0;i<12;i++)            /* 要写8行    */
  { buf = ASC12[A][i];         /* 取字模     */
    LCD_SetXY(X,Y+i);         /* 定显示地址 */
    for(n=0;n<8;n++)          /* 横向显示6点*/
    { if((buf&0x80)==0x80)LCD_WR_DAT_16Bit(COLOR[Fc]); /* 字模点为1显示前景色 */
      else                                             /* 字模点为0显示原底色或背景色 */
      { if(Bc==80){ LCD_SetXY(X+n,Y+i); }              /* 透明显示模式:仅做地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);              /* 正常显示模式:背景色 */
      }
      buf = buf << 1;          /* 水平移位取点 */
    }                          /* 水平6点显示毕*/
  } /* 一个8*12点阵西文字符显示完毕 */
}   /* A12()   */
/*==============================================================================================================
* 在液晶显示屏显示一个8*16点阵ASC字符函数(占用8*16点阵)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_A16(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc)
{ unsigned char i,n,buf,A;
  A=Ch-32;                    /* 取字符的ASC码 */
  for(i=0;i<16;i++)           /* 要写8行    */
  { buf = ASC16[A][i];        /* 取字模     */
    LCD_SetXY(X,Y+i);         /* 定显示地址 */
    for(n=0;n<8;n++)          /* 横向显示6点*/
    { if((buf&0x80)==0x80)LCD_WR_DAT_16Bit(COLOR[Fc]); /* 字模点为1显示前景色 */
      else                                             /* 字模点为0显示原底色或背景色 */
      { if(Bc==80){ LCD_SetXY(X+n,Y+i); }              /* 透明显示模式:仅做地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);              /* 正常显示模式:背景色 */
      }
      buf = buf << 1;          /* 水平移位取点 */
    }                          /* 水平6点显示毕*/
  } /* 一个8*16点阵西文字符显示完毕 */
}   /* A16()   */
/*===================================================================================================
** LCD液晶显示屏显示12*24点阵单个字符
** 调用参数: X列Y行坐标, * str串指针, Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
=====================================================================================================*/
void LCD_A24(u16 X,u16 Y,u8 Ch,u8 Fc,u8 Bc)
{ 
  unsigned int i,j,ZM;
  
  for(j=0;j<24;j++)
  { /* 写24行 */
    ZM=ASC24[Ch-32][j*2];           /* 取字模码1 */
    ZM=ZM*256+ASC24[Ch-32][j*2+1];  /* 取字模码2 */
    LCD_SetXY(X,Y+j);               /* 定显示地址 */
    for(i=0;i<12;i++)
    { if((ZM&0x8000)==0x8000)LCD_WR_DAT_16Bit(COLOR[Fc]);/* 字模点为1显示前景色 */
      else                                               /* 字模点为0显示原底色或背景色 */
      { if(Bc==80) { LCD_SetXY(X+i,Y+j); }               /* 透明显示模式: 仅地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);                /* 正常显示模式:背景色 */
      }
      ZM = ZM << 1; 
    } /* 水平12点显示完毕 */
  }   /* 一位字符显示完毕  */
}    /* A24()   */
//========================================================================
// 函数: LCD_HZ16
// 功能: 显示一个16*16汉字
// 参数: X: 水平坐标列 Y: 垂直坐标行
//========================================================================
void LCD_HZ16(u16 X,u16 Y,u8 *Hz,u8 Fc,u8 Bc)
{ unsigned char i,j; 
  unsigned int m,HZM;
  //查找字库汉字表
  for(m=0;m<TotalHZ16;m++)
  { if((Hz[0]==HZ16M[m][0])&&(Hz[1]==HZ16M[m][1]))break; }
  if(m>=TotalHZ16)return; //查无汉字...返回.
  //显示汉字...传输点阵字模32字节
  for( j=0;j<16;j++ )     //16行
  { 
    HZM=256*(HZ16[m][j*2])+(unsigned char)(HZ16[m][j*2+1]);     //取字模
    LCD_SetXY(X,Y+j);     /* 定显示地址 */
    for( i=0;i<16;i++ )   //画16点
    { if((HZM&0x8000)==0x8000)LCD_WR_DAT_16Bit(COLOR[Fc]); /* 字模点为1显示前景色 */
      else
      { if(Bc==80){ LCD_SetXY(X+i,Y+j); }        /* 透明显示模式:仅做地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);        /* 字模点0显示背景色 */
      } HZM=HZM<<1; 
    } 
  }
}
//========================================================================
// 函数: LCD_HZ24
// 功能: 显示一个24*24汉字
// 参数: X: 水平坐标列 Y: 垂直坐标行
//========================================================================
void LCD_HZ24(u16 X,u16 Y,u8 *Hz,u8 Fc,u8 Bc)
{ unsigned char i,j; 
  unsigned int m;
  unsigned long HZM;
  //查找字库汉字表
  for(m=0;m<TotalHZ16;m++)
  { if((Hz[0]==HZ24M[m][0])&&(Hz[1]==HZ24M[m][1]))break; }
  if(m>=TotalHZ24)return; //查无汉字...返回.
  //显示汉字...传输点阵字模32字节
  for( j=0;j<24;j++ )     //16行
  { //循环24行
    HZM = HZ24[m][j*3];        /* 取字模码 */
    HZM = HZM*256+(unsigned char)(HZ24[m][j*3+1]);
    HZM = HZM*256+(unsigned char)(HZ24[m][j*3+2]);  //合成24位
    LCD_SetXY(X,Y+j);     /* 定显示地址 */
    for( i=0;i<24;i++ )   //画16点
    { if((HZM&0x800000)==0x800000)LCD_WR_DAT_16Bit(COLOR[Fc]); /* 字模点为1显示前景色 */
      else
      { if(Bc==80){ LCD_SetXY(X+i,Y+j); }        /* 透明显示模式:仅做地址+1定位 */
        else LCD_WR_DAT_16Bit(COLOR[Bc]);        /* 字模点0显示背景色 */
      } HZM=HZM<<1; 
    } 
  }
}  
/*==============================================================================================================
* 在液晶显示屏显示一个6*8点阵ASC字符串函数(占用8*16点阵)(自动换行)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_Str8(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc)
{
  while(*str)
  { LCD_A08(X,Y,*str,Fc,Bc); X+=6; str++; if(X>LCD_W-6){ X=0;Y+=8; } }
}
/*==============================================================================================================
* 在液晶显示屏显示一个16*16点阵中西文字符串函数(自动换行)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_Str16(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc)
{ u8 ch[3];
  while(*str)
  { if(*str>0x80)
    { ch[0]=*str;ch[1]=*(str+1);
      LCD_HZ16(X,Y,ch,Fc,Bc); 
      X+=16; str+=2;
    } 
    else
    { LCD_A16(X,Y,*str,Fc,Bc); X+=8; str++; }
    if(X>LCD_W-8){ X=0;Y+=16; }
  }
}
/*==============================================================================================================
* 在液晶显示屏显示一个24*24点阵中西文字符串函数(自动换行)
* 参数: 坐标 水平列X(0-239) 垂直行Y(0-319), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
===============================================================================================================*/
void LCD_Str24(u16 X,u16 Y,u8 * str,u8 Fc,u8 Bc)
{ u8 ch[3];
  while(*str)
  { if(*str>0x80)
    { ch[0]=*str;ch[1]=*(str+1);
      LCD_HZ24(X,Y,ch,Fc,Bc); 
      X+=24; str+=2;
    } 
    else
    { LCD_A24(X,Y,*str,Fc,Bc); X+=12; str++; }
    if(X>LCD_W-12){ X=0;Y+=24; }
  }
}
/*****************************************************************************
* 函数:void LCD_BMP(u16 X, u16 Y,u16 W,u16 H,u8 Color) //显示图片
* 参数: X起点 Y起点 W宽度 H高度 Color: 色号
* 说明: 采用DMA方式时,图片像素必须是256的倍数.
******************************************************************************/ 
void LCD_BMP(u16 X,u16 Y,u8 W,u8 H,u8* pic)
{ u16 i,Col;
  u8 h,l;
  LCD_SetWindow(X,Y,X+W-1,Y+H-1);  //设置显示窗口
  for(i=0;i< W*H;i++)              //< W*H=像素点
  { l=pic[2*i]; h=pic[2*i+1];      //图像数据:低位在前/高位在后
    Col=256*h+l;                   //组成颜色字(16位)
    LCD_WR_DAT_16Bit(Col);         //显示彩色点(高位在前)
  }
}

/*****************************************************************************
 * @name       :void LCD_Clear(u16 Color)
 * @date       :2018-08-09 
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/  
void LCD_CLS(u8 Color)
{ u16 i,j;
  LCD_SetWindow(0,0,LCD_W-1,LCD_H-1);  
#if (USE_DMA==1)           //若使用DMA方式
  for(j=0,i=0;i<=DMA_AMT_LEN;i++)LCD_Buf[i] = COLOR[Color]; //先向缓存区填颜色字
  DMA_TxCount = 75;   //确定发送次数: (320 * 240 * 2) / 2048 = 75 总字节数/缓存区大小=循环次数
  LCD_CS=0;           //片选中,让DMA向显示屏发送数据...
  DMA_LCM_CR = 0xA0;  //(Write dat 写数据) 使能LCM_DMA / 启动发送数据模式操作 / 引发中断
  while(!LCD_CS);     //等待中断操作完成... (试验不等待...)
#else
  for(i=0;i<LCD_W;i++)
  { for (j=0;j<LCD_H;j++)
    { LCD_WR_DAT_16Bit(COLOR[Color]); }
  }
#endif  
}

/*****************************************************************************
 * @name       :void LCM_Interrupt(void)
 * @date       :2018-11-13 
 * @function   :None
 * @parameters :None
 * @retvalue   :
******************************************************************************/ 
void LCMIF_DMA_Interrupt(void) interrupt 13
{ 
  if(LCMIFSTA & 0x01)
  {
    LCMIFSTA = 0x00;
		LcmFlag = 0;           //LCD_CS=1;
  }
  
  if(DMA_LCM_STA & 0x01)   //若是DMA发送完毕中断...
  { 
     DMA_TxCount--;        //发送次数倒计数
     if(DMA_TxCount==0)
     { 
        DMA_LCM_CR=0;
        LCD_CS=1;
     }    
     else 
     {
        DMA_LCM_CR=0xA0;   //82 Write dat 继续传数据...结束后再次引发中断 DMA_LCM_STA = 1;
     }
    DMA_LCM_STA = 0;       //状态字复位
  }
}

//***** LCD9341.C END ********************************************************
