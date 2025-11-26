//========================================================
// TFT 1.8" 彩色显示屏 160*128
// 驱动:ST7735 
// 基于:STC32G12K128芯片 屠龙刀三核心实验板
// 编程:浦晓明(浦江一水) 2024-06-01 PXM
//========================================================
#include "self_LCD.h"
#include "ASCII.h"
//#include "LCD_HZK.h"
#include "STC32Gxx.h"
#include "PIC.h"

//数据类型定义
#define u8  unsigned char 
#define u16 unsigned int
//屏幕像素说明
#define Height 160      //高
#define Width  128      //宽
//RGB_565_16位色彩定义
#define RED    0xF800   //红色
#define GREEN  0x07E0   //绿色
#define BLUE   0x001F   //蓝色
#define YELLOW 0xFFE0   //黄色
#define WHITE  0xFFFF   //白色
#define BLACK  0x0000   //黑色
#define PURPLE 0xF81F   //紫色
/* 32彩色表色号定义 */
unsigned int code COLOR[]={
  0x0000,   //0黑色  00000 000000 00000
  0x1048,   //1蓝色  00010 000010 01000
  0x1442,   //2绿色  00010 100010 00010
  0x1452,   //3青色  00010 100010 10010
  0x9042,   //4红色  10010 000010 00010
  0x9052,   //5紫色  10010 000010 10010
  0xA484,   //6橙色  10100 100100 00100
  0x8410,   //7灰色  10000 100000 10000
  0xC618,   //8亮灰  11000 110000 11000
  0x001F,   //9亮蓝  00000 000000 11111 
  0x07E0,   //10亮绿
  0x07FF,   //11亮青 0000 0111 1111 1111
  0xF800,   //12亮红
  0xF81F,   //13亮紫
  0xFFE0,   //14黄色
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
};
//================================================================
// 接口定义: 利用LCD1602接口(反相排序)
// 1602:  K- A+  D7  D6  D5  D4  D3  D2 D1 D0 E WR RS Vo VCC GND
// OLED: GND VCC SCL SDA RES DC  CS     12864单色OLED
// TFT:  GND VCC P25 P23 P20 P21 P22 BL 128160 TFT     
//================================================================
sbit SCL = P2^5;  //时钟端
sbit SDA = P2^3;  //数据端
sbit RES = P2^0;  //复位端 0:复位
sbit  DC = P2^1;  //0:指令/1:数据
sbit  CS = P2^2;  //片选 0:选中
sbit  BL = P2^7;  //背光 0:灭 1:亮
//========================================================
//函数说明
void delay(u16 count);     //延时
void LCD_WR_CMD(u8 Cmd);   //写命令 
void LCD_WR_DAT(u8 Dat);   //写数据 
//========================================================
// 非精确延时函数
//========================================================
void delay(u16 count)
{ unsigned int i,j;
  for(i=0;i<count;i++)       
    for(j=0;j<500;j++);             
}
//========================================================
// SPI接口写一字节数据基本函数
//========================================================
void SPI_WriteByte(u8 Byte)
{ int i;
  for(i=0;i<8;i++)
  { if(Byte &0x80)SDA=1;
    else          SDA=0;
    SCL=0;
    delay_cpu_cycle(2); //增加低电平延时
    SCL=1;
    delay_cpu_cycle(2);//增加高电平延时
    Byte<<=1;
  }
}
//========================================================
// 写命令基本函数 8位数 一字节
//========================================================
void LCD_WR_CMD(u8 Cmd)
{ 
  CS=0;
  DC=0;
  SPI_WriteByte(Cmd);
  CS=1;	
}
//========================================================
// 写数据令基本函数 8位数 一字节
//========================================================
void LCD_WR_DAT(u8 Dat)
{ 
  CS=0;
  DC=1;
  SPI_WriteByte(Dat);
  CS=1;
}
//========================================================
// 写数据令基本函数 16位数 2字节
//========================================================
void LCD_WR_DATA(u16 Data)
{
  CS=0;
  DC=1;
  SPI_WriteByte((u8)(Data/256));
  SPI_WriteByte((u8)(Data%256));
  CS=1;
}
//========================================================
// 显示屏初始化
//========================================================
void  LCD_Init(void)
{ 
  //CS=0;  //片选中
  //BL=1;  //背光亮
  //--ST7735R Reset Sequence--
  //RES =1; delay(20);   //Delay 20ms 
  RES =0; delay(120);  //Delay 120ms 
  RES =1; delay(100);  //Delay 100ms 
  //--End ST7735S Reset Sequence--
  LCD_WR_CMD(0x11);  //Sleep out 
  delay(120);          //Delay 120ms 
  //--ST7735S Frame Rate-- 
  LCD_WR_CMD(0xB1); LCD_WR_DAT(0x01); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x2D); 
  LCD_WR_CMD(0xB2); LCD_WR_DAT(0x01); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x2D); 
  LCD_WR_CMD(0xB3); LCD_WR_DAT(0x01); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x2D); 
                    LCD_WR_DAT(0x01); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x2D); 
  //--End ST7735S Frame Rate--
  LCD_WR_CMD(0xB4); LCD_WR_DAT(0x07); //Dot inversion 
  //--ST7735S Power Sequence--
  LCD_WR_CMD(0xC0); LCD_WR_DAT(0xA2); LCD_WR_DAT(0x02); LCD_WR_DAT(0x84); 
  LCD_WR_CMD(0xC1); LCD_WR_DAT(0xC5); 
  LCD_WR_CMD(0xC2); LCD_WR_DAT(0x0A); LCD_WR_DAT(0x00); 
  LCD_WR_CMD(0xC3); LCD_WR_DAT(0x8A); LCD_WR_DAT(0x2A); 
  LCD_WR_CMD(0xC4); LCD_WR_DAT(0x8A); LCD_WR_DAT(0xEE); 
  //--End ST7735S Power Sequence--
  LCD_WR_CMD(0xC5); LCD_WR_DAT(0x0E); //VCOM //1a
  LCD_WR_CMD(0x36); LCD_WR_DAT(0xC0); //LCD_WR_DAT(0xC0);竖屏 // LCD_WR_DAT(0xA8); 横屏  //MX, MY, RGB mode 
  //--ST7735S Gamma Sequence--
  LCD_WR_CMD(0xE0); LCD_WR_DAT(0x0F); LCD_WR_DAT(0x1A); LCD_WR_DAT(0x0F); LCD_WR_DAT(0x18); 
                    LCD_WR_DAT(0x2F); LCD_WR_DAT(0x28); LCD_WR_DAT(0x20); LCD_WR_DAT(0x22); 
                    LCD_WR_DAT(0x1F); LCD_WR_DAT(0x1B); LCD_WR_DAT(0x23); LCD_WR_DAT(0x37); 
                    LCD_WR_DAT(0x00); LCD_WR_DAT(0x07); LCD_WR_DAT(0x02); LCD_WR_DAT(0x10); 
  LCD_WR_CMD(0xE1); LCD_WR_DAT(0x0F); LCD_WR_DAT(0x1B); LCD_WR_DAT(0x0F); LCD_WR_DAT(0x17); 
                    LCD_WR_DAT(0x33); LCD_WR_DAT(0x2C); LCD_WR_DAT(0x29); LCD_WR_DAT(0x2E); 
                    LCD_WR_DAT(0x30); LCD_WR_DAT(0x30); LCD_WR_DAT(0x39); LCD_WR_DAT(0x3F); 
                    LCD_WR_DAT(0x00); LCD_WR_DAT(0x07); LCD_WR_DAT(0x03); LCD_WR_DAT(0x10); 
  LCD_WR_CMD(0x2A); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0x80);  //宽度128
  LCD_WR_CMD(0x2B); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0x00); LCD_WR_DAT(0xA0);  //高度160
	LCD_WR_CMD(0xF0); LCD_WR_DAT(0x01);  //Enable test command  
	LCD_WR_CMD(0xF6); LCD_WR_DAT(0x00);  //Disable ram power save mode 
