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

void main()
{
    clock_init(SYSTEM_CLOCK_30M);
	debug_init();

    // 此处编写用户代码 例如外设初始化代码等
	tft180_init();
	
    while(1)
    {
		
		tft180_clear(RGB565_WHITE);										//清屏
        tft180_show_string(0, 16*0, "seekfree"); 			//显示字符串
        tft180_show_uint16(0, 16*1, 111);                				//显示一个16位无符号整数
        tft180_show_int32 (0, 16*2, 222,3);               				//显示一个32位有符号数  并去除无效0
        tft180_show_int32 (0, 16*3, -333,3);              				//显示一个32位有符号数  并去除无效0
        tft180_show_float (0, 16*4, 56.35,3,1);           				//显示一个浮点数        并去除整数部分无效0
        //以上函数x坐标设置是以像素为单位   y坐标是以字符为单位

		//以下为色彩测试，红绿蓝全屏各显示一秒。
		system_delay_ms(2000);									//延时1秒
		tft180_clear(RGB565_RED);
		system_delay_ms(1000);									//延时1秒
		tft180_clear(RGB565_GREEN);
		system_delay_ms(1000);									//延时1秒
		tft180_clear(RGB565_BLUE);
		system_delay_ms(1000);									//延时1秒
        // 此处编写需要循环执行的代码
    }
}
