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

#ifndef __LQ_SoftI2C_H
#define __LQ_SoftI2C_H

#include "config.h"

/* 注意 IIC总线规定，IIC空闲时 SCL和SDA都为高电平 最好外部上拉（一定不能下拉） */
/* 模拟 IIC需要注意，IIC地址左移一位 例如MPU6050 模拟就是地址 0xD0 */

/*通过宏定义来选择使用哪组管脚作为软件I2C管脚使用，然后再初始化 */
/* 如果使用其他管脚，只需要修改下面的端口管脚宏定义即可，此处默认定义了三组可供选择 */


//#define Soft_I2C_P15P14  FALSE
//#define Soft_I2C_P24P23  TRUE       //选择 P23 P24
//#define Soft_I2C_P32P33  FALSE


//#if(Soft_I2C_P15P14)
//#define Soft_I2C_PORT           GPIO_P1
//#define Soft_I2C_SCL_Pin        GPIO_Pin_5
//#define Soft_I2C_SDA_Pin        GPIO_Pin_4
////快速配置端口模式
//#define SOFT_IIC_SCL_PIN        P15 /*!< P15  作为 SCL */
//#define SOFT_IIC_SDA_PIN        P14 /*!< P14  作为 SDA */
//#define SDA_OUT                {P1_MODE_OUT_PP(Soft_I2C_SDA_Pin);};      //SDA 推挽输出
//#define SDA_IN                 {P1_MODE_IN_HIZ(Soft_I2C_SDA_Pin);};      //SDA 高阻态输入
////------------------------------------------------------------------------------------------
//#elif(Soft_I2C_P24P23)
#define Soft_I2C_PORT           GPIO_P2
#define Soft_I2C_SCL_Pin        GPIO_Pin_4
#define Soft_I2C_SDA_Pin        GPIO_Pin_3
//快速配置端口模式
#define SOFT_IIC_SCL_PIN        P24 /*!< P24  作为 SCL */
#define SOFT_IIC_SDA_PIN        P23 /*!< P23  作为 SDA */
#define SDA_OUT                {P2_MODE_OUT_PP(Soft_I2C_SDA_Pin);};      //SDA 推挽输出
#define SDA_IN                 {P2_MODE_IN_HIZ(Soft_I2C_SDA_Pin);};      //SDA 高阻态输入
//------------------------------------------------------------------------------------------
//#elif(Soft_I2C_P32P33)
//#define Soft_I2C_PORT           GPIO_P3
//#define Soft_I2C_SCL_Pin        GPIO_Pin_2
//#define Soft_I2C_SDA_Pin        GPIO_Pin_3
////快速配置端口模式
//#define SOFT_IIC_SCL_PIN        P32 /*!< P32  作为 SCL */
//#define SOFT_IIC_SDA_PIN        P33 /*!< P33  作为 SDA */
//#define SDA_OUT                {P3_MODE_OUT_PP(Soft_I2C_SDA_Pin);};      //SDA 推挽输出
//#define SDA_IN                 {P3_MODE_IN_HIZ(Soft_I2C_SDA_Pin);};      //SDA 高阻态输入
////------------------------------------------------------------------------------------------
//#endif

#define IIC_SCL_H SOFT_IIC_SCL_PIN = 1;
#define IIC_SCL_L SOFT_IIC_SCL_PIN = 0;

#define IIC_SDA_H SOFT_IIC_SDA_PIN = 1;
#define IIC_SDA_L SOFT_IIC_SDA_PIN = 0;

#define IIC_SDA_READ SOFT_IIC_SDA_PIN

void IIC_Start(void);                          // 发送IIC开始信号
void IIC_Stop(void);                           // 发送IIC停止信号
void IIC_Ack(void);                            // IIC发送ACK信号
void IIC_NAck(void);                           // IIC不发送ACK信号
unsigned char IIC_WaitAck(void);               // IIC等待ACK信号
void IIC_SendByte(unsigned char data_t);       // IIC发送一个字节
unsigned char IIC_ReadByte(unsigned char ack); // IIC读取一个字节
int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
void Soft_I2C_Init(void);
unsigned char IIC_ReadByteFromSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char *buf);
unsigned char IIC_ReadMultByteFromSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);
unsigned char IIC_WriteByteToSlave(unsigned char I2C_Addr, unsigned char reg, unsigned char buf);
unsigned char IIC_WriteMultByteToSlave(unsigned char dev, unsigned char reg, unsigned char length, unsigned char *data_t);

#endif