//LCD_WR_CMD(0x2C); 
  //--End ST7735S Gamma Sequence--
  LCD_WR_CMD(0x3A); LCD_WR_DAT(0x05); //65k mode 
  LCD_WR_CMD(0x29); //Display on
  BL=1;  //背光亮
}
//========================================================
//设置显示窗口: X,Y左上坐标,W宽,H高 
//========================================================
void LCD_SetBox(unsigned char X,unsigned char Y,unsigned char W,unsigned char H)
{ 
  LCD_WR_CMD(0x2A); LCD_WR_DAT(0); LCD_WR_DAT(X); LCD_WR_DAT((unsigned char)(X+W-1)); LCD_WR_DAT((unsigned char)(X+W-1));
  LCD_WR_CMD(0x2B); LCD_WR_DAT(0); LCD_WR_DAT(Y); LCD_WR_DAT((unsigned char)(Y+H-1)); LCD_WR_DAT((unsigned char)(Y+H-1));
  LCD_WR_CMD(0x2C);
}
//========================================================
// 指定16位色清屏 Color (16位 RGB 色)
//========================================================
//void LCD_CLS(u16 Color)
//{ int i,j;
//  LCD_SetBox(0,0,128,160);
//	for (i=0;i<Height;i++)
//	{ for (j=0;j<Width; j++)
//	  LCD_WR_DATA(Color);   //16位数据
//  }
//}
/*===========================================================================
** 函数名称: CLS(unsigned int Color) 
** 功能描述: 对LCD进行初始化 同时用某色清屏
============================================================================*/
void LCD_CLS(unsigned char Bcolor)
{ u16 temp;
  LCD_WR_CMD(0x2A);LCD_WR_DAT(0x00);LCD_WR_DAT(0x00);LCD_WR_DAT(0x00);LCD_WR_DAT(0x80);   //设置水平GRAM起止地址
  LCD_WR_CMD(0x2B);LCD_WR_DAT(0x00);LCD_WR_DAT(0x00);LCD_WR_DAT(0x00);LCD_WR_DAT(0xA0);   //设置垂直GRAM起止地址
  LCD_WR_CMD(0x2C); //LCD_WR_Start();
  for (temp = 0; temp < 20480; temp++) LCD_WR_DATA(COLOR[Bcolor]); //128*160*2=40960
}
/*===========================================================================
** 移动点坐标(转换成显存地址,并做好读写数据准备).
** 调用参数: X列(0--159) Y行(0--127) 
============================================================================*/
void LCD_MoveTo(unsigned int X,unsigned int Y)
{
 LCD_WR_CMD(0x2A); LCD_WR_DAT((u8)(X/256)); LCD_WR_DAT((u8)(X%256)); LCD_WR_DAT(0x00); LCD_WR_DAT(0x80);  //定X坐标
 LCD_WR_CMD(0x2B); LCD_WR_DAT((u8)(Y/256)); LCD_WR_DAT((u8)(Y%256)); LCD_WR_DAT(0x00); LCD_WR_DAT(0xA0);  //定Y坐标    
 LCD_WR_CMD(0x2C);
}
/*================================================================================
** 作图函数子集
==================================================================================*/
/* 画彩色点函数. 调用参数: X列 Y行 color色 */
void LCD_Point(unsigned int X,unsigned int Y,unsigned char Fcolor)
{ LCD_MoveTo(X,Y); LCD_WR_DATA(COLOR[Fcolor]); }
/* 画彩色水平线函数.调用参数: 起始点X列 Y行 w长度 Color色 */
void LCD_LineH(unsigned int X,unsigned int Y,unsigned int H,unsigned char Fcolor)
{ unsigned int n;
  LCD_MoveTo(X,Y);
  for(n=0;n<H;n++)LCD_WR_DATA(COLOR[Fcolor]);
}
/* 画彩色垂直线函数.调用参数: 起始点X列 Y行 h长度 Color色 */
void LCD_LineV(unsigned int X,unsigned int Y,unsigned int V,unsigned char Fcolor)
{ unsigned int n;
  LCD_SetBox(X,Y,1,V);
  for(n=0;n<V;n++){ LCD_WR_DATA(COLOR[Fcolor]); } //一次性设置1XV窗口
}
/* 任意两点画直线函数 调用参数:起始点X0列Y0行,结束点X1列Y1行,Color色 */
void LCD_Line(unsigned int X1,unsigned int Y1,unsigned int X2,unsigned int Y2,unsigned char Color)
{ unsigned int i;
  float r;
  if(X1==X2)                                         /* 垂直线 */
  { if(Y1==Y2){ LCD_Point(X1,Y1,Color); return;          /* 画点 */  }
    if(Y1<Y2)LCD_LineV(X1,Y1,Y2-Y1+1,Color);
    else     LCD_LineV(X1,Y2,Y1-Y2+1,Color); return;
  }
  if(X1>X2){ i=X1;X1=X2;X2=i;i=Y1;Y1=Y2;Y2=i; }      /* 交换坐标 */
  if(Y1==Y2){ LCD_LineH(X1,Y1,X2-X1,Color);return; }     /* 画水平线 */
  r=(float)(Y2-Y1)/(float)(X2-X1);                   /* 计算斜率 */
  if(r>=-1&&r<=1)                                    /* 与水平夹角<=45度 */
   for(i=0;i<(X2-X1);i++)LCD_Point(X1+i,Y1+i*r,Color);   /* 画出斜线 */
  else                                               /* 斜率>1  */
  { r=1/r;
   if(r>0)                                           /* 与水平夹角>45度 */ 
     for(i=0;i<(Y2-Y1);i++)LCD_Point(X1+i*r,Y1+i,Color); /* 画出斜线 */
   else 
   { r=-r;
     for(i=0;i<(Y1-Y2);i++)LCD_Point(X1+i*r,Y1-i,Color); /* 画出斜线 */
   }  
  }
}
/*==============================================================================================================*/
/* 画彩色线方框程序 调用参数: 起始点X列 Y行,宽度w 高度h, 左上边色C1,右下边色C2,背景色BC(约定80为透明) */
/*==============================================================================================================*/
void LCD_Box(unsigned int X,unsigned int Y,unsigned int H,unsigned int V,unsigned char C1,unsigned char C2,unsigned char BC)
{ int l; 
  LCD_LineH(X,Y,H,C1);  LCD_LineV(X,Y,V,C1);
  LCD_LineH(X,Y+V,H,C2);LCD_LineV(X+H,Y,V+1,C2);
  if(BC!=80)
  for(l=1;l<V;l++)LCD_LineH(X+1,Y+l,H-1,BC);
}
/*==============================================================================================================
** 在液晶显示屏显示5*7点阵ASC字符串函数(占用6*8点阵)
** 参数: 坐标 水平列X(0-127) 垂直行Y(0-159), * str串指针,Fcolor字前景色,Bcolor字背景色,约定:色号80为透明,Len指定长度 
===============================================================================================================*/
void LCD_A08(unsigned int X,unsigned int Y, char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len)
{ unsigned char i,n,buf,A;
  unsigned int j,l;
  l=Len; //Strlen(str);          /* 计算字符串字符数 */
  for(j=0;j<l;j++)               /* 串字符轮流处理  */
  {                              /* 逐个显示字符串 */
    A=str[j]-32;                 /* 取字符的ASC码 */
    for(i=0;i<8;i++)             /* 要写8行    */
    {  buf = ASC8[A][i];         /* 取字模     */
       LCD_MoveTo(X+j*6,Y+i);    /* 定显示地址 */
       for(n=0;n<6;n++)          /* 横向显示6点*/
      { if((buf&0x80)==0x80)LCD_WR_DATA(COLOR[Fcolor]); /* 字模点为1显示前景色 */
        else                                            /* 字模点为0显示原底色或背景色 */
        { if(Bcolor==80){ LCD_MoveTo(X+j*6+n,Y+i); }    /* 透明显示模式:仅做地址+1定位 */
          else  LCD_WR_DATA(COLOR[Bcolor]);             /* 正常显示模式:背景色 */
        }
       buf = buf << 1;           /* 水平移位取点 */
      }                          /* 水平6点显示毕*/
    } /* 一个6*8点阵西文字符显示完毕 */
  }  /* 一字符串显示完毕 */
}   /* A8()   */
/*===============================================================================================================
** 在彩色LCD液晶显示屏显示16点阵字符串函数 
** 参数: X列Y行坐标, * str串指针, Fcolor字前景色,Bcolor字背景色,约定:色号80为透明,Len指定长度
=================================================================================================================*/
void LCD_A12(unsigned int X,unsigned int Y, char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len)
{ /* 行Y(0-479) 列X(0-319) */
  unsigned char i,buf,A[3],n;
  unsigned int j,l;
  A[1]=0;A[2]=0;
  l=Len;                          //Strlen(str);
  for(i=0;i<l;i++)                /* 逐个显示字符串 */
  {                               
   A[0]=str[i];A[1]=str[i+1];
   for(j=0;j<12;j++)
   { /* 写12行 */
     buf = ASC12[A[0]-32][j];     /* 取字模 */
     LCD_MoveTo(X+i*8,Y+j);           /* 字符左上角显示地址  */
     for(n=0;n<8;n++)             /* 西文字母横向显示8点 */
     { if((buf&0x80)==0x80)LCD_WR_DATA(COLOR[Fcolor]);   /* 字模点为1显示前景色 */
       else                                             /* 字模点为0显示原底色或背景色 */
       { if(Bcolor==80){ LCD_MoveTo(X+i*8+n,Y+j); }         /* 透明显示模式: 仅地址+1定位 */
         else  LCD_WR_DATA(COLOR[Bcolor]);               /* 正常显示模式:背景色 */
       }
       buf = buf << 1; 
     } /* 水平8点显示完毕 */
    } /* 一个8*16点阵西文字符显示完毕 */
   } /* for(i)  */
}   /* A12()  */
/*==================================================================================================================
** 在彩色LCD液晶显示屏显示16点阵西文字符串函数(字模取自ROM) 
** 参数: X列Y行坐标, * str串指针, Fcolor字前景色,Bcolor字背景色,约定:色号80为透明,Len指定长度
===================================================================================================================*/
void LCD_A16(unsigned int X,unsigned int Y, char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len)
{ /* 行Y(0-479) 列X(0-319) */
  char i,buf,A,n;
  unsigned int j,l;
  l=Len;                          //strlen(str);
  for(i=0;i<l;i++)                /* 逐个显示字符串 */
  {  A=str[i];                    /* 显示西文ASCII字符   */
     for(j=0;j<16;j++)
     { /* 写16行 */
       buf = ASC16[A-32][j];      /* 取字模 */
       LCD_MoveTo(X+i*8,Y+j);         /* 字符左上角显示地址 */
       for(n=0;n<8;n++)          /* 西文字母横向显示8点 */
       { if((buf&0x80)==0x80)LCD_WR_DATA(COLOR[Fcolor]);   /* 字模点为1显示前景色 */
        else                                              /* 字模点为0显示原底色或背景色 */
        { if(Bcolor==80){ LCD_MoveTo(X+i*8+n,Y+j); }          /* 透明显示模式: 仅地址+1定位 */
          else  LCD_WR_DATA(COLOR[Bcolor]);                /* 正常显示模式:背景色 */
        }
        buf = buf << 1; 
       } /* 水平8点显示完毕 */
     }  /* 一个8*16点阵西文字符显示完毕 */
  }    /* while   */
}     /* A16()   */
/*===================================================================================================
** 在单色LCD液晶显示屏显示24*16点阵数字串函数 
** 调用参数: X列Y行坐标, * str串指针, Fcolor字前景色,Bcolor字背景色,约定:色号80为透明,Len指定长度
=====================================================================================================*/
void LCD_A24(unsigned int X,unsigned int Y, char *str,unsigned char Fcolor,unsigned char Bcolor,unsigned char Len)
{  /* X(0-319) Y(0-479) */
  unsigned char n,C;
  unsigned int i,j,L,Lw=0,ZM;
  L=Len;                          //strlen(str);
  for(j=0;j<L;j++)
  { C=str[j];                     /* 逐个显示字符串 */
    for(i=0;i<24;i++)
    { /* 写24行 */
      ZM=ASC24[C-32][i*2];           /* 取字模码1 */
      ZM=ZM*256+ASC24[C-32][i*2+1];  /* 取字模码2 */
      LCD_MoveTo(X+Lw,Y+i);              /* 定位左上角 */
      for(n=0;n<12;n++)
      { if((ZM&0x8000)==0x8000)LCD_WR_DATA(COLOR[Fcolor]);/* 字模点为1显示前景色 */
        else                                             /* 字模点为0显示原底色或背景色 */
        { if(Bcolor==80) { LCD_MoveTo(X+Lw+n,Y+i); }         /* 透明显示模式: 仅地址+1定位 */
           else LCD_WR_DATA(COLOR[Bcolor]);               /* 正常显示模式:背景色 */
        }
        ZM = ZM << 1; 
      }         /* 水平16点显示完毕 */
    } Lw=Lw+12; /* 水平坐标位移 */
  }             /* 一位数字显示完毕  */
}   /* A24()   */

