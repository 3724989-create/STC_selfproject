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

// *************************** 例程硬件连接说明 ***************************
// 接入GPS模块
//      模块管脚            单片机管脚
//      RX                  查看 SEEKFREE_GPS_TAU1201.h 中 GPS_TAU1201_RX  宏定义
//      TX                  查看 SEEKFREE_GPS_TAU1201.h 中 GPS_TAU1201_TX  宏定义 
//      GND                 核心板电源地 GND
//      3V3                 核心板 3V3 电源

// *************************** 例程测试说明 ***************************
// 1.核心板烧录完成本例程
// 2.如果使用主板测试 在断电情况下 将核心板插入主板 GPS模块插入主板无线模块接口左侧
// 3.如果使用核心板和模块测试 在断电情况下 将核心板与无线模块连接
// 5.编译并下载程序可以看到串口调试助手中打印当前从GPS模块获取到的信息
// 6.如果GPS初始化成功之后，将会有值，如果值全为0，请稍微等待几分钟或者拿到室外开阔场地再试
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查

//！！运行效果：
//GPS正常获取卫星信号后，模块上方会有一个蓝色LED灯间歇闪烁，如果没有闪烁则说明没有获取到信息
//屏幕将会显示当前获取到的日期时间、经纬度、高度、速度、方向等参数
	
void main()
{
    clock_init(SYSTEM_CLOCK_30M);
	debug_init();

    // 此处编写用户代码 例如外设初始化代码等
	gps_init();				// GPS初始化
	ips114_init();			// IPS114初始化

    // 此处编写用户代码 例如外设初始化代码等

    while(1)
    {
        // 此处编写需要循环执行的代码
        if(gps_tau1201_flag)
        {
            gps_tau1201_flag = 0;
			
            if(!gps_data_parse())          //开始解析数据
			{
				//显示日期及时间
                ips114_show_uint16(  0, 16*0, gps_tau1201.time.year);
                ips114_show_uint8(  80, 16*0, gps_tau1201.time.month);
                ips114_show_uint8( 160, 16*0, gps_tau1201.time.day);
                ips114_show_uint8(   0, 16*1, gps_tau1201.time.hour);
                ips114_show_uint8(  80, 16*1, gps_tau1201.time.minute);
                ips114_show_uint8( 160, 16*1, gps_tau1201.time.second);
				//显示GPS位置信息

                ips114_show_uint8(  0, 16*2, gps_tau1201.state);
                ips114_show_float(120, 16*2, gps_tau1201.latitude, 4, 6);
                ips114_show_float(  0, 16*3, gps_tau1201.longitude, 4, 6);
                ips114_show_float(120, 16*3, gps_tau1201.speed, 4, 6);
                ips114_show_float(  0, 16*4, gps_tau1201.direction, 4, 6);
                ips114_show_uint8(120, 16*4, gps_tau1201.satellite_used);
                ips114_show_float(  0, 16*5, gps_tau1201.height, 4, 6);
				
				printf("now time:\r\n");                                        // 输出年月日时分秒
				printf("year-%d, month-%d, day-%d\r\n", gps_tau1201.time.year, gps_tau1201.time.month, gps_tau1201.time.day);           // 输出年月日时分秒
				printf("hour-%d, minute-%d, second-%d\r\n", gps_tau1201.time.hour, gps_tau1201.time.minute, gps_tau1201.time.second);   // 输出年月日时分秒
				printf("gps_state       = %d\r\n" , gps_tau1201.state);         //输出当前定位有效模式 1：定位有效  0：定位无效
				printf("latitude        = %lf\r\n", gps_tau1201.latitude);      //输出纬度信息
				printf("longitude       = %lf\r\n", gps_tau1201.longitude);     //输出经度信息
				printf("speed           = %lf\r\n", gps_tau1201.speed);         //输出速度信息
				printf("direction       = %lf\r\n", gps_tau1201.direction);     //输出方向信息
				printf("satellite_used  = %d\r\n" , gps_tau1201.satellite_used);//输出当前用于定位的卫星数量
				printf("height          = %lf\r\n", gps_tau1201.height);        //输出当前GPS天线所处高度
			}
			system_delay_ms(1000);//这里延时主要目的是为了降低输出速度，便于在串口助手中观察数据，实际使用的时候不需要这样去延时
			P52 = !P52;
        }
        // 此处编写需要循环执行的代码
    }
}
