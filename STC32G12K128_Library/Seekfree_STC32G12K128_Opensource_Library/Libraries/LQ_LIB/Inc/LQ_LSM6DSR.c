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
#include "include.h"

#include "LQ_LSM6DSR.h"

bit LSM60D_9AGM_FLAG = 0; // 是否读取3轴地磁，默认0，仅使用6轴不读，即LSM60DSR，1为9轴方式需要读取

unsigned char LQ_IIC_LSM60DSR_GetID(unsigned char dev, unsigned char reg)
{
    unsigned char ID[1];
#ifdef USE_SOFT_I2C
    IIC_ReadMultByteFromSlave(dev, reg, 1, ID);
#else
    Hard_I2C_ReadNbyte(dev, reg, ID, 1); // 硬件IIC读取一个字节
#endif
    return ID[0];
}

void LQ_IIC_LSM60DSR_Init(void)
{
#ifdef USE_SOFT_I2C
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL1_XL, 0X20);       // 加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL9_XL, 0X38);       // 使能加速度计x,y,z轴
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL6_C, 0X40 | 0x10); // 陀螺仪电平触发，加速度计高性能使能
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL7_G, 0X80);        // 陀螺仪高性能使能
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_INT2_CTRL, 0X03);      // 加速度计INT2引脚失能,陀螺仪数据INT2使能
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL2_G, 0X1C);        // 陀螺仪12.5hz  2000dps
    IIC_WriteByteToSlave(LSM6DSR_ADDR, LSM6DSR_CTRL10_C, 0X38);       // 使能陀螺仪x,y,z轴
#else
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL1_XL, 0X20, 1);         // 加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL9_XL, 0X38, 1);         // 使能加速度计x,y,z轴
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL6_C, (0X40 | 0x10), 2); // 陀螺仪电平触发，加速度计高性能使能
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL7_G, 0X80, 1);          // 陀螺仪高性能使能
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_INT2_CTRL, 0X03, 1);        // 加速度计INT2引脚失能,陀螺仪数据INT2使能
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL2_G, 0X1C, 1);          // 陀螺仪12.5hz  2000dps
    Hard_I2C_WriteNbyte(LSM6DSR_ADDR, LSM6DSR_CTRL10_C, 0X38, 1);         // 使能陀螺仪x,y,z轴
#endif
    //    //WAKE_UP INTERRUPT Configuration
    //    MPU_Write_Byte(LSM6DSR_ADDR, LSM6DSR_CTRL1_XL,0x60);
    //    MPU_Write_Byte(TAP_CFG,0x90);
    //    MPU_Write_Byte(WAKE_UP_DUR,0x00);
    //    MPU_Write_Byte(WAKE_UP_THS,0x02);
    //    MPU_Write_Byte(MD1_CFG,0x20);
    //    //6D Orientation Configuration
    //    MPU_Write_Byte(TAP_THS_6D,0x40);
    //    MPU_Write_Byte(CTRL8_XL,0x01);
}

void LQ_IIC_LSM60DSR_Read(signed short *ax, signed short *ay, signed short *az, signed short *gx, signed short *gy, signed short *gz)
{
    unsigned char buf[12];
#ifdef USE_SOFT_I2C
    IIC_ReadMultByteFromSlave(LSM6DSR_ADDR, LSM6DSR_OUTX_L_GYRO, 12, buf);
#else
    Hard_I2C_ReadNbyte(LSM6DSR_ADDR, LSM6DSR_OUTX_L_GYRO, buf, 12); // 硬件IIC读取一个字节
#endif
    *gx = ((uint16)buf[1] << 8) | buf[0];
    *gy = ((uint16)buf[3] << 8) | buf[2];
    *gz = ((uint16)buf[5] << 8) | buf[4];
    *ax = ((uint16)buf[7] << 8) | buf[6];
    *ay = ((uint16)buf[9] << 8) | buf[8];
    *az = ((uint16)buf[11] << 8) | buf[10];
}