/*===================================================================================================
** 在单色LCD液晶显示屏显示24点阵中西文混合字符串函数  (小字库字模取自ROM)
** 调用参数: X列Y行坐标, * str串指针, Fcolor字前景色,Bcolor字背景色,约定:色号80为透明
=====================================================================================================*/
//void LCD_H24(unsigned int X,unsigned int Y, char *str,unsigned char Fcolor,unsigned char Bcolor)
//{  /* X(0-127) Y(0-159) */
//  unsigned char C[3];
//  unsigned int i,j,l,m,n,p,ZM;
//  unsigned long HZM;
//  l=strlen(str);
//  for(i=0;i<l;i++)
//  { /* 逐个显示字符串 */
//    C[0]=str[i];C[1]=str[i+1];   //取汉字内码
//    if(C[0]>160)                 //是汉字...
//    { m=1;p=0;
//      while(m!=0&&p<TotalHZ24)   /* 搜索汉字,若找到返m=0和指针p */
//      { m=strncmp(C,HZ24M[p],2);
//        if(m)p=p+1;
//      }
//      for(j=0;j<24;j++)
//      { /* 循环写24行 */
//        HZM = HZ24[p][j*3];           /* 取字模码 */
//        HZM = HZM*256+(unsigned char)HZ24[p][j*3+1];
//        HZM = HZM*256+(unsigned char)HZ24[p][j*3+2];
//        LCD_MoveTo(X+i*12,Y+j);       /* 字符左上角显示地址 */
//        for(n=0;n<24;n++)             /* 汉字横向显示24点   */
//        { if((HZM&0x800000)==0x800000)LCD_WR_DATA(COLOR[Fcolor]); /* 字模点为1显示前景色 */
//          else                                                    /* 字模点为0显示原底色或背景色 */
//          { if(Bcolor==80){ LCD_MoveTo(X+i*12+n,Y+j); }   /* 透明显示模式:仅做地址+1定位 */
//            else  LCD_WR_DATA(COLOR[Bcolor]);             /* 正常显示模式:背景色 */
//          }
//          HZM = HZM << 1; 
//        }                             /* 水平24点显示完毕 */
//      }
//      i++;
//    }
//    else
//    { for(j=0;j<24;j++)
//      { /* 写24行 */
//        ZM=ASC24[C[0]-32][j*2];          /* 取字模码1 */
//        ZM=ZM*256+ASC24[C[0]-32][j*2+1]; /* 取字模码2 */
//        LCD_MoveTo(X+i*12,Y+j);          /* 定位左上角 */
//        for(n=0;n<12;n++)
//        { if((ZM&0x8000)==0x8000)LCD_WR_DATA(COLOR[Fcolor]);/* 字模点为1显示前景色 */
//          else                                              /* 字模点为0显示原底色或背景色 */
//          { if(Bcolor==80) { LCD_MoveTo(X+i*12+n,Y+j); }         /* 透明显示模式: 仅地址+1定位 */
//            else LCD_WR_DATA(COLOR[Bcolor]);                /* 正常显示模式:背景色 */
//          }
//          ZM = ZM << 1; 
//        } /* 水平12点显示完毕 */
//      } /* 一个12*24点阵字符显示完毕  */
//    } /* else_if */  
//  }  /* 循环逐个显示字符串 */
//}  /* H24()   */
//========================================================
// 显示图片
//========================================================
void LCD_BMP(unsigned char X,unsigned char Y,unsigned char W,unsigned char H,unsigned char * pic)
{	u16 i;
  u8 h,l;
	LCD_SetBox(X,Y,W,H);         //设置显示窗口
  for(i=0;i<W*H;i++)
  { l=pic[2*i]; h=pic[2*i+1];  //图像数据:低位在前/高位在后
    LCD_WR_DAT(h);             //显示彩色点(高位在前)
    LCD_WR_DAT(l);             //显示彩色点(低位在后) 
  }
	LCD_SetBox(0,0,Width,Height);//恢复全屏区域
}

