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
//实际应用情况中，可能有的外设初始化函数、宏定义不会被用到，为节省8051U空间资源提升效率，可以合理选择将不需要的注释掉
//【Target 】 STC32G/AI8051U_32Bit 32位模式，切换芯片 在编译器宏切换


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
    System_Init();
    Global_IRQ_Enable();
    
/*
    测试CCD获取然后在OLED上显示出来，注意阈值可能得重新设定
    管脚选择: 
        ADO --->P17
        SCL --->P15
        SDA --->P13
    
*/
    Test_CCD();          //PASS, 测试函数内含while(1)死循环
    
	while(1)
	{
        ;
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