void Test_I2C_LQLSM6DSR(void)
{
    signed short acc_X, acc_Y, acc_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
    // signed short mag_X, mag_Y, mag_Z;
    unsigned char res;
    char txt[24];

#ifdef USE_SOFT_I2C
    Soft_I2C_Init(); // IIC初始化//IIC初始化
//    delay_ms(100);
#else
    Hard_IIC_Init(0); // IIC初始化
#endif

    OLED_Init(); // LCD初始化
    delay_ms(100);
    OLED_CLS(); // LCD清屏

    /*读取设备ID*/
    res = LQ_IIC_LSM60DSR_GetID(LSM6DSR_ADDR, WHO_AM_LSM6D);

    if (res == DRV_ID_LSM6D)
    {
        OLED_P8x16Str(0, 0, "LQ6DSR Test"); // 字符串显示
    }
    else {
        OLED_P8x16Str(0, 0, "get ID error"); // 字符串显示
    }

    sprintf(txt, " 0X%2X", res);
    OLED_P6x8Str(74, 6, "ID:"); // 显示字符串
    OLED_P6x8Str(88, 7, txt);   // 显示字符串

    LQ_IIC_LSM60DSR_Init();
    while (1)
    {
        LQ_IIC_LSM60DSR_Read(&acc_X, &acc_Y, &acc_Z, &gyro_X, &gyro_Y, &gyro_Z);
        /*屏幕显示选择*/

        sprintf((char *)txt, "ax:%06d", acc_X);
        OLED_P6x8Str(0, 2, txt);
        sprintf((char *)txt, "ay:%06d", acc_Y);
        OLED_P6x8Str(0, 3, txt);
        sprintf((char *)txt, "az:%06d", acc_Z);
        OLED_P6x8Str(0, 4, txt);
        sprintf((char *)txt, "gx:%06d", gyro_X);
        OLED_P6x8Str(0, 5, txt);
        sprintf((char *)txt, "gy:%06d", gyro_Y);
        OLED_P6x8Str(0, 6, txt);
        sprintf((char *)txt, "gz:%06d", gyro_Z);
        OLED_P6x8Str(0, 7, txt);

        delay_ms(50);
    }
}

/*************************************************************************************************************************************
 *                                                       以下为SPI通信模式   尚未完成24-11-22
 * ************************************************************************************************************************************/

unsigned char LQ_SPI_LSM60DSR_GetID(unsigned char reg)
{
    unsigned char buff;

#ifdef USE_SOFT_SPI
    LQ_SPI_Read(reg, 1, &buff);
#else
    SPI_Read_NByte((reg | 0x80), &buff, 1);
#endif
    return buff;
}

// 向寄存器中读写数据
void SPI_Write_reg(u8 reg, u8 dat)
{
#ifdef USE_SOFT_SPI
    LQ_SPI_Write(reg, dat);
#else
    SPI_WriteByte(reg);
    SPI_WriteByte(dat);
#endif
}

// 从寄存器中读取数据
uint8_t SPI_Read_reg(u8 reg)
{
    u8 buff;

#ifdef USE_SOFT_SPI
    LQ_SPI_Read(reg, 1, &buff);
#else
    SPI_Read_NByte((reg | 0x80), &buff, 1);
#endif
    return buff;
}

// 从寄存器中读取数据
void SPI_Read_buf(u8 reg, u8 *buf, u8 len)
{
#ifdef USE_SOFT_SPI
    LQ_SPI_Read(reg, len, buf);
#else
    SPI_Read_NByte((reg | 0x80), buf, len);
#endif
}

// 9轴陀螺仪地磁计的寄存器写数据
uint8_t SPI_Mag_Write_Reg(uint8_t addr, uint8_t dat)
{
    uint32_t count = 0;
    SPI_Write_reg(0x17, 0x00);
    SPI_Write_reg(0x15, 0x38);
    SPI_Write_reg(0x16, addr);
    SPI_Write_reg(0x21, dat);
    SPI_Write_reg(0x14, 0x4c);

    while ((SPI_Read_reg(0x22) & 0x80) == 0) // 此处做检查是否写动作完成
    {
        if (1000 < count++)
        {
            return 1;
        }
        delay_us(100);
    }
    return 0;
}

// 从9轴陀螺仪地磁计的寄存器读数据
uint8_t SPI_Mag_Read_Reg(uint8_t addr)
{
    uint16_t count = 0;
    SPI_Write_reg(0x17, 0x01);
    SPI_Write_reg(0x15, 0x39);
    SPI_Write_reg(0x16, addr);
    SPI_Write_reg(0x14, 0x4c);
    while ((SPI_Read_reg(0x22) & 0x01) == 0) // 此处做检查是否读动作完成
    {
        if (1000 < count++)
        {
            break;
        }
        delay_us(100);
    }
    return SPI_Read_reg(0x02); // 6DSR的缓冲数据区地址，用于存放挂载的3轴地磁
}

