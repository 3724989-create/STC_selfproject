#ifndef _IIC_H_
#define _IIC_H_

#include "bsp.h"
extern unsigned char slave_data_ready;  // 从设备数据就绪标志
extern unsigned char slave_receive_count;
extern unsigned char i2c_slave_receive_buffer[16];

void IIC_intit();
void IIC_Process();
void sendI2CStatusToUART(char *status);
void WriteNbyte(unsigned char addr, unsigned char *p, unsigned char number);   //typedef 问题
void I2C_Slave_Process(void);
int I2C_Slave_CheckStart(void);
int I2C_Slave_CheckStop(void);
void I2C_Slave_SendACK(void);
void uart_send_hex_8bit(unsigned char dat);
void Test_I2C_Wiring(void);
void Check_Pin_Configuration(void);
void Test_Without_External_Pullup(void);
void Complete_I2C_Communication_Test(void);
void Enhanced_Complete_Test(void);
void Diagnose_SCL_Problem(void);
void Diagnose_All_P0_Pins(void);
//IIC总线的使用需要大的开漏电阻以确保SCL拉下，不可以单片机直接输出输入

#endif // DEBUG