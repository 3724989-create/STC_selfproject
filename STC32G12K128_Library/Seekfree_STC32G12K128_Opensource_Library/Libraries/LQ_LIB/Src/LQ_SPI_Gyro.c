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

#include "LQ_SPI_Gyro.h"


// 陀螺仪初始化函数
// 初始化陀螺仪只需要调用此函数即可
void Gyro_SPI_Init(void)
{
#ifdef USE_HAL_SPI
//    SPIM_Init(SPI1, baud_4500K, GYRO_SCK_PIN, GYRO_SDO_PIN, GYRO_SDI_PIN, PA15);

//    Gyro_Check();
//    if (Gyro_Parameter_Set())
//    {
//        LCD_ShowStr_16(3, 1, "Test Fail", u16RED, u16BLACK); // 字符串显示
//        while (1)
//            ;
//    }
#else           //USE_SOFT_SPI
    
   Soft_SPI_Init();

#endif      //USE_HAL_SPI  or USE_SOFT_SPI

}

// 向寄存器中读取数值
unsigned char SPI_Gyro_read_reg(unsigned char reg)
{
    unsigned char buff1, buff2;
    buff1 = reg | 0x80;
    GYRO_CS_L;
    #ifdef USE_HAL_SPI
//    SPI_Read_Array_8Bit_Add(GYRO_SPI, &buff2, buff1, 2);
    #else
    LQ_SPI_Read(reg, 1, ID);
    #endif
    GYRO_CS_H;
    return buff2;
}

// 向一个寄存器写数据
void SPI_Gyro_write_reg(unsigned char reg, unsigned char value)
{
    GYRO_CS_L;
    SPI_Write_8Bit_Add(GYRO_SPI, reg, value);
    GYRO_CS_H;
}

// 从reg寄存器开始取数据，读的长度为len，保存在buf中
void SPI_Gyro_Read_Len(unsigned char reg, unsigned char len, unsigned char *buf)
{
    GYRO_CS_L;
    SPI_Read_Array_8Bit_Add(GYRO_SPI, buf, reg | 0x80, len);
    GYRO_CS_H;
}

// 9轴陀螺仪地磁计的寄存器写数据
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t datax)
{
    uint32_t count = 0;
    SPI_Gyro_write_reg(0x17, 0x00);
    SPI_Gyro_write_reg(0x15, 0x38);
    SPI_Gyro_write_reg(0x16, addr);
    SPI_Gyro_write_reg(0x21, datac);
    SPI_Gyro_write_reg(0x14, 0x4c);
    while ((SPI_Gyro_read_reg(0x22) & 0x80) == 0)
    {
        if (1000 < count++)
        {
            return 1;
        }
        Delay_Us(100);
    }
    return 0;
}

// 向9轴陀螺仪地磁计的寄存器读数据
uint8_t SPI_Mag_Read_Reg(uint8_t addr)
{
    uint16_t count = 0;
    SPI_Gyro_write_reg(0x17, 0x01);
    SPI_Gyro_write_reg(0x15, 0x39);
    SPI_Gyro_write_reg(0x16, addr);
    SPI_Gyro_write_reg(0x14, 0x4c);
    while ((SPI_Gyro_read_reg(0x22) & 0x01) == 0)
    {
        if (1000 < count++)
        {
            break;
        }
        Delay_Us(100);
    }
    return SPI_Gyro_read_reg(0x02);
}

// 6轴陀螺仪LSM6DSR和九轴陀螺仪LQ9AGMV的检测
uint8_t Gyro_LSM6DSR_LQ9AGMV_Check(void)
{
    uint8_t res;
    SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x00); // 主IIC操作
    SPI_Gyro_write_reg(LSM6DSR_CTRL3_C, 0x01);
    delay_ms(2);
    SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x00);
    res = SPI_Gyro_read_reg(WHO_AM_LSM6D);
    if (res == DRV_ID_LSM6D)
    {
        for (int i = 0; i <= 3; i++)
        {
            SPI_Gyro_write_reg(0x14, 0x80);
            delay_ms(5);
            SPI_Gyro_write_reg(0x14, 0x00);
            delay_ms(5);
            SPI_Gyro_write_reg(LSM6DSR_CTRL1_XL, 0X20);
            SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x40);
            if (SPI_Mag_Read_Reg(0x0f) == Mag_ID)
            {
                res = Mag_ID;
                break;
            }
        }
        SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x00);
    }

    return res;
}