// 6轴陀螺仪LSM6DSR和九轴陀螺仪LQ9AGMV的检测
uint8_t Gyro_LSM6DSR_LQ9AGMV_Check(void)
{
    uint8_t res, i;
    SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x00); // 切换LSM6D寄存器来连接挂载的3轴地磁
    //    SPI_Write_reg(LSM6DSR_CTRL3_C, 0x01);
    //    delay_ms(2);
    //    SPI_Write_reg(LSM6DSR_FUNC_CFG,0x00);

    res = LQ_SPI_LSM60DSR_GetID(WHO_AM_LSM6D);

    if (res == DRV_ID_LSM6D)
    {
        for (i = 0; i <= 3; i++)
        {
            SPI_Write_reg(0x14, 0x80);
            delay_ms(5);
            SPI_Write_reg(0x14, 0x00);
            delay_ms(5);
            SPI_Write_reg(LSM6DSR_CTRL1_XL, 0X20);
            SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x40); // 切换到挂载总线读取3轴地磁
            if (SPI_Mag_Read_Reg(0x0f) == Mag_ID)
            {
                res = Mag_ID;         // 若能读到3轴地磁ID，则为9AGM，否则可视为6轴的LSM6D
                LSM60D_9AGM_FLAG = 1; // 标志1，为9轴模式
                break;                // 读到后，退出循环
            }
        }
        SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x00);
    }

    return res;
}

void LQ_SPI_LSM60DSR_Init(void)
{
    u8 res;
    uint16_t count = 0;
#ifdef USE_SOFT_SPI
    Soft_SPI_Init(); // 软件SPI初始化
#else                // 使用硬件SPI方式
    HSPI_Init();
#endif

    SPI_Write_reg(LSM6DSR_CTRL1_XL, 0X20);       // 加速度计52HZ（倾斜角检测功能工作在26HZ，因此加速度计ODR必须设置为>=26hz）,2g量程。
    SPI_Write_reg(LSM6DSR_CTRL9_XL, 0X38);       // 使能加速度计x,y,z轴
    SPI_Write_reg(LSM6DSR_CTRL6_C, 0X40 | 0x10); // 陀螺仪电平触发，加速度计高性能使能
    SPI_Write_reg(LSM6DSR_CTRL7_G, 0X80);        // 陀螺仪高性能使能
    SPI_Write_reg(LSM6DSR_INT2_CTRL, 0X03);      // 加速度计INT2引脚失能,陀螺仪数据INT2使能
    SPI_Write_reg(LSM6DSR_CTRL2_G, 0X1C);        // 陀螺仪12.5hz  2000dps
    SPI_Write_reg(LSM6DSR_CTRL10_C, 0X38);       // 使能陀螺仪x,y,z轴

    delay_ms(5);
    res = Gyro_LSM6DSR_LQ9AGMV_Check();
    if (res == DRV_ID_LSM6D)
    {
        //        OLED_P8x16Str(0,0,"LQ6DSR Test");// 字符串显示
    }
    else if (res == Mag_ID)
    {
        // ------------------------ 初始化3轴地磁 --------------------------
        SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x40);
        SPI_Write_reg(0x14, 0x80);
        delay_ms(2);
        SPI_Write_reg(0x14, 0x00);
        delay_ms(2);
        SPI_Mag_Write_Reg(0x21, 0x04); // 陀螺仪复位
        delay_ms(5);
        SPI_Mag_Write_Reg(0x21, 0x00);
        delay_ms(5);
        SPI_Mag_Write_Reg(0x20, 0x7f);
        SPI_Mag_Write_Reg(0x23, 0x0c);
        SPI_Mag_Write_Reg(0x22, 0x00);
        SPI_Mag_Write_Reg(0x24, 0x00);
        SPI_Mag_Write_Reg(0x30, 0x02);
        SPI_Write_reg(0x17, 0x06);
        SPI_Write_reg(0x15, 0x39);
        SPI_Write_reg(0x16, 0x28);
        SPI_Write_reg(0x14, 0x4c);

        //        while((SPI_Read_reg(0x22) & 0x01) == 0)
        //        {
        //            if(1000 < count++){
        //                break;
        //            }
        //            delay_us(100);
        //        }
        SPI_Write_reg(0x14, 0x6c);
        SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x00);
        //    OLED_P8x16Str(0,0,"LQ9AGM Test");// 字符串显示
    }
}

