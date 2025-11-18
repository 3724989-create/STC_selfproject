#ifndef __LQ_ICM42688_H_
#define __LQ_ICM42688_H_

#include "config.h"

//*!**************************************
// 定义ICMxx内部地址
#define ICM_SELF_TESTX_REG  0X0D     // 自检寄存器X
#define ICM_SELF_TESTY_REG  0X0E     // 自检寄存器Y
#define ICM_SELF_TESTZ_REG  0X0F     // 自检寄存器Z
#define ICM_SELF_TESTA_REG  0X10     // 自检寄存器A
#define ICM_SAMPLE_RATE_REG 0X19     // 采样频率分频器
#define ICM_CFG_REG         0X1A     // 配置寄存器
#define ICM_GYRO_CFG_REG    0X1B     // 陀螺仪配置寄存器
#define ICM_ACCEL_CFG_REG   0X1C     // 加速度计配置寄存器
#define ICM_MOTION_DET_REG  0X1F     // 运动检测阀值设置寄存器
#define ICM_FIFO_EN_REG     0X23     // FIFO使能寄存器

#define ICM_I2CMST_STA_REG  0X36     // IIC主机状态寄存器
#define ICM_INTBP_CFG_REG   0X37     // 中断/旁路设置寄存器
#define ICM_INT_EN_REG      0X38     // 中断使能寄存器
#define ICM_INT_STA_REG     0X3A     // 中断状态寄存器

#define ICM_ACCEL_XOUTH_REG 0X3B     // 加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG 0X3C     // 加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG 0X3D     // 加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG 0X3E     // 加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG 0X3F     // 加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG 0X40     // 加速度值,Z轴低8位寄存器

#define ICM_ACCEL_DATA_X1   0X1F     //加速度值,X轴高8位寄存器
#define ICM_ACCEL_DATA_X0   0X20     //加速度值,X轴低8位寄存器
#define ICM_ACCEL_DATA_Y1   0X21     //加速度值,Y轴高8位寄存器
#define ICM_ACCEL_DATA_Y0   0X22     //加速度值,Y轴低8位寄存器
#define ICM_ACCEL_DATA_Z1   0X23     //加速度值,Z轴高8位寄存器
#define ICM_ACCEL_DATA_Z0   0X24     //加速度值,Z轴低8位寄存器

#define ICM_TEMP_OUTH_REG   0X41     // 温度值高八位寄存器
#define ICM_TEMP_OUTL_REG   0X42     // 温度值低8位寄存器
     
#define ICM_GYRO_XOUTH_REG  0X43     // 陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG  0X44     // 陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG  0X45     // 陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG  0X46     // 陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG  0X47     // 陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG  0X48     // 陀螺仪值,Z轴低8位寄存器
                                 
#define ICM_I2CSLV0_DO_REG  0X63     // IIC从机0数据寄存器
#define ICM_I2CSLV1_DO_REG  0X64     // IIC从机1数据寄存器
#define ICM_I2CSLV2_DO_REG  0X65     // IIC从机2数据寄存器
#define ICM_I2CSLV3_DO_REG  0X66     // IIC从机3数据寄存器

#define ICM_I2CMST_DELAY_REG 0X67    // IIC主机延时管理寄存器
#define ICM_SIGPATH_RST_REG  0X68    // 信号通道复位寄存器
#define ICM_MDETECT_CTRL_REG 0X69    // 运动检测控制寄存器
#define ICM_USER_CTRL_REG    0X6A    // 用户控制寄存器
#define ICM_PWR_MGMT1_REG    0X6B    // 电源管理寄存器1
#define ICM_PWR_MGMT2_REG    0X6C    // 电源管理寄存器2
#define ICM_FIFO_CNTH_REG    0X72    // FIFO计数寄存器高八位
#define ICM_FIFO_CNTL_REG    0X73    // FIFO计数寄存器低八位
#define ICM_FIFO_RW_REG      0X74    // FIFO读写寄存器
#define WHO_AM_I             0X75    // 器件ID寄存器



void Test_SPI_Gyro(void);
unsigned char SPI_Gryo_Init(void);
unsigned char ICM_Set_Gyro_Fsr(unsigned char fsr);/*@brief    设置陀螺仪测量范围*/
unsigned char ICM_Set_Accel_Fsr(unsigned char fsr);/*@brief    设置加速度计测量范围*/
unsigned char ICM_Set_LPF(unsigned short lpf);  /*@brief   设置数字低通滤波*/
unsigned char ICM_Set_Rate(unsigned short rate);/*@brief    设置采样率*/
short ICM_Get_Temperature(void);                 /*@brief    获取温度值*/
unsigned char ICM_Get_Gyroscope(short *gx, short *gy, short *gz);/*@brief    获取陀螺仪值*/
unsigned char ICM_Get_Accelerometer(short *ax, short *ay, short *az);/*brief    获取加速度值*/
unsigned char ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz);/*@brief    获取 加速度值 角速度值*/
unsigned char ICM_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf);/*@brief    SPI 连续读*/
unsigned char ICM_Write_Byte(unsigned char reg, unsigned char value);/*@brief    SPI 写一个寄存器*/
unsigned char ICM_Read_Byte(unsigned char reg);/* @brief    SPI 读一个寄存器*/
void Gyro_Get_Mag_data(short *magx,short *magy,short *magz);

uint8_t SPI_Mag_Read_Reg(uint8_t addr);
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t datax);

#endif
