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


#include "LQ_Soft_SPI.h"

#include "include.h"

/*************************************************************************
*  函数名称：void SPI_SoftInit(void)
*  功能说明：模拟SPI口初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：
*************************************************************************/
void Soft_SPI_Init(void)
{
    GPIO_InitTypeDef GPIO_Soft_SPI;

//    GPIO_Soft_SPI.Pin  = Soft_SPI_SCK_Pin|Soft_SPI_MOSI_Pin|Soft_SPI_MISO_Pin|Soft_SPI_CS_Pin;         //要初始化的I/O
    GPIO_Soft_SPI.Pin  = Soft_SPI_Pin;              //初始化的管脚，低四位，P40~P43    //方式2，一次性初始
    GPIO_Soft_SPI.Mode = GPIO_PullUp;               //要初始化的I/O 准双向口
    GPIO_Inilize(Soft_SPI_PORT,&GPIO_Soft_SPI);     //初始化管脚配置
}

/*************************************************************************
*  函数名称：void SPI_SoftReadWriteNbyte(u8 *lqbuff, u16 len)
*  功能说明：SPI读写数据及长度
*  参数说明：u8 *buf数据指针,u16 len长度
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：
*************************************************************************/
void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u16 len)
{
	u8 i;

    SPI_CS = 0;
    SPI_SCK= 1;
    do
    {
        for(i = 0; i < 8; i++)
        {
            SPI_MOSI = ((*lqbuff) >= 0x80);
            SPI_SCK= 0;
            (*lqbuff) = (*lqbuff)<<1;

            SPI_SCK= 1;

            (*lqbuff) |= SPI_MISO;
        }
    lqbuff++;
  }while(--len);
  SPI_CS = 1;
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI从设备读取数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
{
    unsigned short i;
    unsigned char buff[32];
    buff[0] = reg | 0x80;

    Soft_SPI_ReadWriteNbyte(buff, len+1);
    for(i=0;i<len;i++)
        buf[i]=buff[i+1];
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@功能说明：SPI向设备写入数据
@参数说明：unsigned char reg,设备起始地址unsigned char* buf,数据存放地址unsigned short len,待写入的数据长度
@函数返回：无
@修改时间：2022/02/24
@调用方法：无
@备    注：无
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f;   //先发送寄存器
    buff[1] = value;        //再发送数据
    Soft_SPI_ReadWriteNbyte(buff, 2);
}

