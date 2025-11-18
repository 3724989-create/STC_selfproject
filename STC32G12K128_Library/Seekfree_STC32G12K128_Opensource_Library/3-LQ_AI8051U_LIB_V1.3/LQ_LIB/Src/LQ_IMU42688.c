#include "include.h"
#include "LQ_IMU42688.h"




enum//SPI通信陀螺仪类型
{
    SPI_ICM42688=1,
}GyroType;



//void Test_SPI_Gyro(void)
//{
//    char txt[30];
//    short aacx, aacy, aacz;    // 加速度传感器原始数据
//    short gyrox, gyroy, gyroz; // 陀螺仪原始数据
//    unsigned char res;
//    GPIO_LED_Init();        //LED初始化
//    OLED_Init();                //屏幕初始化

//    res = SPI_Gryo_Init();
////    Display_CLS(U16_BLACK);
//    sprintf(txt,"returnID:%2x",res);
////    Display_showString(20, 12, txt, U16_WHITE, U16_BLACK, 16);

////    Display_showString(15, 0, "   LQ 42688 Test", U16_WHITE, U16_BLACK, 24);


//    while (1)
//    {
//        ICM_Get_Raw_data(&aacx, &aacy, &aacz, &gyrox, &gyroy, &gyroz); // 得到加速度传感器数据
//        sprintf((char *)txt, "ax:%06d", aacx);
////        Display_showString(0, 2, txt,U16_WHITE, U16_BLACK, 16);

//        sprintf((char *)txt, "ay:%06d", aacy);
////        Display_showString(0, 3, txt, U16_WHITE, U16_BLACK, 16);
//        sprintf((char *)txt, "az:%06d", aacz);
////        Display_showString(0, 4, txt, U16_WHITE, U16_BLACK, 16);

//        sprintf((char *)txt, "gx:%06d", gyrox);
////        Display_showString(0, 5, txt, U16_WHITE, U16_BLACK, 16);

//        sprintf((char *)txt, "gy:%06d", gyroy);
////        Display_showString(0, 6, txt, U16_WHITE, U16_BLACK, 16);

//        sprintf((char *)txt, "gz:%06d", gyroz);
////        Display_showString(0, 7, txt, U16_WHITE, U16_BLACK, 16);



//        delay_ms(5);
//    }
//}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void delayms_icm(uint16 ms)
@功能说明：不精确延时
@参数说明：需要延时时间
@函数返回：无
@调用方法：delayms_icm(100);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void delayms_icm(uint16 ms)
{
    delay_ms(ms);
//    volatile unsigned long i = 0;
//    while (ms--)
//    {
//        for (i = 0; i < 30000; ++i)
//        {
//            __asm("NOP"); /* delay */
//        }
//    }
}

//9轴陀螺仪地磁计的寄存器写数据
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t datax)
{
    uint32_t count = 0;
    ICM_Write_Byte(0x17, 0x00);
    ICM_Write_Byte(0x15, 0x38);
    ICM_Write_Byte(0x16, addr);
    ICM_Write_Byte(0x21, datax);
    ICM_Write_Byte(0x14, 0x4c);
    while((ICM_Read_Byte(0x22) & 0x80) == 0)
    {
        if(1000 < count++){
            return 1;
        }
        delay_us(100);
    }
    return 0;
}

