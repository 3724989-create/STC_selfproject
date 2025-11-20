#include "self_CCD.h"

uint8 y1_boundary[128];
uint8 y2_boundary[128];
uint8 y3_boundary[128];

void CCD_init(void)
{
    tsl1401_init();

    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);          //初始化UART串口
	
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, 128, 256);
    // 设置一个边线用于显示CCD波形
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, 128, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);
    // 边线3用于显示中线
    memset(y3_boundary, 0x80, sizeof(y3_boundary));   //黄色的那根是中线
}

void ccd_process_data(void)
{
    // 检查是否有数据采集完成 (此检查冗余，因为外部已检查 tsl1401_finish_flag)
    // if (!tsl1401_finish_flag) return; 
    
    // TSL1401_AD_RESOLUTION 被定义为 ADC_12BIT (0-4095)
    // 压缩系数为 4096 / 128 = 32
    u8 j;
    const u16 COMPRESSION_DIVER=32;
    const u8 MAX_NOMPRESSED_VALUE=128;

    for( j=0;j<128;j++)
    {
         // 压缩逻辑: 128 - (ADC_VALUE / 32)
         // 实现：高亮度 (高 ADC 值) 对应低压缩值 (接近 0)
         // ADC 值 4095 -> 128 - 128 = 0
         // ADC 值 0 -> 128 - 0 = 128
         y1_boundary[j]=(u8)(MAX_NOMPRESSED_VALUE-tsl1401_data[0][j] / COMPRESSION_DIVER);
    }
     // 此时 y1_boundary 已经准备好供 LCD 绘制
}

//void CCD_process(void)
//{
//    uint8 i,j;
//    		if(tsl1401_finish_flag)
//        {
//            tsl1401_finish_flag = 0;
//            
//		//tsl1401_binary_data(DEBUG_UART_INDEX,0,10);
//        
//            //tsl1401_send_data(DEBUG_UART_INDEX, 1);
//			
//             for(j = 0; j < 128; j++)
//             {
//                 //threshold=calculate_dynamic_threshold(0);
//                 //tsl1401_binary_data_process(&tsl1401_data[0],j,threshold);
//                 // 获取CCD数据，并按分辨率进行压缩
//                 switch(TSL1401_AD_RESOLUTION)   //初始ADC_8BIT
//                 {
//                     case ADC_8BIT:
//                     {
//                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 2);
//                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 2);
//                         break;
//                     }
//                     case ADC_10BIT:
//                     {
//                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 8);
//                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 8);
//                         break;
//                     }
//                     case ADC_12BIT:
//                    {
//                         y1_boundary[j] = (uint8)(128 - tsl1401_data[0][j] / 32);
//                         y2_boundary[j] = (uint8)(256 - tsl1401_data[1][j] / 32);
//                         
//                         break;
//                     }
//                 }
//                
//                //printf("%d\r\n",y1_boundary[j]);
//             }
//            // 发送图像
//        }
//    }