// 陀螺仪型号检测
uint8_t Gyro_Check(void)
{
    unsigned char res;
    char txt[50] = {0};
    res = SPI_Gyro_read_reg(WHO_AM_I); // 读取ICM20602的ID
    switch (res)
    {
    case 0x12:
        GyroType = SPI_ICM20602;
        break;
    case 0x71:
        GyroType = SPI_MPU9250;
        break;
    case 0x98:
        GyroType = SPI_ICM20689;
        break;
    case 0x42:
        GyroType = SPI_ICM42605;
        break;
    default:
        res = Gyro_LSM6DSR_LQ9AGMV_Check();
        if (res == DRV_ID_LSM6D)
            GyroType = SPI_LSM6DSR;
        else if (res == Mag_ID)
            GyroType = SPI_LQ9AGMV;
        else
            GyroType = error;
        break;
    }
    switch (GyroType)
    {
    case SPI_ICM20602:
        LCD_ShowStr_16(0, 0, "LQ 20602 Test", u16RED, u16BLACK);
        break; // 字符串显示
    case SPI_ICM20689:
        LCD_ShowStr_16(0, 0, "LQ 20689 Test", u16RED, u16BLACK);
        break; // 字符串显示
    case SPI_MPU9250:
        LCD_ShowStr_16(0, 0, "LQ 9250 Test", u16RED, u16BLACK);
        break; // 字符串显示
    case SPI_ICM42605:
        LCD_ShowStr_16(0, 0, "LQ 42605 Test", u16RED, u16BLACK);
        break; // 字符串显示
    case SPI_LSM6DSR:
        LCD_ShowStr_16(0, 0, "LQ LSM6DSR Test", u16RED, u16BLACK);
        break; // 字符串显示
    case SPI_LQ9AGMV:
        LCD_ShowStr_16(0, 0, "LQ LQ9AGMV Test", u16RED, u16BLACK);
        break; // 字符串显示
    case error:
        sprintf(txt, "Gyro rec fail:%x", res);
        LCD_ShowStr_16(0, 0, txt, u16RED, u16BLACK);
        while (1)
            ;
    }
    return res;
}

