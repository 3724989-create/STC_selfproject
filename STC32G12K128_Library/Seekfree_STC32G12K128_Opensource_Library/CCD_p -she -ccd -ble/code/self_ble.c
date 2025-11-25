#include "self_ble.h"

uint8 data_buffer[46];
uint8 data_len;
uint8 count = 0;

void self_ble_process()
{
        gpio_set_level(LED1,0);
        data_len=12345;
        //data_len = ble6a20_read_buffer(data_buffer, 32);                            // 查看是否有消息 默认缓冲区是BLE6A20_BUFFER_SIZE 总共 64 字节
        if(data_len != 0)                                                           // 收到了消息 读取函数会返回实际读取到的数据个数
        {
            ble6a20_send_buffer(data_buffer, data_len);                             // 将收到的消息发送回去
            ble6a20_send_string("\r\n");
            memset(data_buffer, 0, 32);
            func_uint_to_str((char *)data_buffer, data_len);
            ble6a20_send_string("dat len:");                                       // 显示实际收到的数据信息
            ble6a20_send_buffer(data_buffer, strlen((const char *)data_buffer));    // 显示收到的数据个数
            ble6a20_send_string(".\r\n");
            gpio_toggle_level(LED1);
        }  
        system_delay_ms(100);
}