// 读取6轴陀螺仪数据
void LQ_SPI_LSM60DSR_Read(signed short *ax, signed short *ay, signed short *az, signed short *gx, signed short *gy, signed short *gz)
{
    unsigned char buf[12];

    SPI_Read_buf(LSM6DSR_OUTX_L_GYRO, buf, 12);

    *gx = ((uint16)buf[1] << 8) | buf[0];
    *gy = ((uint16)buf[3] << 8) | buf[2];
    *gz = ((uint16)buf[5] << 8) | buf[4];
    *ax = ((uint16)buf[7] << 8) | buf[6];
    *ay = ((uint16)buf[9] << 8) | buf[8];
    *az = ((uint16)buf[11] << 8) | buf[10];
}

// 读取9轴陀螺仪LQ9AGMV的3轴磁力计
void Get_Mag_data(signed short *magx, signed short *magy, signed short *magz)
{
    unsigned char buf[6];
    SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x40);
    SPI_Read_buf(0x02, buf, 6);
    *magx = ((signed)(uint16_t)buf[1] << 8) | buf[0];
    *magy = ((signed)(uint16_t)buf[3] << 8) | buf[2];
    *magz = ((signed)(uint16_t)buf[5] << 8) | buf[4];
    SPI_Write_reg(LSM6DSR_FUNC_CFG, 0x00);
}

void Test_SPI_LQLSM6DSR()
{
    signed short acc_X, acc_Y, acc_Z;
    signed short gyro_X, gyro_Y, gyro_Z;
//    signed short mag_X, mag_Y, mag_Z;
    unsigned char res;
    char txt[30];

    LQ_SPI_LSM60DSR_Init(); // 函数内判断软件还是硬件SPI
    OLED_Init();            // LCD初始化
    delay_ms(100);
    OLED_CLS(); // LCD清屏
    /*读取设备ID   屏幕显示选择 */
    //   res = LQ_SPI_LSM60DSR_GetID(WHO_AM_LSM6D);   //函数内判断软件还是硬件SPI
    res = Gyro_LSM6DSR_LQ9AGMV_Check();
    if (res == DRV_ID_LSM6D)
    {
        OLED_P8x16Str(0, 0, "LQ6DSR Test"); // 字符串显示
    }
    else if (res == Mag_ID)
    {
        OLED_P8x16Str(0, 0, "LQ9AGM Test"); // 字符串显示
    }
     else{
        OLED_P8x16Str(0, 0, "get ID error"); // 字符串显示
    }

    sprintf(txt, " 0X%2X", res);
    OLED_P6x8Str(74, 6, "ID:"); // 显示字符串
    OLED_P6x8Str(88, 7, txt);   // 显示字符串

    while (1)
    {
        LQ_SPI_LSM60DSR_Read(&acc_X, &acc_Y, &acc_Z, &gyro_X, &gyro_Y, &gyro_Z);

        sprintf((char *)txt, "ax:%06d", acc_X);
        OLED_P6x8Str(0, 2, txt);
        sprintf((char *)txt, "ay:%06d", acc_Y);
        OLED_P6x8Str(0, 3, txt);
        sprintf((char *)txt, "az:%06d", acc_Z);
        OLED_P6x8Str(0, 4, txt);
        sprintf((char *)txt, "gx:%06d", gyro_X);
        OLED_P6x8Str(0, 5, txt);
        sprintf((char *)txt, "gy:%06d", gyro_Y);
        OLED_P6x8Str(0, 6, txt);
        sprintf((char *)txt, "gz:%06d", gyro_Z);
        OLED_P6x8Str(0, 7, txt);

//        if (LSM60D_9AGM_FLAG) // 9轴模式下读取地磁数据
//        {
//            Get_Mag_data(&mag_X, &mag_Y, &mag_Z);

//            sprintf((char *)txt, "mx:%06d", mag_X);
//            OLED_P6x8Str(64, 2, txt);
//            sprintf((char *)txt, "my:%06d", mag_Y);
//            OLED_P6x8Str(64, 3, txt);
//            sprintf((char *)txt, "mz:%06d", mag_Z);
//            OLED_P6x8Str(64, 4, txt);
//        }

        LED_Ctrl(LED0, RVS); // 电平翻转,LED闪烁
        delay_ms(5);
    }
}
