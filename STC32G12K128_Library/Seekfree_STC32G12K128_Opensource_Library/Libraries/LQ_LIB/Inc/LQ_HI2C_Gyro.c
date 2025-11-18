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

#include "LQ_HI2C_Gyro.h"


//extern signed short  aacx,aacy,aacz;            //加速度传感器原始数据
//extern signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据
//extern short magx, magy, magz;


#define Dev_add 0xD0
Dev dev_Name;

/**************************************************************************
  * @brief   陀螺仪测试函数
  * @param   void
  * @retur   void
  * @see      Test_HardIIC_Gyro();
  * @date     2024/05/21 星期二
	* 备注  测试陀螺仪加速度计，采用硬件IIC方式，可以修改IIC初始化函数的入口参数 选择不同的引脚
**************************************************************************/
void Test_HardIIC_Gyro(void)
{
    signed short  aacx,aacy,aacz;            //加速度传感器原始数据
    signed short  gyrox,gyroy,gyroz;         //陀螺仪原始数据
    short magx, magy, magz;
	unsigned char DATH[6], DATL[6];
	unsigned char ID;
	char txt[64];

	//硬件IIC初始化，0:使用P2.4与P2.3引脚   1：使用P1.5 P1.4引脚  2：使用P3.2与P3.3引脚
	Hard_IIC_Init(1);

	OLED_CLS();                 //LCD清屏

	ID = Get_Slave_ID();				//检测型号
	sprintf(txt,"ID:0X%2x",ID);
	OLED_P6x8Str(70,2,txt);     // 字符串显示ID

	switch(dev_Name)
	{
		case ICM20602:OLED_P8x16Str(0,0,"LQ ICM20602 Test"); break;// 字符串显示
		case MPU9250:OLED_P8x16Str(0,0,"LQ MPU9250 Test"); break;// 字符串显示
		case ICM20689:OLED_P8x16Str(0,0,"LQ ICM20689 Test"); break;// 字符串显示
		case ICM42605:OLED_P8x16Str(0,0,"LQ ICM42605 Test"); break;// 字符串显示
		case MPU6050:OLED_P8x16Str(0,0,"LQ MPU6050 Test"); break;// 字符串显示
		case errorid:OLED_P8x16Str(0,0,"LQ Test fail"); while(1);// 字符串显示
	}

	Gyro_Init();

	while(1)
	{
		//得到加速度传感器数据
		DATH[0] = Hard_IIC_Read_MPU_Byte(Dev_add,0x3B);DATL[0] = Hard_IIC_Read_MPU_Byte(Dev_add,0x3C);
		DATH[1] = Hard_IIC_Read_MPU_Byte(Dev_add,0x3D);DATL[1] = Hard_IIC_Read_MPU_Byte(Dev_add,0x3E);
		DATH[2] = Hard_IIC_Read_MPU_Byte(Dev_add,0x3F);DATL[2] = Hard_IIC_Read_MPU_Byte(Dev_add,0x40);
		DATH[3] = Hard_IIC_Read_MPU_Byte(Dev_add,0x43);DATL[3] = Hard_IIC_Read_MPU_Byte(Dev_add,0x44);
		DATH[4] = Hard_IIC_Read_MPU_Byte(Dev_add,0x45);DATL[4] = Hard_IIC_Read_MPU_Byte(Dev_add,0x46);
		DATH[5] = Hard_IIC_Read_MPU_Byte(Dev_add,0x47);DATL[5] = Hard_IIC_Read_MPU_Byte(Dev_add,0x48);
		//数据转换
		aacx = DATH[0]; aacx = (aacx<<8)|DATL[0];
		aacy = DATH[1]; aacy = (aacy<<8)|DATL[1];
		aacz = DATH[2]; aacz = (aacz<<8)|DATL[2];
		gyrox = DATH[3]; gyrox = (gyrox<<8)|DATL[3];
		gyroy = DATH[4]; gyroy = (gyroy<<8)|DATL[4];
		gyroz = DATH[5]; gyroz = (gyroz<<8)|DATL[5];

		if(dev_Name == MPU9250)
		{
			DATH[0] = Hard_IIC_Read_MPU_Byte(Dev_add,0X03);DATL[0] = Hard_IIC_Read_MPU_Byte(Dev_add,0X04);
			DATH[1] = Hard_IIC_Read_MPU_Byte(Dev_add,0X05);DATL[1] = Hard_IIC_Read_MPU_Byte(Dev_add,0X06);
			DATH[2] = Hard_IIC_Read_MPU_Byte(Dev_add,0X07);DATL[2] = Hard_IIC_Read_MPU_Byte(Dev_add,0X08);
			magx = DATH[0]; magx = (magx<<8)|DATL[0];
			magy = DATH[1]; magy = (magy<<8)|DATL[1];
			magz = DATH[2]; magz = (magz<<8)|DATL[2];
			sprintf((char*)txt,"gx:%06d",gyrox);
			OLED_P6x8Str(70,5,txt);
			sprintf((char*)txt,"gy:%06d",gyroy);
			OLED_P6x8Str(70,6,txt);
			sprintf((char*)txt,"gz:%06d",gyroz);
			OLED_P6x8Str(70,7,txt);
		}
		sprintf((char*)txt,"ax:%06d",aacx);
		OLED_P6x8Str(0,2,txt);
		sprintf((char*)txt,"ay:%06d",aacy);
		OLED_P6x8Str(0,3,txt);
		sprintf((char*)txt,"az:%06d",aacz);
		OLED_P6x8Str(0,4,txt);
		sprintf((char*)txt,"gx:%06d",gyrox);
		OLED_P6x8Str(0,5,txt);
		sprintf((char*)txt,"gy:%06d",gyroy);
		OLED_P6x8Str(0,6,txt);
		sprintf((char*)txt,"gz:%06d",gyroz);
		OLED_P6x8Str(0,7,txt);

		LED_Ctrl(LED0,RVS);        //LED闪烁
		delay_ms(100);
	}
}

