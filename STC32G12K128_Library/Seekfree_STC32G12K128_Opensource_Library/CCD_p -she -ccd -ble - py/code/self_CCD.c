#include "self_CCD.h"

uint8 y1_boundary[128];
uint8 y2_boundary[128];
uint8 y3_boundary[128];
uint16 Tsl_the=0;

bit CCD_flag=0;

void ccd_handler (void);

void CCD_init(void)
{
    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);  		// 初始化 LED2 输出 默认低电平 推挽输出模式
    tsl1401_init();
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_BLE6A20);          //初始化蓝牙串口
	
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, 128, 256);
    // 设置一个边线用于显示CCD波形
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, 128, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);
    // 边线3用于显示中线
    memset(y3_boundary, 0x80, sizeof(y3_boundary));   //黄色的那根是中线

    pit_ms_init(CCD_CH, 10);
    interrupt_set_priority(CCD_PRIORITY, 0);
    tim1_irq_handler = ccd_handler;
}

uint16 calculate_threshold(const uint8 *dat, uint8 count)
{
    uint32 sum=0;
    uint8 i;
    uint16 result;
    for(i=0;i<count;i++)
    {
       sum += *(dat + i);
    }
    result=(uint16)(sum / count);
    return result;
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

void CCD_process(void)
{
    uint8 i,j;
    uint16 threshold;
    uint8 temp_data[TSl140_LEN];
    uint8 pre_dat;
    uint8 now_dat;
    uint8 pass_dat;
            
		//tsl1401_binary_data(DEBUG_UART_INDEX,0,10);
        
            //tsl1401_send_data(DEBUG_UART_INDEX, 1);
			threshold=calculate_threshold(tsl1401_data[0],TSl140_LEN)+TSl140Dyn;
             for(j = 0; j < 128; j++)
             {
                 if (tsl1401_data[0][j] >=threshold)
                {
                    // 假设高 ADC 值代表亮区（背景）
                    y1_boundary[j] = 1; 
                }

                else
                {
                    y1_boundary[j] = 0;
                }
            }
                   
            for(i=1;i<TSl140_LEN-1;i++) //去掉头尾避免越界
            {
                pre_dat= y1_boundary[i-1];
                now_dat=y1_boundary[i];
                pass_dat=y1_boundary[i+1];
                //消除独立的点
                if(pre_dat==1&&now_dat==0&&pass_dat==1)
                {
                    y1_boundary[i] = 1;     
                }
                if(pre_dat==0&&now_dat==1&&pass_dat==0)
                {
                    y1_boundary[i] = 0;     
                }
            }


            // 发送图像
              //seekfree_assistant_camera_send();
              ble_send_ccd_frame();
            //self_ble_CCD_process();
}


void ccd_handler (void)
{
    tsl1401_finish_flag = 1;
}

void Menu_CCD_process()
{
    CCD_flag=!CCD_flag;
}

void CCD_main_process()
{
    if(CCD_flag)
    {
      if (tsl1401_finish_flag)
      {
          tsl1401_finish_flag = 0;
          tsl1401_collect_pit_handler();
          CCD_process();
      }
    }
}