void LCD_gpio_Init(void)
{
  unsigned char i;
  unsigned int wait=0;
  //IO端口初始化
  WTST  = 0; //设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
  EAXFR = 1; //扩展寄存器(XFR)访问使能
  CKCON = 0; //提高访问XRAM速度
  //IO端口初始化//全部为双向口
  P0M1 = 0x00;   P0M0 = 0x04;   //设置为准双向口: P0.2推挽输出控制背光
  P1M1 = 0x00;   P1M0 = 0x00;   //设置为准双向口
    // 【关键修复：将 P2 端口所有 LCD 控制引脚设置为推挽输出】
  // P2.0, P2.1, P2.2, P2.3, P2.5, P2.7 (RES, DC, CS, SDA, SCL, BL) 设置为推挽输出 (0xAF)
  P2M1 = 0x00;   P2M0 = 0xAF;   //设置为准双向口
  P3M1 = 0x00;   P3M0 = 0x00;   //设置为准双向口
  P4M1 = 0x00;   P4M0 = 0x00;   //设置为准双向口
  P5M1 = 0x00;   P5M0 = 0x00;   //设置为准双向口
  LCD_Init();
  LCD_CLS(1);             //蓝色清屏
}

void LCD_Display_CCD_Binary(u8 *ccd_data,u8 len,u8 y_start)
{
    u8 x,y;
    const u8 BLACK_COLOR=0;
    const u8 WHITE_COLOR=15;

    //由于y1_boundry范围大概是0-128    4096(ADC分辨率)/32(分辨率)=128，中间值128/2=64
    const u8 FIXED_THRESHOLD=30;
    
    //用10行显示这条线
    const u8 LINE_HEIGHT=10;
    u8 color_index;
    
    for(x=0;x<len;x++)//x:0 to 127
    {
        if(ccd_data[x]<FIXED_THRESHOLD)
        {
            color_index=WHITE_COLOR;
        }
        else
        {
            color_index = BLACK_COLOR;  
        }
            // 绘制垂直方向上的 10 个点
        for (y = 0; y < LINE_HEIGHT; y++)
        {
            LCD_Point(x, y_start + y, color_index);
        }
    }
}