/**************************************************************************
  * @brief   读取陀螺仪的设备ID
  * @param   void
  * @retur   ID
  * @see      Get_Slave_ID();
  * @date     2024/05/21 星期二
**************************************************************************/
unsigned char Get_Slave_ID(void)
{
	unsigned char ID;

	//读取设备ID，所有陀螺仪的ID都是0xD0,并且存放ID的寄存器都是0X7F
	ID = Hard_IIC_Read_MPU_Byte(Dev_add,WHO_AM_I);
	switch(ID)
	{
		case 0x09:dev_Name = ICM20602;break;
		case 0x38:dev_Name = MPU9250 ;break;
		case 0x4C:dev_Name = ICM20689;break;
		case 0x21:dev_Name = ICM42605;break;
		case 0x34:dev_Name = MPU6050 ;break;
		default :dev_Name = errorid ;return 0;
	}
	return ID;
}


/**************************************************************************
  * @brief    初始化陀螺仪
  * @param    无
  * @return   0：初始化成功   1：失败
  * @note     使用前先初始化IIC接口
  * @see      MPU6050_Init();
  * @date     2019/6/12 星期三
  *************************************************************************/
unsigned char Gyro_Init(void)
{
	unsigned char res;
	unsigned char  dat, Accel_Fsr = 1, Gyro_Fsr = 3;
	int rate = 1000;
	switch(dev_Name)
	{
		case ICM42605:
        {
            OLED_P8x16Str(0,3,"ICM42605 NO CARE");
//            while(1);
        }break;
		case MPU9250:
        {
            Hard_IIC_Write_MPU_Byte(Dev_add,MPU_FIFO_EN_REG,0X00);  //关闭FIFO
            Hard_IIC_Write_MPU_Byte(Dev_add,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式
            res = Hard_IIC_Read_MPU_Byte(AK8963_ADDR,MPU_MAG_WIA);   //读取MPU9250的ID
            if(res == AK8963_ID)                                //器件ID正确
            {
                Hard_IIC_Write_MPU_Byte(AK8963_ADDR,MPU_MAG_CNTL2_REG,0X01);//软件重置
                delay_ms(100);
                Hard_IIC_Write_MPU_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X16);//设置为单次模式，输出16bit
            }
        }break;
		case ICM20602:
        {}break;
		case ICM20689:
        {}break;
		case MPU6050:
        {
            res = 0;
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_PWR_MGMT1_REG,0X80);//复位MPU6050
            delay_ms(100);  //延时100ms
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU6050
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_GYRO_CFG_REG,Gyro_Fsr<<3);					        	   //陀螺仪传感器,±2000dps
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_ACCEL_CFG_REG,Accel_Fsr<<3);					       	   //加速度传感器,±4g
            if(rate>1000) rate = 1000;
            if(rate<4) rate=4;
            dat = 1000/rate-1;
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_SAMPLE_RATE_REG,dat);	    //设置数字低通滤波器  设置采样率1000Hz
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_CFG_REG,0x02);      //设置数字低通滤波器   98hz
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_INT_EN_REG,0X00);   //关闭所有中断
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
            res += Hard_IIC_Write_MPU_Byte(Dev_add,MPU_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作

        }break;
        case errorid: break;

	}
	return 0;
}


////////////////////////////////////驱动程序////////////////////////////////////////////////


/**************************************************************************
  * @brief    陀螺仪 读一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  *
  * @return   读取的值
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      Hard_IIC_Read_MPU_Byte(Dev_add,WHO_AM_I);
  *
  * @date     2019/6/12 星期三
**************************************************************************/
unsigned char Hard_IIC_Read_MPU_Byte(unsigned char addr,unsigned char reg)
{
	unsigned char res, Read_addr;
	Read_addr = addr+1;
	Hard_IIC_Start();							//起始信号
	Hard_IIC_SendData(addr);			//发送设备地址
	Hard_IIC_RecvACK();						//等待回复
	Hard_IIC_SendData(reg);				//发送寄存器地址
	Hard_IIC_RecvACK();						//等待回复

	Hard_IIC_Start();							//发送起始信号
	Hard_IIC_SendData(Read_addr);	//发送设备地址+1 （为读取该地址的信号）
	res = Hard_IIC_RecvData();		//读取数据
	Hard_IIC_SendNAK();						//反馈信号
	Hard_IIC_Stop();							//停止信号

	return res;
}



/**************************************************************************
  * @brief    陀螺仪 写一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  * @param    value :要写入的值
  *
  * @return   0：写成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      Hard_IIC_Write_MPU_Byte(Dev_add,MPU_PWR_MGMT2_REG,0X00);
  *
  * @date     2019/6/12 星期三
**************************************************************************/
unsigned char Hard_IIC_Write_MPU_Byte(unsigned char addr,unsigned char reg, unsigned char value)
{
	Hard_IIC_Start();							//起始信号
	Hard_IIC_SendData(addr);			//发送设备地址
	Hard_IIC_RecvACK();						//等待回复
	Hard_IIC_SendData(reg);				//发送寄存器地址
	Hard_IIC_RecvACK();						//等待回复
	Hard_IIC_SendData(value);				//发送寄存器地址
	Hard_IIC_RecvACK();						//等待回复
	Hard_IIC_Stop();							//停止信号

	return 0;
}












