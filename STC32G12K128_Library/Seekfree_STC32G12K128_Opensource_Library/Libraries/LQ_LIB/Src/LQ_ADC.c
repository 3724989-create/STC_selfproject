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
#include "LQ_ADC.h"

/***********************************************************************************
 * 函 数 名：void ADC_Init(void)
 * 功    能：ADC初始化，管脚初始化和中断模式、使能等设置
 * 参    数：无
 * 返 回 值：无
 * 说    明：  ADC通到    对应GOIO
 *               ADC0	    P1.0
 *               ADC1	    P1.1
 *               ADC2	    P1.2
 *               ADC3	    P1.3
 *               ADC4	    P1.4
 *               ADC5	    P1.5
 *               ADC6	    P1.6
 *               ADC7	    P1.7
 *      
 *               ADC8	    P0.0
 *               ADC9	    P0.1
 *               ADC10	    P0.2
 *               ADC11	    P0.3
 *               ADC12	    P0.4
 *               ADC13	    P0.5
 *               ADC14	    P0.6
 **********************************************************************************/
void ADC_Init(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//结构定义

    //ADC 端口IO配置，这里将支持ADC的端口全部配置了，实际请仅初始化需要的管脚
//    P0_MODE_IN_HIZ(GPIO_Pin_LOW | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);	//P0.0~P0.6 设置为高阻输入
//	P1_MODE_IN_HIZ(GPIO_Pin_All);		    //P1.0~P1.7 设置为高阻输入
    P0_MODE_IN_HIZ(GPIO_Pin_0);
    P1_MODE_IN_HIZ(GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7);    //或 GPIO_InitConfig(P1_1|P1_3|P1_5|P1_7,OUT_IN);

    //ADC配置
	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

}



/********************************************************************************************************
**函数信息 ：Get_Adc_Average(uint8_t ADC_Channel_x,uint8_t times)
**功能描述 ：获取几次ADC1采样值的平均值
**输入参数 ：ADC_Channel_x , x为0~8
**输出参数 ：puiADData为ADC读到的值
********************************************************************************************************/
u16 ADC_Read_Average(uint8 ADC_Channel_x,uint8 times)
{
    u32 temp_val=0;
    u8 t;
    u8 delay;
    for(t=0;t<times;t++)
    {
        temp_val+=Get_ADCResult(ADC_Channel_x);
        for(delay=0;delay<100;delay++);
    }
    return temp_val/times;
}





void Test_ADC()
{
    char txt[20];
    ADC_Init();         //ADC初始化，进入函数内部选择对应需要的管脚通道
    Global_IRQ_Enable();

    OLED_Init();                    //OLED初始化
    OLED_CLS();       //加入OLED显示
    OLED_P8x16Str(10, 0, "Test LQ ADC ");
    
	while (1)
	{
        sprintf(txt, "ADV:%05d ", Get_ADCResult(8));  //ADC8-P00 做电源电压 11.0V->1700左右，12.6V->1970左右
        OLED_P6x8Str(10,2, txt); 	// 显示
        sprintf(txt, "BAT:%.1fV ", Get_ADCResult(8)*0.0053f);        //核心板母板
        OLED_P6x8Str(70,2, txt); 	// 显示
        
        sprintf(txt, "AD1:%05d ", Get_ADCResult(1));  //ADC1-P11
        OLED_P6x8Str(10,3, txt); 	// 显示
        sprintf(txt, "AD3:%05d ", Get_ADCResult(3));  //ADC3-P13
        OLED_P6x8Str(10,4, txt); 	// 显示
        sprintf(txt, "AD5:%05d ", Get_ADCResult(5));  //ADC5-P15
        OLED_P6x8Str(10,5, txt); 	// 显示
        sprintf(txt, "AD7:%05d ", Get_ADCResult(7));  //ADC7-P17
        OLED_P6x8Str(10,6, txt); 	// 显示
        delay_ms(100);
	}

}