void LCD_Draw_CCD_Waveform(u8* ccd_data,u8 len,u8 y_start)
{
  //10亮绿色（波形），7灰色（基线）
  const u8 WAVE_COLOR=10;
  const u8 BASELINE_COLOR=7;

  //背景色为索引
  const u8 BANKGROUND_COLOR_INDEX=1;
  const u16 BACKGROUND_COLOR=COLOR[BANKGROUND_COLOR_INDEX];

  //绘图尺寸和范围
  const u8 GRAPH_HEIGHT=120;         //图像再屏幕上占据的像素高度
  const u8 MAX_DATA_VALUE=128;      //y1_boundry最大理论值（0-128）
  const u8 Y_AXIS_START=y_start;      //垂直方向上的基线偏移顶部

  u8 y_prev_mapped=Y_AXIS_START+GRAPH_HEIGHT-1; //用于绘制线上的一个点
  u8 y_new_mopped;
  u16 total_pixels=(u16)len*GRAPH_HEIGHT;
  u16 i;
  u8 x;
  u8 inverted_value;
    

  //清空区域实现
  LCD_SetBox(0,y_start,len,GRAPH_HEIGHT);
  //连续写入背景色
  for(i=0;i<total_pixels;i++)
  {
    LCD_WR_DATA(BACKGROUND_COLOR);
  }

  //绘制网格线
  LCD_LineH(0,Y_AXIS_START+GRAPH_HEIGHT/2,len,BASELINE_COLOR);
  LCD_LineH(0,Y_AXIS_START,len,BASELINE_COLOR);
  LCD_LineH(0,Y_AXIS_START+GRAPH_HEIGHT-1,len,BASELINE_COLOR);

  //遍历所有128个像素点
  for(x=0;x<len;x++)
  {
        // y1_boundary: 0 (亮) -> 128 (暗)
        // 映射逻辑: 将 0-128 映射到 y_start ~ (y_start + GRAPH_HEIGHT)
        // 使用简单的线性映射: 值越大，位置越靠下（代表暗）
    inverted_value=MAX_DATA_VALUE-ccd_data[x];
    y_new_mopped = Y_AXIS_START + GRAPH_HEIGHT - 1 - (inverted_value * GRAPH_HEIGHT / MAX_DATA_VALUE);

    if(x>0)
    {
      LCD_Line(x-1,y_prev_mapped,x,y_new_mopped,WAVE_COLOR);
    }
    else
    {
      LCD_Point(x,y_new_mopped,WAVE_COLOR);
    }
    y_prev_mapped=y_new_mopped;
  }

}

