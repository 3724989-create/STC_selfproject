/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32位核心板
 【编    写】龙邱科技
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 --------------------------------------------------------------------------------
 【  IDE  】 keil C251 V5.60
 【Target 】 STC32G/STC8051U/AI8051U 32位模式
 【SYS CLK】 42 MHz使用内部晶振
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


//下载时, 选择时钟频率 与"config.h"中宏定义保持一致 默认使用42M主频
//实际应用情况中，可能有的外设初始化函数、宏定义不会被用到，为节省8051U空间资源提升效率，可以合理选择将注释掉
//【Target 】 STC32G/AI8051U_32Bit 32位模式，切换芯片后

#include "include.h"



void System_Init(void);         //上电初始化配置

/***************************************************************************
* 函     数: void main(void)
* 描     述: 函数入口
* 参     数: None.
* 返     回: None.
* 版     本: V1.0, 2024-11-01
****************************************************************************/
void main(void)
{
    u16 i = 0;
    System_Init();
    
    UART_Init(UART1,115200ul);      //初始化串口1 波特率115200 8位数据,切换管脚需进入该函数内部，默认第一管脚
    GPIO_LED_Init();
    GPIO_KEY_Init();
    
    Global_IRQ_Enable();            //使能全局中断    Global_IRQ_Disable();

    OLED_Init();                    //OLED初始化
    OLED_CLS();
	OLED_Show_LQLogo();             //显示LOGO
//    for(i=0;i<2;i++)
//    {
//        LED_Ctrl(Beep0,RVS); 
//        delay_ms(100);
//    }
    UART1_PutStr("USER Init OK \r\n");      // UART1 打印 printf("USER Init OK \n");
//    
//==================================LQ 应用例程测试函数,均内含死循环 ==================================
 /*  有关NVIC中断的设置，涉及的独立例程中均为 Priority_0 最低优先级，实际使用请切合需要设置*/
//    Test_GPIO_LED();            //PASS , 初始化GPIO模式，LED状态控制，测试LED灯闪烁,展示GPIO初始化方法
//    Test_Sample_WDT();          //PASS , 测试看门口复位，程序超时会自动触发复位，可手动加大循环内的延时观察情况
//    Test_GPIO_KEY();            //PASS , 测试按键，KEY1,KEY2,KEY3,任意按键按下时LED0点亮，松开熄灭 
//    Test_OLED();                //PASS , 测试OLED显示时插上OLED屏幕，软件SPI，在LQ_OLED096.c 开头修改对应管脚初始化管脚
//    Test_UART();                //PASS , 测试串口初始化发送字符，printf选择 UART1~UART4（在STC32G_UART.h 中宏定义）,注意多个串口如果使用同一个定时器则波特率需要保持一致
//    Test_Timer();               //PASS , 测试定时器10ms中断*20次闪灯，Timer0至Timer11的1us到5s全自动配置
//    Test_ADC();                 //PASS , 测试ADC信号读取，电压范围0~5V对应ADC读取的模拟量0~4095
//    Test_IPSLCD();              //PASS , 已移除.8051U不建议使用LCD屏,占空间大,STC32G可以考虑，或者屏幕使用硬件SPI口，如果要使用，请打include.h关闭头文件注释并将对应的.C文件添加到工程
//    Test_Encoder();             //PASS , 测试T3/T4脉冲计数用作编码器读取OLED显示
//    Test_Motor();               //PASS , 测试LQ_DRV8701或7971驱动电机驱动,按键K0,K2控制速度加减，负占空比值电机的反转
//    Test_Servo();               //PASS , 测试舵机驱动，标定舵机中值，可以用来确定舵机左右打角 限幅值
//    Test_BLSmotor();            //PASS , 测试无感无刷电机
//    Test_ExINT();               //PASS , 测试传统外部中断，INT脚位于P3口：INTO:P3.2,INT1:P3.3,INT2:P3.6,INT3:P3.7,INT4:P3.0
//    Test_GPIO_PxINT();          //PASS , 测试普通IO中断功能，所有I/0均可设置，下降沿、上升沿、高电平、低电平四种模式
//    Test_IIC_VL53();            //PASS , 测试软I2C的VL53激光测距模块,OLED显示实时测距结果
//    Test_I2C_LQLSM6DSR();       //PASS , 测试软I2C通信读取LSM6DSR 六轴陀螺仪数据，OLED显示,注意I2C模式下本测试需拉低SDO为0x6B，SDO拉高地址为0x6A
//    Test_SPI_LQLSM6DSR();       //PASS , 测试LQLSM6DSR六轴陀螺仪或者LQ9AGM陀螺仪，OLED显示数据,
//    Test_UART_DMA();            //PASS , 测试UART1固定长度数据收发
//    Test_HotRC();               //PASS , 测试SBUS遥控器，数据解析，使用串口3进行测试
//    Test_BD_GPS();              //PASS , 测试GPS BD1202模块，数据读取，时间，坐标OLED显示
//    Test_UTM();                 //PASS , 测试GPS BD坐标数据，转换UTM
//    Test_EEPROM();              //PASS , 测试AI8051U的 EEPROM读写
//    Test_CCD();                 //PASS , 测试CCD获取然后在OLED上显示出来，注意阈值可能得重新设定

/*  若有其他外设功能增加，会陆续更新  */
//======================================================================================================
/*************若上方有测试函数未注释，则程序不会运行到这里，测试函数内部有while(1)*******************/

   
	while(1)
	{
        // LED_Ctrl(LED0,RVS);        //P01
        GPIO_WritePin(P0_1, !GPIO_ReadPin(P0_1));   //准双向口GPIO便捷操作，实际建议直接: P01=！P01 方式 简洁高效
        delay_ms(100);            //50ms
//        LED_Ctrl(LED0,ON);
//        delay_us(50000);        //50ms
//        LED_Ctrl(LED0,OFF);
	}
}






// 初始化pll时钟,注意使用时需要添加对应的库，详情参考数据手册
void PLL_Init(void)
{
    
	USBCLK &= ~0xf0;
	USBCLK |= 0xd0;     // 允许追频
	CLKSEL |= 0xc0;     // 时钟选择设置，并将PLL时钟作为外设高速时钟
	TFPU_CLKDIV = 0;
	DMAIR = 0x3f;		// 将PLL时钟作为TFPU时钟，0x3e 选择系统时钟为TFPU时钟 42*3=126Mhz
}

/***************************************************************************
* 函     数: void main(void)
* 描     述: 函数入口
* 参     数: None.
* 返     回: None.
****************************************************************************/
void System_Init()
{
	WTST = 0;		//设置程序指令延时参数，赋值为0可将CPU执行指令的速度设置为最快
	EAXSFR();		//扩展SFR(XFR)外设寄存器访问使能
	CKCON = 0;      //提高访问XRAM速度
    
    PLL_Init();
    P0_MODE_IN_HIZ(GPIO_Pin_All);       // 上电先将所有IO配置为高阻态，可以防止意外导致管脚烧坏的情况发生
    P1_MODE_IN_HIZ(GPIO_Pin_All);
    P2_MODE_IN_HIZ(GPIO_Pin_All);
    P3_MODE_IN_HIZ(GPIO_Pin_All);
    P4_MODE_IN_HIZ(GPIO_Pin_All);
    P5_MODE_IN_HIZ(GPIO_Pin_All);
    
    PNRST=1;   //P47，即RSTCFG|=1<<4;复位寄存器配置（STC32G/F为P54），防止ISP下载误设为I/O下次需要上电复位才能下载使得复位按键失效

}


