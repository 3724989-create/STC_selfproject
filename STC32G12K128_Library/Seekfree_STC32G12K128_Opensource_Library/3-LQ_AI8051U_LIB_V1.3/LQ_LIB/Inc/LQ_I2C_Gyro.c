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

#include "LQ_I2C_Gyro.h"

signed short aac_x, aac_y, aac_z;    // 加速度传感器原始数据
signed short gyro_x, gyro_y, gyro_z; // 陀螺仪原始数据

#define MPU6050_ADDR 0x68 // IIC写入时的地址字节数据，+1为读取

unsigned char IIC_ICM20602 = 0;
unsigned char IIC_ICM20689 = 0;
unsigned char IIC_MPU9250 = 0;
unsigned char IIC_ICM42605 = 0;
unsigned char IIC_MPU6050 = 0;
unsigned char IIC_errorid = 0;

unsigned char Gyro_ID = 0;

/***********************************************************************
 * 函 数 名：unsigned char Gyro_Chose(void)
 * 功    能：判断陀螺仪型号
 * 入口参数：无
 * 返 回 值：判断正确，返回对应ID，错误返回0
 * 说    明：无
 * ***********************************************************************/

unsigned char Gyro_Chose(void)
{
  Gyro_ID = MPU_Read_Byte(MPU6050_ADDR, WHO_AM_I);
  switch (Gyro_ID)
  {
  case 0x12: IIC_ICM20602 = 1;   break;
  case 0x71: IIC_MPU9250 = 1;    break;
  case 0x98: IIC_ICM20689 = 1;   break;
  case 0x42: IIC_ICM42605 = 1;   break;
  case 0x68: IIC_MPU6050 = 1;    break;
  default:   IIC_errorid = 1;
      return 0;
  }
  return Gyro_ID;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【函数名】void Test_IIC_Gyro(void)
 【功  能】测试陀螺仪模块
 【参数值】无
 【返回值】无
 【作  者】LQ Guo
 【最后更新】2022年11月6日
 【软件版本】V1.82
 【调用样例】 Test_IIC_Gyro();
   检测陀螺仪加速度计模块
   型号：6050 20602 20689 9250 42605（AD0接GND）自动识别
 IIC接口 SCK-B10 SDA-B11
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_IIC_Gyro(void)
{
  unsigned char res;
  char txt[30];
  signed short aacx, aacy, aacz;    // 加速度传感器原始数据
  signed short gyrox, gyroy, gyroz; // 陀螺仪原始数据
  short magx, magy, magz;           // 地磁原始数据

  SOFT_IIC_Init(); // IIC初始化//IIC初始化
  delay_ms(100);
  res = Gyro_Chose(); // 判断陀螺仪型号
  sprintf(txt, "ID:0X%2x", res);

#ifdef USE_OLED
  OLED_Init(); // LCD初始化
  delay_ms(100);
  OLED_CLS();               // LCD清屏
  OLED_P6x8Str(70, 2, txt); // 字符串显示
  if (IIC_ICM20602)
    OLED_P8x16Str(0, 0, "LQ 20602 Test"); // 字符串显示
  if (IIC_MPU6050)
    OLED_P8x16Str(0, 0, "LQ 6050 Test"); // 字符串显示
  if (IIC_ICM42605)
    OLED_P8x16Str(0, 0, "LQ 42605 Test"); // 字符串显示
  if (IIC_MPU9250)
    OLED_P8x16Str(0, 0, "LQ 9250 Test"); // 字符串显示
  if (IIC_ICM20689)
    OLED_P8x16Str(0, 0, "LQ 20689 Test"); // 字符串显示
  if (IIC_errorid)
  {
    OLED_P8x16Str(0, 0, "Gyro Test fail"); // 字符串显示
    while (1)
      ;
  }
#endif
#ifdef TFT18
  TFTSPI_Init(1);       // 屏幕初始化为横屏
  TFTSPI_CLS(u16BLACK); // 清屏
  if (IIC_MPU9250)
  {
    sprintf(txt, "0X%2x", res);
    TFTSPI_P8X16Str(10, 8, "ID:", u16RED, u16BLACK);
    TFTSPI_P8X16Str(12, 9, txt, u16RED, u16BLACK); // 字符串显示
  }
  else
    TFTSPI_P8X16Str(9, 9, txt, u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM20602)
    TFTSPI_P8X16Str(2, 0, "LQ 20602 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_MPU6050)
    TFTSPI_P8X16Str(2, 0, "LQ 6050 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM42605)
    TFTSPI_P8X16Str(2, 0, "LQ 42605 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_MPU9250)
    TFTSPI_P8X16Str(2, 0, "LQ 9250 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM20689)
    TFTSPI_P8X16Str(2, 0, "LQ 20689 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_errorid)
  {
    TFTSPI_P8X16Str(1, 0, "Gyro Test fail", u16RED, u16BLACK); // 字符串显示
    while (1)
      ;
  }
#endif
#ifdef TFT20
  TFT2SPI_Init(1);                                // LCD初始化  0:横屏  1：竖屏
  TFT2SPI_CLS(u16BLACK);                          // 清屏
  TFT2SPI_P8X16Str(0, 10, txt, u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM20602)
    TFT2SPI_P8X16Str(2, 0, "LQ 20602 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_MPU6050)
    TFT2SPI_P8X16Str(2, 0, "LQ 6050 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM42605)
    TFT2SPI_P8X16Str(2, 0, "LQ 42605 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_MPU9250)
    TFT2SPI_P8X16Str(2, 0, "LQ 9250 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_ICM20689)
    TFT2SPI_P8X16Str(2, 0, "LQ 20689 Test", u16RED, u16BLACK); // 字符串显示
  if (IIC_errorid)
  {
    TFT2SPI_P8X16Str(1, 0, "Gyro Test fail", u16RED, u16BLACK); // 字符串显示
    while (1)
      ;
  }
#endif
  MPU6050_Init();
  while (1)
  {
    MPU_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); // 得到加速度传感器数据
    if (IIC_MPU9250)
      MPU9250_Get_Magnetometer(&magx, &magy, &magz);
#ifdef OLED
    sprintf((char *)txt, "ax:%06d", aacx);
    OLED_P6x8Str(0, 2, txt);
    sprintf((char *)txt, "ay:%06d", aacy);
    OLED_P6x8Str(0, 3, txt);
    sprintf((char *)txt, "az:%06d", aacz);
    OLED_P6x8Str(0, 4, txt);
    sprintf((char *)txt, "gx:%06d", gyrox);
    OLED_P6x8Str(0, 5, txt);
    sprintf((char *)txt, "gy:%06d", gyroy);
    OLED_P6x8Str(0, 6, txt);
    sprintf((char *)txt, "gz:%06d", gyroz);
    OLED_P6x8Str(0, 7, txt);

    if (IIC_MPU9250)
    {
      sprintf((char *)txt, "gx:%06d", magx);
      OLED_P6x8Str(70, 5, txt);
      sprintf((char *)txt, "gy:%06d", magy);
      OLED_P6x8Str(70, 6, txt);
      sprintf((char *)txt, "gz:%06d", magz);
      OLED_P6x8Str(70, 7, txt);
    }

#endif
#ifdef TFT18
    sprintf((char *)txt, "ax:%06d", aacx);
    TFTSPI_P8X16Str(0, 1, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "ay:%06d", aacy);
    TFTSPI_P8X16Str(0, 2, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "az:%06d", aacz);
    TFTSPI_P8X16Str(0, 3, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gx:%06d", gyrox);
    TFTSPI_P8X16Str(0, 4, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gy:%06d", gyroy);
    TFTSPI_P8X16Str(0, 5, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gz:%06d", gyroz);
    TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLACK); // 字符串显示
    if (IIC_MPU9250)
    {
      sprintf((char *)txt, "mx:%06d", magx);
      TFTSPI_P8X16Str(0, 7, txt, u16RED, u16BLACK);
      sprintf((char *)txt, "my:%06d", magy);
      TFTSPI_P8X16Str(0, 8, txt, u16RED, u16BLACK);
      sprintf((char *)txt, "mz:%06d", magz);
      TFTSPI_P8X16Str(0, 9, txt, u16RED, u16BLACK);
    }

#endif
#ifdef TFT20
    sprintf((char *)txt, "ax:%06d", aacx);
    TFT2SPI_P8X16Str(0, 1, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "ay:%06d", aacy);
    TFT2SPI_P8X16Str(0, 2, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "az:%06d", aacz);
    TFT2SPI_P8X16Str(0, 3, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gx:%06d", gyrox);
    TFT2SPI_P8X16Str(0, 4, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gy:%06d", gyroy);
    TFT2SPI_P8X16Str(0, 5, txt, u16RED, u16BLACK); // 字符串显示
    sprintf((char *)txt, "gz:%06d", gyroz);
    TFT2SPI_P8X16Str(0, 6, txt, u16RED, u16BLACK); // 字符串显示
    if (IIC_MPU9250)
    {
      sprintf((char *)txt, "mx:%06d", magx);
      TFT2SPI_P8X16Str(0, 7, txt, u16RED, u16BLACK);
      sprintf((char *)txt, "my:%06d", magy);
      TFT2SPI_P8X16Str(0, 8, txt, u16RED, u16BLACK);
      sprintf((char *)txt, "mz:%06d", magz);
      TFT2SPI_P8X16Str(0, 9, txt, u16RED, u16BLACK);
    }
#endif
    // 上位机显示
    //        ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,1,1);
    delay_ms(30);
  }
}

//***************反正切Y轴角度*************************//
/*
 * 函数名：Mpu6050_ACC_angle_X_Z
 * 描述  ：加速度计反正切角度
 * 输入  ：无
 * 输出  ：角度值
 * 调用  ：内部调用
 */
// float Mpu6050_ACC_angle_X_Z(void)
//{
//   Get_AccData();
//   return (atan2f((float)aacz,(float)aacx)*(180.0/3.1415));   //反正切加速度计得到角度值
// }
////**************积分Y轴角度*************************//
///*
// * 函数名：Mpu6050_GYRO_Y_Ang(void)
// * 描述  ：积分Y轴角度
// * 输入  ：无
// * 输出  ：无
// * 调用  ：内部调用
// */
// void Mpu6050_GYRO_Y_Ang(void)
//{
//    float
//  Get_Gyro();   //获取Y轴陀螺仪值
//  GYRO_Speed_Y= (float)((GYRO_Y)*0.00032); //转换系数0.00018波形正常   0.00015
//  GYRO_Ang_Y+=GYRO_Speed_Y;  //积分角度
//}
////***************加速度计和陀螺仪数据融合*****************************//
// void Mpu6050_GYRO_ACCEL_Ang_Y()
//{
//   Mpu6050_GYRO_Y_Ang();
//   ACCEL_Ang_Y=Mpu6050_ACC_angle_X_Z();
//   temp_p=(ACCEL_Ang_Y-GYRO_Ang_Y)*0.025;//融合系数  //0.025
//   GYRO_Ang_Y=GYRO_Ang_Y+temp_p;
//   ACC_GYRO_Ang_Y=GYRO_Ang_Y;
// }

/************************************************************************
 * 函数名：MPU6050_Init
 * 描述  ：初始化MPU6050 或者 ICM20602
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 *************************************************************************/
unsigned char MPU6050_Init(void)
{
  int res;
  if (IIC_ICM42605)
  {
    MPU_Write_Byte(MPU6050_ADDR, device_config_reg,
                   bit_soft_reset_chip_config); // chip soft reset
    delay_ms(10);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x01); // Change to bank 1
    MPU_Write_Byte(MPU6050_ADDR, intf_config4, 0x03); // 4 wire spi mode  四线SPI  默认值0x03
    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    MPU_Write_Byte(MPU6050_ADDR, fifo_config_reg, 0x40); // Stream-to-FIFO Mode  陀螺仪Y轴陷波滤波器的频率选择

    res = MPU_Read_Byte(MPU6050_ADDR, int_source0_reg);
    MPU_Write_Byte(MPU6050_ADDR, int_source0_reg, 0x00);
    MPU_Write_Byte(MPU6050_ADDR, fifo_config2_reg, 0x00); // watermark
    MPU_Write_Byte(MPU6050_ADDR, fifo_config3_reg, 0x02); // watermark
    MPU_Write_Byte(MPU6050_ADDR, int_source0_reg, (unsigned char)res);
    MPU_Write_Byte(MPU6050_ADDR, fifo_config1_reg, 0x63); // Enable the accel and gyro to the FIFO

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    MPU_Write_Byte(MPU6050_ADDR, int_config_reg, 0x36);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = (MPU_Read_Byte(MPU6050_ADDR, int_source0_reg) | bit_int_fifo_ths_int1_en);
    MPU_Write_Byte(MPU6050_ADDR, int_source0_reg, (unsigned char)res);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = ((MPU_Read_Byte(MPU6050_ADDR, accel_config0_reg) & 0x1F) | (bit_accel_ui_fs_sel_8g)); // 8g
    MPU_Write_Byte(MPU6050_ADDR, accel_config0_reg, (unsigned char)res);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = ((MPU_Read_Byte(MPU6050_ADDR, accel_config0_reg) & 0xF0) | bit_accel_odr_50hz);
    MPU_Write_Byte(MPU6050_ADDR, accel_config0_reg, (unsigned char)res);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = ((MPU_Read_Byte(MPU6050_ADDR, gyro_config0_reg) & 0x1F) | (bit_gyro_ui_fs_sel_1000dps));
    MPU_Write_Byte(MPU6050_ADDR, gyro_config0_reg, (unsigned char)res);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = ((MPU_Read_Byte(MPU6050_ADDR, gyro_config0_reg) & 0xF0) | bit_gyro_odr_50hz);
    MPU_Write_Byte(MPU6050_ADDR, gyro_config0_reg, (unsigned char)res);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = MPU_Read_Byte(MPU6050_ADDR, pwr_mgmt0_reg) | (bit_accel_mode_ln); // Accel on in LNM
    MPU_Write_Byte(MPU6050_ADDR, pwr_mgmt0_reg, (unsigned char)res);
    delay_ms(1);

    MPU_Write_Byte(MPU6050_ADDR, reg_bank_sel, 0x00);
    res = MPU_Read_Byte(MPU6050_ADDR, pwr_mgmt0_reg) | (bit_gyro_mode_ln); // Gyro on in LNM
    MPU_Write_Byte(MPU6050_ADDR, pwr_mgmt0_reg, (unsigned char)res);
    delay_ms(1);
    return 0;
  }
  else
  {
    res = MPU_Read_Byte(MPU6050_ADDR, WHO_AM_I); // 读取MPU6050的ID
    if (res != Gyro_ID)                          // 器件ID正确
      return 1;

    res = 0;
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
    delay_ms(100);                                                // 延时100ms
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
    res += MPU_Set_Gyro_Fsr(3);                                   // 陀螺仪传感器,±2000dps
    res += MPU_Set_Accel_Fsr(1);                                  // 加速度传感器,±4g
    res += MPU_Set_Rate(1000);                                    // 设置采样率1000Hz
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_CFG_REG, 0x02);       // 设置数字低通滤波器   98hz
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_INT_EN_REG, 0X00);    // 关闭所有中断
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
    res += MPU_Write_Byte(MPU6050_ADDR, MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
    if (IIC_MPU9250)
    {
      MPU_Write_Byte(MPU6050_ADDR, MPU_FIFO_EN_REG, 0X00);   // 关闭FIFO
      MPU_Write_Byte(MPU6050_ADDR, MPU_INTBP_CFG_REG, 0X82); // INT引脚低电平有效，开启bypass模式
      res = MPU_Read_Byte(AK8963_ADDR, MPU_MAG_WIA);         // 读取MPU9250的ID
      if (res == AK8963_ID)                                  // 器件ID正确
      {
        MPU_Write_Byte(AK8963_ADDR, MPU_MAG_CNTL2_REG, 0X01); // 软件重置
        delay_ms(100);
        MPU_Write_Byte(AK8963_ADDR, MPU_MAG_CNTL1_REG, 0X16); // 设置为单次模式，输出16bit
      }
    }
    return 0;
  }
}


/*************************************************************************
 * 函数名称 ：MPU_Set_Gyro_Fsr(unsigned char fsr)
 * 功    能 ：设置陀螺仪测量范围
 * 参数说明 ：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
 * 返 回 值 ：
 *************************************************************************/

unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
  return MPU_Write_Byte(MPU6050_ADDR, MPU_GYRO_CFG_REG, fsr << 3);
}


/**************************************************************************
 * 函数名称 ：MPU_Set_Accel_Fsr(unsigned char fsr)
 * 功    能 ：设置加速度计测量范围
 * 参数说明 ：fsr:0,±2g;1,±4g;2,±8g;3,±16g
 * 返 回 值 ：
 *************************************************************************/
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
  return MPU_Write_Byte(MPU6050_ADDR, MPU_ACCEL_CFG_REG, fsr << 3);
}


/**************************************************************************
 * 函数名称 ：MPU_Set_LPF(uint16 lpf)
 * 功    能 ：设置数字低通滤波器
 * 参数说明 ：lpf:数字低通滤波频率(Hz)
 * 返 回 值 ：
 *************************************************************************/
unsigned char MPU_Set_LPF(uint16 lpf)
{
  unsigned char dat = 0;
  if (lpf >= 188)
    dat = 1;
  else if (lpf >= 98)
    dat = 2;
  else if (lpf >= 42)
    dat = 3;
  else if (lpf >= 20)
    dat = 4;
  else if (lpf >= 10)
    dat = 5;
  else
    dat = 6;
  return MPU_Write_Byte(MPU6050_ADDR, MPU_CFG_REG, dat); // 设置数字低通滤波器
}


/**************************************************************************
 * 函数名称 ：MPU_Set_Rate(uint16 rate)
 * 功    能 ：设置采样率
 * 参数说明 ：rate:4~1000(Hz)
 * 返 回 值 ：0：设置成功
 *************************************************************************/
unsigned char MPU_Set_Rate(uint16 rate)
{
  unsigned char dat;
  if (rate > 1000)
    rate = 1000;
  if (rate < 4)
    rate = 4;
  dat = 1000 / rate - 1;
  MPU_Write_Byte(MPU6050_ADDR, MPU_SAMPLE_RATE_REG, dat); // 设置数字低通滤波器
  return MPU_Set_LPF(rate / 2);                           // 自动设置LPF为采样率的一半
}


/**************************************************************************
 * 函数名称 ：MPU_Get_Temperature(void)
 * 功    能 ：读取温度值
 * 参数说明 ：无
 * 返 回 值 ：温度值(扩大了100倍)
 * **************************************************************************/
short MPU_Get_Temperature(void)
{
  unsigned char buf[2];
  short raw;
  float temp;
  MPU_Read_Len(MPU6050_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
  raw = ((uint16)buf[0] << 8) | buf[1];
  temp = 21 + ((double)raw) / 333.87;
  return (short)temp * 100;
}

/**************************************************************************
 * 函数名称 ：MPU9250_Get_Magnetometer(short *mx, short *my, short *mz)
 * 功    能 ：读取磁力计值
 * 参数说明 ：mx,my,mz:磁力计仪x,y,z轴的原始读数(带符号)
 * 返 回 值 ：：读取磁力计值
 **************************************************************************/
unsigned char MPU9250_Get_Magnetometer(short *mx, short *my, short *mz)
{
  unsigned char buf[6], res;
  res = MPU_Read_Len(AK8963_ADDR, MPU_MAG_XOUTL_REG, 6, buf);
  if (res == 0)
  {
    *mx = ((uint16_t)buf[1] << 8) | buf[0];
    *my = ((uint16_t)buf[3] << 8) | buf[2];
    *mz = ((uint16_t)buf[5] << 8) | buf[4];
  }
  MPU_Read_Byte(AK8963_ADDR, MPU_MAG_ST2_REG);
  return res;
}


/**************************************************************************
 * 函数名称 ：MPU_Get_Gyroscope(signed short *gx, signed short *gy, signed short *gz)
 * 功    能 ：读取陀螺仪值
 * 参数说明 ：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
 * 返 回 值 ：
 * **************************************************************************/
unsigned char MPU_Get_Gyroscope(signed short *gx, signed short *gy,
                                signed short *gz)
{
  unsigned char buf[6], res;
  res = MPU_Read_Len(MPU6050_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
  if (res == 0)
  {
    *gx = ((uint16)buf[0] << 8) | buf[1];
    *gy = ((uint16)buf[2] << 8) | buf[3];
    *gz = ((uint16)buf[4] << 8) | buf[5];
  }
  return res;
}


/**********************************************************************************
 * 函数名称 ：MPU_Get_Accelerometer(signed short *ax, signed short *ay, signed short *az)
 * 功    能 ：读取加速度值
 * 参数说明 ：ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
 * 返 回 值 ：加速度值
 * ********************************************************************************/
unsigned char MPU_Get_Accelerometer(signed short *ax, signed short *ay,
                                    signed short *az)
{
  unsigned char buf[6], res;
  res = MPU_Read_Len(MPU6050_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
  if (res == 0)
  {
    *ax = ((uint16)buf[0] << 8) | buf[1];
    *ay = ((uint16)buf[2] << 8) | buf[3];
    *az = ((uint16)buf[4] << 8) | buf[5];
  }
  return res;
}


/******************************************************************************************
 * 函数名称 ：MPU_Get_Raw_data(signed short *ax, signed short *ay, signed short *az, signed short *gx, signed short *gy, signed short *gz)
 * 功    能 ：读取 加速度值 角速度值
 * 参数说明 ：ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
 *             gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
 * 返 回 值 ：0：读取成功，1：读取失败
 * ***************************************************************************************/
unsigned char MPU_Get_Raw_data(signed short *ax, signed short *ay,
                               signed short *az, signed short *gx, signed short *gy, signed short *gz)
{
  if (IIC_ICM42605)
  {
    unsigned char buf[14], res;
    res = MPU_Read_Len(MPU6050_ADDR, ICM_ACCEL_XOUTH_REG, 12, buf);
    if (res == 0)
    {
      *ax = ((uint16)buf[0] << 8) | buf[1];
      *ay = ((uint16)buf[2] << 8) | buf[3];
      *az = ((uint16)buf[4] << 8) | buf[5];
      *gx = ((uint16)buf[6] << 8) | buf[7];
      *gy = ((uint16)buf[8] << 8) | buf[9];
      *gz = ((uint16)buf[10] << 8) | buf[11];
    }
    return res;
  }
  else
  {
    unsigned char buf[14], res;

    res = MPU_Read_Len(MPU6050_ADDR, MPU_ACCEL_XOUTH_REG, 14, buf);
    if (res == 0)
    {
      *ax = ((uint16)buf[0] << 8) | buf[1];
      *ay = ((uint16)buf[2] << 8) | buf[3];
      *az = ((uint16)buf[4] << 8) | buf[5];
      *gx = ((uint16)buf[8] << 8) | buf[9];
      *gy = ((uint16)buf[10] << 8) | buf[11];
      *gz = ((uint16)buf[12] << 8) | buf[13];
    }
    return res;
  }
}

/********************************************************************************
 * 函数名称 ：MPU_Read_Len(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,14,buf);
 * 参数说明 ：addr:器件地址 reg :要读取的寄存器地址 len :要读取的长度 buf :读取到的数据存储区
 * 返 回 值 ：0：读取成功，1：读取失败
 * ********************************************************************************/
unsigned char MPU_Read_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
  return IIC_ReadMultByteFromSlave(addr << 1, reg, len, buf);
}

/********************************************************************************
 * 函数名称 ：MPU_Write_Len(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,14,buf);
 * 参数说明 ：addr:器件地址 reg :要写入的寄存器地址 len :要写入的长度 buf :写入到的数据存储区
 * 返 回 值 ：0：写入成功，1：写入失败
 * ********************************************************************************/
unsigned char MPU_Write_Len(unsigned char addr, unsigned char reg, unsigned char len, unsigned char *buf)
{
  return IIC_WriteMultByteToSlave(addr << 1, reg, len, buf);
}


/***********************************************************************************
 * 函数名称 ：MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);
 * 参数说明 ：addr:器件地址 reg :要写入的寄存器地址 value :要写入的值
 * 返 回 值 ：0：写入成功，1：写入失败
 **********************************************************************************/
unsigned char MPU_Write_Byte(unsigned char addr, unsigned char reg, unsigned char value)
{
  return IIC_WriteByteToSlave(addr << 1, reg, value);
}

/**********************************************************************************
 * 函 数 名 ：MPU_Read_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG);
 * 参数说明 ：addr:器件地址 reg :要读取的寄存器地址
 * 返 回 值 ：读取到的值
 **********************************************************************************/
unsigned char MPU_Read_Byte(unsigned char addr, unsigned char reg)
{
  unsigned char value[1];
  MPU_Read_Len(addr, reg, 1, value);
  return value[0];
}

// void Test_LQDMP(void)
// {
//   char txt[10];
//   unsigned char res;

//   TFTSPI_Init(1); // LCD初始化  0:横屏  1：竖屏
//   SOFT_IIC_Init();
//   TFTSPI_CLS(u16BLACK); // 蓝色屏幕
//   TFTSPI_P8X16Str(2, 0, "LQ DMP Test", u16RED, u16BLACK);
//   printf("LQ DMP Test\n");
//   delay_ms(100);
//   LQ_I2C_Read(0x68, WHO_AM_I, 1, &res); // 读取6050的ID
//   sprintf(txt, "ID:0x%02X", res);
//   TFTSPI_P8X16Str(9, 9, txt, u16PURPLE, u16BLACK); // 字符串显示
//   res = LQ_DMP_Init();
//   printf("res:%d\n", res);
//   delay_ms(100);

//   while (1)
//   {
//     LQ_DMP_Read();

//     //        printf("P：%.2f  R：%.2f  Y：%.2f\n", Pitch, Roll, Yaw);
//     sprintf((char *)txt, "Pitch:%.2f", Pitch);
//     TFTSPI_P8X16Str(0, 1, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "Roll :%.2f", Roll);
//     TFTSPI_P8X16Str(0, 2, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "Yaw  :%.2f", Yaw);
//     TFTSPI_P8X16Str(0, 3, txt, u16RED, u16BLACK);

//     sprintf((char *)txt, "ax:%06d", accel[0]);
//     TFTSPI_P8X16Str(0, 4, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "ay:%06d", accel[1]);
//     TFTSPI_P8X16Str(0, 5, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "az:%06d", accel[2]);
//     TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "gx:%06d", gyro[0]);
//     TFTSPI_P8X16Str(0, 7, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "gy:%06d", gyro[1]);
//     TFTSPI_P8X16Str(0, 8, txt, u16RED, u16BLACK);
//     sprintf((char *)txt, "gz:%06d", gyro[2]);
//     TFTSPI_P8X16Str(0, 9, txt, u16RED, u16BLACK);
//     //        delay_ms(50);
//     // 上位机
//     ANO_DT_send_int16((short)Pitch, (short)Roll, Yaw, gyro[0], gyro[1], gyro[2], -200, 200);
//   }
// }