void LCD_PrintfLine(unsigned int Line,const char *format,...)
{
   unsigned int Y_Start = Line * 12; // 假设每行高 12   
    // 水平起始位置 (X 坐标)
   unsigned int X_Start = 8;
  //缓冲取储存格式化后的字符串
  char String[16];
  unsigned char i;  
  const u8 MAX_CHARS=15;
  va_list arg;
  //设置不定变量
  va_start(arg,format);
  // 2. 绑定不定变量并格式化字符串
  // vsnprintf 是更安全的版本，如果您的库支持，建议使用：
  // vsnprintf(String, sizeof(String), format, arg);
  // 这里使用 vsprintf，它在嵌入式环境中更常见：
   vsprintf(String,format,arg);
  //结束不定量定义
  va_end(arg);
  i=sizeof(String);
  if(i>MAX_CHARS)
      i=MAX_CHARS;
  //  强制转换 char* 为 u8* 以匹配 LCD_DisplayStringLine 的参数类型
  LCD_A12(X_Start, Y_Start, (char*)String, 0xA484, WHITE, i);

}

void Self_LCD_Progress(void)
{
    //LCD_Display_CCD_Binary(y1_boundary,Width,120);
    LCD_PrintfLine(1,"the number is %d",key_value_test);
}




//=== END ===============================================================================================