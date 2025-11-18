/*********************************************************************************************************************
* STC32G Opensourec Library 即（STC32G 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是STC 开源库的一部分
*
* STC32G 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK FOR C251
* 适用平台          STC32G
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2024-08-01        大W            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

#pragma warning disable = 177

// *************************** 例程硬件连接说明 ***************************
// 接入红孩儿 CCD 模块
//      模块管脚            单片机管脚
//      CLK                 查看 SEEKFREE_TSL1401.h 中 CCD_CLK_PIN 宏定义
//      SI                  查看 SEEKFREE_TSL1401.h 中 CCD_SI_PIN 宏定义
//      AO[x]               查看 SEEKFREE_TSL1401.h 中 AD_CHANNEL_CH 宏定义
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源

// *************************** 例程使用步骤说明 ***************************
// 1.根据硬件连接说明连接好模块，使用电源供电(下载器供电会导致模块电压不足)
//
// 2.下载例程到单片机中，打开逐飞串口助手。
//
// 3.在逐飞串口助手中，选择图像传输
//
// 4.选择下载器对应的串口号，波特率(默认115200)，点击连接
//
// 5.可以在逐飞助手的图像传输界面看到对应波形

// *************************** 例程测试说明 ***************************
// 1.本例程会通过 Debug 串口输出测试信息 请务必接好调试串口以便获取测试信息
//
// 2.连接好模块和核心板后（尽量使用配套主板测试以避免供电不足的问题） 烧录本例程 按下复位后程序开始运行
//


#define PIT_CH                          (TIM1_PIT )                             // 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
//#define PIT_PRIORITY                    (TIM1_IRQn)                           TIM1的中断优先级默认最低，不可修改，具体看手册。



uint16 i, j;

uint8 y1_boundary[128];
uint8 y2_boundary[128];
uint8 y3_boundary[128];


////-------------------------------------------------------------------------------------------------------------------
//// 函数简介     滴答客发送函数
//// 参数说明     *buff           需要发送的数据地址
//// 参数说明     length          需要发送的长度
//// 返回参数     uint32          剩余未发送数据长度
//// 使用示例
////-------------------------------------------------------------------------------------------------------------------
//uint32 seekfree_assistant_transfer   (const uint8 *buff, uint32 length)
//{
//	uart_write_buffer(DEBUG_UART_INDEX, buff, (uint16)length);
//	return 0;
//}

void pit_handler (void);

void main()
{
    uint16 threshold=0;
    
    clock_init(SYSTEM_CLOCK_30M);
	debug_init();
	// 初始化CCD
    tsl1401_init();
	
	// 设置定时器1中断回调函数
	tim1_irq_handler = pit_handler;
	
    pit_ms_init(PIT_CH, 10);        //1的太快了                                             // 初始化 PIT 为周期中断 100ms 周期
	
    
	seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);          //初始化UART串口
	
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, 128, 256);
    // 设置一个边线用于显示CCD波形
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, 128, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);
    // 边线3用于显示中线
    memset(y3_boundary, 0x80, sizeof(y3_boundary));   //黄色的那根是中线
    
    
	
    while(1)
	{
		if(tsl1401_finish_flag)
        {
            tsl1401_finish_flag = 0;
            
		//tsl1401_binary_data(DEBUG_UART_INDEX,0,10);
        
            //tsl1401_send_data(DEBUG_UART_INDEX, 1);
			
             for(j = 0; j < 128; j++)
             {
                 //threshold=calculate_dynamic_threshold(0);
                 //tsl1401_binary_data_process(&tsl1401_data[0],j,threshold);
                 // 获取CCD数据，并按分辨率进行压缩
                 switch(TSL1401_AD_RESOLUTION)   //初始ADC_8BIT
                 {
                     case ADC_8BIT:
                     {
                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 2);
                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 2);
                         break;
                     }
                     case ADC_10BIT:
                     {
                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 8);
                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 8);
                         break;
                     }
                     case ADC_12BIT:
                    {
                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 32);
                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 32);
                         
                         break;
                     }
                 }
                
                
             }

             // 发送图像
            seekfree_assistant_camera_send();
	
        }	
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     PIT 的周期中断处理函数 这个函数将在 PIT 对应的定时器中断调用 详见 isr.c
// 参数说明     void
// 返回参数     void
// 使用示例     pit_handler();
//-------------------------------------------------------------------------------------------------------------------
void pit_handler (void)
{	
	tsl1401_collect_pit_handler();
}
