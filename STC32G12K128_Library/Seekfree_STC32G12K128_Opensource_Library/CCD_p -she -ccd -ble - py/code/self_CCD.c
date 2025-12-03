#include "self_CCD.h"

//高斯核参数
#define Guess_kernel_size 5
#define Guess_Kernel_weight 20

// --- 配置参数 ---
#define CCD_PIXEL_COUNT 128     // CCD 像素点数量
#define NUM_FRAMES_TO_AVERAGE 5 // 帧平均的帧数 N
#define TSL1401_MAX_VALUE 255   // 假设 ADC 数据的最大值 (如果是 10bit 或 12bit，请修改)

uint8 y1_boundary[128];
uint8 y2_boundary[128];
uint8 y3_boundary[128];
uint16 Tsl_the=0;
bit CCD_flag=0;

// --- 数据缓冲区 ---
// 历史数据存储区：存储最近 N 帧的原始像素值
// 帧数 * 像素数
// 使用 uint16_t 是为了防止像素值在求和时溢出 (N * MaxValue)
// 假设 MaxValue=255, N=5. 255 * 5 = 1275, 仍在 uint16_t (65535) 范围内。
static uint8 frame_history_buffer[NUM_FRAMES_TO_AVERAGE][CCD_PIXEL_COUNT];
// 最终平均后的像素值（送给高斯平滑或直接发送）
static uint8 averaged_pixels[CCD_PIXEL_COUNT];

// 用于跟踪当前存储的是哪一帧，作为环形缓冲区的索引
static size_t current_frame_index = 0;

// 用于跟踪目前缓冲区中存储了多少帧数据
static size_t frames_count = 0;

static const uint8 Guess_kernel[Guess_kernel_size]={3,4,6,4,3};

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

int apply_gussian_smoothing(const uint16 *data_in,uint16*data_out,int length)
{
    int i,j;
    long sum;
    int kernel_offset=Guess_kernel_size/2;
    int data_index;

    //参数检查
    if(data_in==NULL||data_out==NULL||length<Guess_kernel_size)
    {
        printf("something oversize.");
        return 1;
    }

    for(i=0;i<length;i++)
    {
        sum=0;
        for(j=0;j<Guess_kernel_size;j++)
        {
           // 计算当前核权重对应的输入数据索引
           // j=0 -> i-2, j=1 -> i-1, j=2 -> i, j=3 -> i+1, j=4 -> i+2
           data_index= i+(j-kernel_offset);
           if(data_index<=0)
           {
                //左边界使用第一个值
                data_index=0;
           }
           else if(data_index>=length)
           {
                data_index=length-1;
           }
           sum+=(long)data_in[data_index]*Guess_kernel[j];
        }
        //在被除数上增加权重的一半实现四舍五入
        data_out[i]=(int)(sum+Guess_Kernel_weight/2)/Guess_Kernel_weight;
//       if(data_out[i]==0&&i!=0&&data_out[i-1]!=0)
//       {
//         data_out[i]=data_out[i-1];  
//       }
    }
    return 0; // 成功返回 0
}

void apply_frame_averaging(const uint8 *new_frame_data)
{
    uint32 pixel_sum;
    uint8 i,j,z;

    //将新帧存入缓存区
    for( i=0;i<CCD_PIXEL_COUNT;i++)
    {
        frame_history_buffer[current_frame_index][i]=new_frame_data[i];
    }
    //更新帧计数
    if(frames_count<NUM_FRAMES_TO_AVERAGE)
    {
        frames_count++;
    }
    
    current_frame_index=(frames_count+1)%NUM_FRAMES_TO_AVERAGE;
    //计算平均值
    if(frames_count==NUM_FRAMES_TO_AVERAGE)
    {
        //遍历像素点
        for(j=0;j<CCD_PIXEL_COUNT;j++)
        {
            pixel_sum=0;
            for(z=0;z<NUM_FRAMES_TO_AVERAGE;z++)
            {
                pixel_sum+=frame_history_buffer[z][j];
            }
            averaged_pixels[i]=(uint8)(pixel_sum/NUM_FRAMES_TO_AVERAGE);
        }

    }
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

void ccd_avarage_data(void)
{
    // 检查是否有数据采集完成 (此检查冗余，因为外部已检查 tsl1401_finish_flag)
    // if (!tsl1401_finish_flag) return; 
    
    // TSL1401_AD_RESOLUTION 被定义为 ADC_12BIT (0-4095)
    // 压缩系数为 4096 / 128 = 32
    u8 j;
    const u16 COMPRESSION_DIVER=32;
    const u8 MAX_NOMPRESSED_VALUE=128;

    apply_frame_averaging(tsl1401_data[0]);
    for( j=0;j<128;j++)
    {
         // 压缩逻辑: 128 - (ADC_VALUE / 32)
         // 实现：高亮度 (高 ADC 值) 对应低压缩值 (接近 0)
         // ADC 值 4095 -> 128 - 128 = 0
         // ADC 值 0 -> 128 - 0 = 128
         y1_boundary[j]=averaged_pixels[j];
    }
     // 此时 y1_boundary 已经准备好供 LCD 绘制
     ble_send_ccd_frame();
}

void CCD_binary_process(void)
{
    uint8 i,j;
    uint16 threshold;
    uint8 temp_data[TSl140_LEN];
    uint8 pre_dat;
    uint8 now_dat;
    uint8 pass_dat;
            
		//tsl1401_binary_data(DEBUG_UART_INDEX,0,10);
        
            //tsl1401_send_data(DEBUG_UART_INDEX, 1);
			//threshold=calculate_threshold(tsl1401_data[0],TSl140_LEN)+TSl140Dyn;
            threshold=calculate_threshold(tsl1401_data[0],TSl140_LEN);
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
              ble_send_ccd_frame();
}

void CCD_G_process(void)
{
    uint8 i,j;
            
//   for(j = 0; j < 128; j++)
//   {
//     y1_boundary[j] =tsl1401_data[0][j];
//   }
//    
  //apply_frame_averaging(tsl1401_data[0]); //计算量太大丢数据
  apply_gussian_smoothing(averaged_pixels,y1_boundary,TSl140_LEN);

  // 发送图像
  ble_send_ccd_frame();
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
//    if(CCD_flag)
//    {
      if (tsl1401_finish_flag)
      {
          tsl1401_finish_flag = 0;
          tsl1401_collect_pit_handler();
          CCD_G_process();
      }
//    }
}