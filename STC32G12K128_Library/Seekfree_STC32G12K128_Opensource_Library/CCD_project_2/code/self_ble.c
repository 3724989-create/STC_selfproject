#include "self_ble.h"
//CCD数据定义
#define CCD_PIXEL_COUNT 128
#define CCD_DATA_SIZE 128

//协议帧定义
#define FRAME_HEADER_LEN 2      //帧头长度
#define DATA_LEN_FIELD_LEN 2    //长度域长度（低位在前，存储128）
#define CHECKSUM_LEN 1         //校验和长度
#define FRAME_END_LEN 2        //帧尾长度
#define FRAME_OVERHEAD (FRAME_HEADER_LEN + DATA_LEN_FIELD_LEN + CHECKSUM_LEN + FRAME_END_LEN) // 7 字节
#define MAX_BLE_TX_BUFFER_SIZE (CCD_DATA_SIZE + FRAME_OVERHEAD) // 128 + 7 = 135 字节

// 缓冲区定义
uint8 tx_buffer[MAX_BLE_TX_BUFFER_SIZE];
uint8 data_buffer[46];
uint8 data_len;
uint8 count = 0;

void self_ble_process()
{
        gpio_set_level(LED1,0);
        //data_len="12345";
        data_len = ble6a20_read_buffer(data_buffer, 32);                            // 查看是否有消息 默认缓冲区是BLE6A20_BUFFER_SIZE 总共 64 字节
        if(data_len != 0)                                                           // 收到了消息 读取函数会返回实际读取到的数据个数
        {
            ble6a20_send_buffer(data_buffer, data_len);                             // 将收到的消息发送回去
            ble6a20_send_string("\r\n");
            memset(data_buffer, 0, 32);
            func_uint_to_str((char *)data_buffer, data_len);
            ble6a20_send_string("data len:");                                       // 显示实际收到的数据信息
            ble6a20_send_buffer(data_buffer, strlen((const char *)data_buffer));    // 显示收到的数据个数
            ble6a20_send_string(".\r\n");
            gpio_toggle_level(LED1);
        }
        //ble6a20_send_string("d"); 
        system_delay_ms(100);
}

void self_ble_CCD_process()
{
        gpio_set_level(LED1,0);
        //data_len="12345";

        ble6a20_send_string("d"); 
        system_delay_ms(100);
}

/**
 * @brief 计算数据体的校验和（简单累加和的低8位）
 * * @param data 要计算校验和的数据指针
 * @param len  数据长度
 * @return uint8_t 校验和
 */
// 修正：返回值类型改为 uint8，参数列表使用 uint8* 和 unsigned int 
uint8 calculate_checksum(uint8 *dat , uint8 len)
{
    unsigned int sum = 0; // 累加器仍然使用 16位 (unsigned int)
    uint8 i; // 循环变量使用 uint8
    uint8 result; // 用于存储最终校验和
   
    for(i = 0; i < len; i++)
    {
        // 使用数组索引访问数据
        sum += *(dat + i);
    }
    
    // 赋值给局部变量
    result = (uint8)sum;
    return result; 
}
/**
 * @brief 封装并发送一帧 CCD 像素数据
 */
void ble_send_ccd_frame(void)
{
        size_t frame_index=0;
        uint16 data_body_len=CCD_DATA_SIZE;   //数据体长度固定为128字节
        uint8 *data_body_ptr;
        uint8 checksum;
        size_t total_frame_len;
    
        //帧头
        tx_buffer[frame_index++]=0xAA;
        tx_buffer[frame_index++]=0x55;
        //数据长度域
        tx_buffer[frame_index++] = (uint8)(data_body_len & 0xFF);         // 低字节 (128)
        tx_buffer[frame_index++] = (uint8)((data_body_len >> 8) & 0xFF);   // 高字节 (0)
        //数据体
        data_body_ptr=&tx_buffer[frame_index]; //标记CCD数据开始的地址
        //将CCD数据拷贝到缓冲区
        memcpy(data_body_ptr,y1_boundary,data_body_len);
        frame_index+=data_body_len;
        //校验和
        checksum=calculate_checksum(data_body_ptr,(uint8)data_body_len);
        tx_buffer[frame_index++] = checksum;
        // --- 5. 帧尾 (0x0D 0x0A) ---
        tx_buffer[frame_index++] = 0x0D;
        tx_buffer[frame_index++] = 0x0A;
        total_frame_len = frame_index;
        ble6a20_send_buffer(tx_buffer,total_frame_len);
}