//向9轴陀螺仪地磁计的寄存器读数据
uint8_t SPI_Mag_Read_Reg(uint8_t addr)
{
    uint16_t count = 0;
    ICM_Write_Byte(0x17, 0x01);
    ICM_Write_Byte(0x15, 0x39);
    ICM_Write_Byte(0x16, addr);
    ICM_Write_Byte(0x14, 0x4c);
    while((ICM_Read_Byte(0x22) & 0x01) == 0)
    {
        if(1000 < count++){
            break;
        }
        delay_us(100);
    }
    return ICM_Read_Byte(0x02);
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void Test_ICM20689(void)
@功能说明：初始化 ICM20689
@参数说明：无
@函数返回：0：初始化成功   1：失败
@调用方法：ICM20689_Init();
@备    注：初始化时调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char SPI_Gryo_Init(void)
{

    unsigned char res,add;
//    QSPI_InitConfig(QSPI2_CLK_P15_3, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_6, QSPI2_CS_P15_2, 4500000, 1);
    res = ICM_Read_Byte(WHO_AM_I); // 读取ICM的ID
    add = res;


    ICM_Write_Byte(reg_bank_sel,0x00);//Set to bank 0
    ICM_Write_Byte(reg_bank_sel,0x00);//Set to bank 0
    ICM_Write_Byte(device_config_reg,bit_soft_reset_chip_config);//chip soft reset
    delayms_icm(100);

    ICM_Write_Byte(reg_bank_sel,0x01);//Change to bank 1
    ICM_Write_Byte(intf_config4,0x02);//4 wire spi mode
    ICM_Write_Byte(reg_bank_sel,0x00);
    ICM_Write_Byte(fifo_config_reg,0x40);//Stream-to-FIFO Mode

    res = ICM_Read_Byte(int_source0_reg);
    ICM_Write_Byte(int_source0_reg,0x00);
    ICM_Write_Byte(fifo_config2_reg,0x00);// watermark
    ICM_Write_Byte(fifo_config3_reg,0x02);// watermark
    ICM_Write_Byte(int_source0_reg, res);
    ICM_Write_Byte(fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO

    ICM_Write_Byte(reg_bank_sel,0x00);
    ICM_Write_Byte(int_config_reg,0x36);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = (ICM_Read_Byte(int_source0_reg)|bit_int_fifo_ths_int1_en);
    ICM_Write_Byte(int_source0_reg, res);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ((ICM_Read_Byte(accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
    ICM_Write_Byte(accel_config0_reg, res);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ((ICM_Read_Byte(accel_config0_reg)&0xF0)|bit_accel_odr_50hz);
    ICM_Write_Byte(accel_config0_reg, res);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ((ICM_Read_Byte(gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));//1000
    ICM_Write_Byte(gyro_config0_reg,res);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ((ICM_Read_Byte(gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);
    ICM_Write_Byte(gyro_config0_reg, res);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ICM_Read_Byte(pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
    ICM_Write_Byte(pwr_mgmt0_reg, res);
    delayms_icm(1);

    ICM_Write_Byte(reg_bank_sel, 0x00);
    res = ICM_Read_Byte(pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
    ICM_Write_Byte(pwr_mgmt0_reg, res);
    delayms_icm(1);

    return add;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Set_Gyro_Fsr(u8 fsr)
@功能说明：设置陀螺仪传感器满量程范围
@参数说明：Fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
@函数返回：
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Set_Gyro_Fsr(unsigned char fsr)
{
    return ICM_Write_Byte(ICM_GYRO_CFG_REG, fsr << 3);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Set_Accel_Fsr(u8 fsr)
@功能说明：设置LQ20689陀螺仪传感器满量程范围
@参数说明：fsr:0,±2g;1,±4g;2,±8g;3,±16g
@函数返回：
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Set_Accel_Fsr(unsigned char fsr)
{
    return ICM_Write_Byte(ICM_ACCEL_CFG_REG, fsr << 3);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Set_LPF(u16 lpf)
@功能说明：设置LQ20689数字低通滤波器
@参数说明：lpf:数字低通滤波频率(Hz)
@函数返回：
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Set_LPF(uint16 lpf)
{
    unsigned char data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return ICM_Write_Byte(ICM_CFG_REG, data); // 设置数字低通滤波器
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Set_Rate(u16 rate)
@功能说明：设置LQ20689陀螺仪传感器满量程范围
@参数说明：rate:4~1000(Hz)
@函数返回：
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Set_Rate(uint16 rate)
{
    unsigned char data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    ICM_Write_Byte(ICM_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
    return ICM_Set_LPF(rate / 2);              // 自动设置LPF为采样率的一半
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：u16 ICM_Get_Temperature(void)
@功能说明：读取温度数据
@参数说明：无
@函数返回：温度值(扩大了100倍)
@调用方法：ICM_Get_Temperature();
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
short ICM_Get_Temperature(void)
{
    unsigned char buf[3];
    short raw;
    float temp;
    ICM_Read_Len(ICM_TEMP_OUTH_REG, 2, buf);
    raw = ((uint16)buf[1] << 8) | buf[2];
    temp = 21 + ((double)raw) / 333.87;
    return (short)temp * 100;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Get_Gyroscope(u16 *gx,u16 *gy,u16 *gz)
@功能说明：读取加速度融合数据
@参数说明：加速度三轴数据
@函数返回：无
@调用方法：
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    unsigned char buf[7], res;
    res = ICM_Read_Len(ICM_GYRO_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *gx = ((uint16)buf[1] << 8) | buf[2];
        *gy = ((uint16)buf[3] << 8) | buf[4];
        *gz = ((uint16)buf[5] << 8) | buf[6];
    }
    return res;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Get_Accelerometer(u16 *ax,u16 *ay,u16 *az)
@功能说明：读取角速度融合数据
@参数说明：角速度三轴数据
@函数返回：无
@调用方法：
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Get_Accelerometer(short *ax, short *ay, short *az)
{
    unsigned char buf[7], res;
    res = ICM_Read_Len(ICM_ACCEL_XOUTH_REG, 6, buf);
    if (res == 0)
    {
        *ax = ((uint16)buf[1] << 8) | buf[2];
        *ay = ((uint16)buf[3] << 8) | buf[4];
        *az = ((uint16)buf[5] << 8) | buf[6];
    }
    return res;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Get_Raw_data(u16 *ax,u16 *ay,u16 *az,u16 *gx,u16 *gy,u16 *gz)
@功能说明：读取陀螺仪融合数据
@参数说明：角速度与加速度的原始数据
@函数返回：无
@调用方法：ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
{
    unsigned char buf[15], res;

    res = ICM_Read_Len(ICM_ACCEL_DATA_X1,12,buf);
    *ax=((uint16)buf[1]<<8)|buf[2];
    *ay=((uint16)buf[3]<<8)|buf[4];
    *az=((uint16)buf[5]<<8)|buf[6];
    *gx=((uint16)buf[7]<<8)|buf[8];
    *gy=((uint16)buf[9]<<8)|buf[10];
    *gz=((uint16)buf[11]<<8)|buf[12];

    return res;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Read_Len(u8 reg,u8 len,u8 *buf)
@功能说明：u8 reg起始寄存器,u8 *buf数据指针,u16 len长度
@参数说明：无
@函数返回：无
@调用方法：ICM_Read_Len(ICM_GYRO_XOUTH_REG,6,buf);
@备    注：內部調用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf)
{
    buf[0] = reg | 0x80;
    /* 写入要读的寄存器地址 */
    return QSPI_ReadWriteNByte(QSPI2, buf, buf, len + 1);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：void ICM_Write_Byte(u8 reg,u8 value)
@功能说明：向寄存器写数据
@参数说明：reg（寄存器）,value（数据）
@函数返回：无
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Write_Byte(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg;                                    // 先发送寄存器
    buff[1] = value;                                  // 再发送数据
    return QSPI_ReadWriteNByte(QSPI2, buff, buff, 2); // 发送buff里数据，并采集到 buff里
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：u8 ICM_Read_Byte(u8 reg)
@功能说明：向寄存器读数据
@参数说明：reg（寄存器）,
@函数返回：value（数据）
@调用方法：
@备    注：内部调用
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char ICM_Read_Byte(unsigned char reg)
{
    unsigned char buff[2];
    buff[0] = reg | 0x80; // 先发送寄存器
//    GYRO_CS_L;
    QSPI_ReadWriteNByte(QSPI2, buff, buff, 2);
//    GYRO_CS_H;
    return buff[1];
}