// 陀螺仪初始化参数设置
uint8_t Gyro_Parameter_Set(void)
{
    uint8_t res;
    if (GyroType == SPI_MPU9250)
        return 0;
    if (GyroType == SPI_ICM42605)
    {
        GYRO_CS_L;
        SPI_Gyro_write_reg(reg_bank_sel, 0x00);                            // Set to bank 0
        SPI_Gyro_write_reg(reg_bank_sel, 0x00);                            // Set to bank 0
        SPI_Gyro_write_reg(device_config_reg, bit_soft_reset_chip_config); // chip soft reset
        delay_ms(100);

        SPI_Gyro_write_reg(reg_bank_sel, 0x01); // Change to bank 1
        SPI_Gyro_write_reg(intf_config4, 0x02); // 4 wire spi mode
        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        SPI_Gyro_write_reg(fifo_config_reg, 0x40); // Stream-to-FIFO Mode

        res = SPI_Gyro_read_reg(int_source0_reg);
        SPI_Gyro_write_reg(int_source0_reg, 0x00);
        SPI_Gyro_write_reg(fifo_config2_reg, 0x00); // watermark
        SPI_Gyro_write_reg(fifo_config3_reg, 0x02); // watermark
        SPI_Gyro_write_reg(int_source0_reg, res);
        SPI_Gyro_write_reg(fifo_config1_reg, 0x63); // Enable the accel and gyro to the FIFO

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        SPI_Gyro_write_reg(int_config_reg, 0x36);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = (SPI_Gyro_read_reg(int_source0_reg) | bit_int_fifo_ths_int1_en);
        SPI_Gyro_write_reg(int_source0_reg, res);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = ((SPI_Gyro_read_reg(accel_config0_reg) & 0x1F) | (bit_accel_ui_fs_sel_8g)); // 8g
        SPI_Gyro_write_reg(accel_config0_reg, res);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = ((SPI_Gyro_read_reg(accel_config0_reg) & 0xF0) | bit_accel_odr_50hz);
        SPI_Gyro_write_reg(accel_config0_reg, res);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = ((SPI_Gyro_read_reg(gyro_config0_reg) & 0x1F) | (bit_gyro_ui_fs_sel_1000dps));
        SPI_Gyro_write_reg(gyro_config0_reg, res);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = ((SPI_Gyro_read_reg(gyro_config0_reg) & 0xF0) | bit_gyro_odr_50hz);
        SPI_Gyro_write_reg(gyro_config0_reg, res);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = SPI_Gyro_read_reg(pwr_mgmt0_reg) | (bit_accel_mode_ln); // Accel on in LNM
        SPI_Gyro_write_reg(pwr_mgmt0_reg, res);
        delay_ms(1);

        SPI_Gyro_write_reg(reg_bank_sel, 0x00);
        res = SPI_Gyro_read_reg(pwr_mgmt0_reg) | (bit_gyro_mode_ln); // Gyro on in LNM
        SPI_Gyro_write_reg(pwr_mgmt0_reg, res);
        GYRO_CS_H;
        delay_ms(1);
        return 0;
    }
    else if ((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        GYRO_CS_L;
        SPI_Gyro_write_reg(LSM6DSR_CTRL1_XL, 0X20);       // 加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
        SPI_Gyro_write_reg(LSM6DSR_CTRL9_XL, 0X38);       // 使能加速度计x,y,z轴
        SPI_Gyro_write_reg(LSM6DSR_CTRL6_C, 0X40 | 0x10); // 陀螺仪电平触发，加速度计高性能使能
        SPI_Gyro_write_reg(LSM6DSR_CTRL7_G, 0X80);        // 陀螺仪高性能使能
        SPI_Gyro_write_reg(LSM6DSR_INT2_CTRL, 0X03);      // 加速度计INT2引脚失能,陀螺仪数据INT2使能
        SPI_Gyro_write_reg(LSM6DSR_CTRL2_G, 0X1C);        // 陀螺仪12.5hz  2000dps
        SPI_Gyro_write_reg(LSM6DSR_CTRL10_C, 0X38);       // 使能陀螺仪x,y,z轴
        if (GyroType == SPI_LQ9AGMV)
        {
            SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x40);
            SPI_Gyro_write_reg(0x14, 0x80);
            delay_ms(1);
            SPI_Gyro_write_reg(0x14, 0x00);
            delay_ms(1);
            SPI_Mag_Write_Reg(0x21, 0x04); // 陀螺仪复位
            delay_ms(5);
            SPI_Mag_Write_Reg(0x21, 0x00);
            delay_ms(5);
            SPI_Mag_Write_Reg(0x20, 0x7f);
            SPI_Mag_Write_Reg(0x23, 0x0c);
            SPI_Mag_Write_Reg(0x22, 0x00);
            SPI_Mag_Write_Reg(0x24, 0x00);
            SPI_Mag_Write_Reg(0x30, 0x02);
            SPI_Gyro_write_reg(0x17, 0x06);
            SPI_Gyro_write_reg(0x15, 0x39);
            SPI_Gyro_write_reg(0x16, 0x28);
            SPI_Gyro_write_reg(0x14, 0x4c);
            while ((SPI_Gyro_read_reg(0x22) & 0x01) == 0)
                ;
            SPI_Gyro_write_reg(0x14, 0x6c);
            SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x00);
        }
        GYRO_CS_H;
        return 0;
    }
    else
    {
        GYRO_CS_L;
        SPI_Gyro_write_reg(ICM_PWR_MGMT1_REG, 0X80);   // 复位ICM20602
        delay_ms(100);                                 // 延时100ms
        SPI_Gyro_write_reg(ICM_PWR_MGMT1_REG, 0X00);   // 唤醒ICM20602
        SPI_Gyro_write_reg(ICM_GYRO_CFG_REG, 3 << 3);  // 陀螺仪传感器,±2000dps
        SPI_Gyro_write_reg(ICM_ACCEL_CFG_REG, 1 << 3); // 加速度传感器,±4g
        SPI_Gyro_write_reg(ICM_SAMPLE_RATE_REG, 0X00); // 设置数字低通滤波器
        SPI_Gyro_write_reg(ICM_CFG_REG, 0X01);         // 设置采样率1000Hz
        SPI_Gyro_write_reg(ICM_CFG_REG, 0X02);         // 设置数字低通滤波器   98hz
        SPI_Gyro_write_reg(ICM_INT_EN_REG, 0X00);      // 关闭所有中断
        SPI_Gyro_write_reg(ICM_USER_CTRL_REG, 0X00);   // I2C主模式关闭
        SPI_Gyro_write_reg(ICM_PWR_MGMT1_REG, 0X01);   // 设置CLKSEL,PLL X轴为参考
        SPI_Gyro_write_reg(ICM_PWR_MGMT2_REG, 0X00);   // 加速度与陀螺仪都工作
        GYRO_CS_H;
        return 0;
    }

    return 1;
}



// 读取陀螺仪的角速度值
void Gyro_SPI_Read_acc(short *accx, short *accy, short *accz)
{
    unsigned char buf[6];
    if (GyroType == SPI_ICM42605)
    {
        SPI_Gyro_Read_Len(ICM_ACCEL_DATA_X1, 6, buf);
        *accx = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *accy = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *accz = (short)(((uint16_t)buf[4] << 8) | buf[5]);
    }
    else if ((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        SPI_Gyro_Read_Len(LSM6DSR_OUTX_L_ACC, 6, buf);
        *accx = (short)(((uint16_t)buf[1] << 8) | buf[0]);
        *accy = (short)(((uint16_t)buf[3] << 8) | buf[2]);
        *accz = (short)(((uint16_t)buf[5] << 8) | buf[4]);
    }
    else
    {
        SPI_Gyro_Read_Len(0x3b, 6, buf);
        *accx = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *accy = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *accz = (short)(((uint16_t)buf[4] << 8) | buf[5]);
    }
}

// 读取陀螺仪的重力加速度
void Gyro_SPI_Read_gyro(short *gyrox, short *gyroy, short *gyroz)
{
    unsigned char buf[6];
    if (GyroType == SPI_ICM42605)
    {
        SPI_Gyro_Read_Len(ICM_GYRO_DATA_X1, 6, buf);
        *gyrox = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *gyroy = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *gyroz = (short)(((uint16_t)buf[4] << 8) | buf[5]);
    }
    else if ((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        SPI_Gyro_Read_Len(LSM6DSR_OUTX_L_GYRO, 6, buf);
        *gyrox = (short)(((uint16_t)buf[1] << 8) | buf[0]);
        *gyroy = (short)(((uint16_t)buf[3] << 8) | buf[2]);
        *gyroz = (short)(((uint16_t)buf[5] << 8) | buf[4]);
    }
    else
    {
        SPI_Gyro_Read_Len(0x3b + 8, 6, buf);
        *gyrox = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *gyroy = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *gyroz = (short)(((uint16_t)buf[4] << 8) | buf[5]);
    }
}
// 读取9轴陀螺仪LQ9AGMV的3轴磁力计
void Gyro_Get_Mag_data(short *magx, short *magy, short *magz)
{
    unsigned char buf[6];
    if (GyroType == SPI_LQ9AGMV)
    {
        SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x40);
        SPI_Gyro_Read_Len(0x02, 6, buf);
        *magx = (short)(((uint16_t)buf[1] << 8) | buf[0]);
        *magy = (short)(((uint16_t)buf[3] << 8) | buf[2]);
        *magz = (short)(((uint16_t)buf[5] << 8) | buf[4]);
        SPI_Gyro_write_reg(LSM6DSR_FUNC_CFG, 0x00);
    }
}


/*****************************************************************************************
 * 函 数 名： void Gyro_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
 * 函数功能：同时读取陀螺仪的重力加速度和角速度
 * 输入参数： ax, ay, az, gx, gy, gz 返回数据的指针
 * 返 回 值： 无
 * 调用示例： Gyro_Get_Raw_data(&ax, &ay, &az, &gx, &gy, &gz);
 * 注意事项： 无
 * ***************************************************************************************/
void Gyro_Get_Raw_data(short *ax, short *ay, short *az, short *gx, short *gy, short *gz)
{
    unsigned char buf[15];
    if (GyroType == SPI_ICM42605)
    {
        SPI_Gyro_Read_Len(ICM_ACCEL_DATA_X1, 12, buf);
        *ax = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *ay = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *az = (short)(((uint16_t)buf[4] << 8) | buf[5]);
        *gx = (short)(((uint16_t)buf[6] << 8) | buf[7]);
        *gy = (short)(((uint16_t)buf[8] << 8) | buf[9]);
        *gz = (short)(((uint16_t)buf[10] << 8) | buf[11]);
    }
    else if ((GyroType == SPI_LSM6DSR) || (GyroType == SPI_LQ9AGMV))
    {
        SPI_Gyro_Read_Len(LSM6DSR_OUTX_L_GYRO, 12, buf);
        *gx = (short)(((uint16_t)buf[1] << 8) | buf[0]);
        *gy = (short)(((uint16_t)buf[3] << 8) | buf[2]);
        *gz = (short)(((uint16_t)buf[5] << 8) | buf[4]);
        *ax = (short)(((uint16_t)buf[7] << 8) | buf[6]);
        *ay = (short)(((uint16_t)buf[9] << 8) | buf[8]);
        *az = (short)(((uint16_t)buf[11] << 8) | buf[10]);
    }
    else
    {
        SPI_Gyro_Read_Len(0x3b, 14, buf);
        *ax = (short)(((uint16_t)buf[0] << 8) | buf[1]);
        *ay = (short)(((uint16_t)buf[2] << 8) | buf[3]);
        *az = (short)(((uint16_t)buf[4] << 8) | buf[5]);
        *gx = (short)(((uint16_t)buf[8] << 8) | buf[9]);
        *gy = (short)(((uint16_t)buf[10] << 8) | buf[11]);
        *gz = (short)(((uint16_t)buf[12] << 8) | buf[13]);
    }
}

#ifdef USE_HAL_SPI
/*******************************************************************************
* 函 数 名 ：void Test_HAL_SPI_Gyro()
* 描    述 ：硬件SPI陀螺仪测试函数
* 输    入 ：无
* 输    出 ：无
* 备    注 ：
********************************************************************************/
//void Test_HAL_SPI_Gyro(void)
//{
//    char txt[30];
//    short accx = 0, accy = 0, accz = 0;    // 加速度传感器原始数据
//    short gyrox = 0, gyroy = 0, gyroz = 0; // 陀螺仪原始数据
//    short magx = 0, magy = 0, magz = 0;    // 9轴陀螺仪的磁力计原始数据

//    Gyro_SPI_Init(); // SPI陀螺仪通用初始化函数 内部判断使用软件还是硬件方式
//    
//#ifdef USE_OLED
//    GPIO_LED_Init();
//    
//    
//#else
//    LCD_Init(); // LCD初始化  0:横屏  1：竖屏
//    LCD_CLS(BLACK);



//    while (1)
//    {
//        Gyro_SPI_Read_acc(&accx, &accy, &accz);     // 读取陀螺仪角速度
//        Gyro_SPI_Read_gyro(&gyrox, &gyroy, &gyroz); // 读取陀螺仪重力加速度

//        // Gyro_Get_Raw_data(&accx,&accy,&accz,&gyrox,&gyroy,&gyroz); //同时读取角速度和加速度

//        sprintf((char *)txt, "ax:%06d", accx);
//        LCD_ShowStr_16(0, 1, txt, u16RED, u16BLACK); // 字符串显示
//        sprintf((char *)txt, "ay:%06d", accy);
//        LCD_ShowStr_16(0, 2, txt, u16RED, u16BLACK); // 字符串显示
//        sprintf((char *)txt, "az:%06d", accz);
//        LCD_ShowStr_16(0, 3, txt, u16RED, u16BLACK); // 字符串显示
//        sprintf((char *)txt, "gx:%06d", gyrox);
//        LCD_ShowStr_16(0, 4, txt, u16RED, u16BLACK); // 字符串显示
//        sprintf((char *)txt, "gy:%06d", gyroy);
//        LCD_ShowStr_16(0, 5, txt, u16RED, u16BLACK); // 字符串显示
//        sprintf((char *)txt, "gz:%06d", gyroz);
//        LCD_ShowStr_16(0, 6, txt, u16RED, u16BLACK); // 字符串显示

//        if (GyroType == SPI_LQ9AGMV)
//        { // 9轴陀螺仪地磁计数据读取
//            Gyro_Get_Mag_data(&magx, &magy, &magz);
//            sprintf((char *)txt, "mx:%06d", magx);
//            LCD_ShowStr_16(0, 7, txt, u16RED, u16BLACK); // 字符串显示
//            sprintf((char *)txt, "my:%06d", magy);
//            LCD_ShowStr_16(0, 8, txt, u16RED, u16BLACK); // 字符串显示
//            sprintf((char *)txt, "mz:%06d", magz);
//            LCD_ShowStr_16(0, 9, txt, u16RED, u16BLACK); // 字符串显示
//        }

//        LED_Ctrl(LED0, RVS);
//        delay_ms(10);
//    }
//  #endif
//    
//}

#else  //USE_SOFT_SPI


void Test_SOFT_SPI_Gyro(void)
 {
     signed short accelerat_X, accelerat_Y, accelerat_Z;
     signed short gyro_X, gyro_Y, gyro_Z;
     unsigned char res;
     char  txt[30];

     Gyro_SPI_Init();      //函数内判断宏选择硬件还是软件SPI方式

 	 delay_ms(500);
     OLED_Init();                //LCD初始化
     OLED_CLS();                 //LCD清屏

     res = Gyro_Chose_SPI();
     OLED_P14x16Str(8,0,(unsigned char*)"北京龙邱智能科技");		//字符串显示
     sprintf(txt,"ID:0X%2x",res);
     OLED_P6x8Str(70,2,txt);     // 字符串显示
     if(res == 0xff)
     {
         sprintf(txt,"Gyro init error",res);
         OLED_P6x8Str(5,4,txt);     // 字符串显示
         while(1);
     }

     Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X80);//复位
     delay_ms(100);  //延时100ms
     Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X00);//唤醒
     delay_ms(100);  //延时100ms

     Gyro_Write_Byte_SPI(ICM_GYRO_CFG_REG,3<<3);//设置陀螺仪满量程范围
     Gyro_Write_Byte_SPI(ICM_ACCEL_CFG_REG,1<<3);//设置加速度传感器满量程范围
     Gyro_Write_Byte_SPI(ICM_SAMPLE_RATE_REG,1000/999);	//设置数字低通滤波器
     Gyro_Write_Byte_SPI(ICM_CFG_REG,1);//设置数字低通滤波器
     Gyro_Write_Byte_SPI(ICM_CFG_REG,0x02);      //设置数字低通滤波器   98hz
     Gyro_Write_Byte_SPI(ICM_INT_EN_REG,0X00);   //关闭所有中断
     Gyro_Write_Byte_SPI(ICM_USER_CTRL_REG,0X00);//I2C主模式关闭
     Gyro_Write_Byte_SPI(ICM_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
     Gyro_Write_Byte_SPI(ICM_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

     while(1)
     {
         Gyro_Get_Raw_data_SPI(&accelerat_X, &accelerat_Y, &accelerat_Z, &gyro_X, &gyro_Y, &gyro_Z);
         sprintf((char*)txt,"ax:%06d",accelerat_X);
         OLED_P6x8Str(0,2,txt);
         sprintf((char*)txt,"ay:%06d",accelerat_Y);
         OLED_P6x8Str(0,3,txt);
         sprintf((char*)txt,"az:%06d",accelerat_Z);
         OLED_P6x8Str(0,4,txt);
         sprintf((char*)txt,"gx:%06d",gyro_X);
         OLED_P6x8Str(0,5,txt);
         sprintf((char*)txt,"gy:%06d",gyro_Y);
         OLED_P6x8Str(0,6,txt);
         sprintf((char*)txt,"gz:%06d",gyro_Z);
         OLED_P6x8Str(0,7,txt);
         delay_ms(20);
     }
 }


#endif      //USE_HAL_SPI  or USE_SOFT_SPI