#include "self_ble.h"

uint8 data_buffer[46];
uint8 data_len;
uint8 count = 0;

void self_ble_process()
{
        while(ble6a20_init())                                                       // 判断是否通过初始化
    {
        gpio_toggle_level(LED1);                                                // 翻转 LED 引脚输出电平 控制 LED 亮灭
        system_delay_ms(100);                                                   // 短延时快速闪灯表示异常

        printf("ble6a20 init error.\r\n");
        printf("restart ble6a20 init.\r\n");
    }
    ble6a20_send_byte('\r');
    ble6a20_send_byte('\n');
    ble6a20_send_string("SEEKFREE BLE6A20 DEMO.\r\n");                    // 初始化正常 输出测试信息
     // 此处编写需要循环执行的代码
        gpio_set_level(LED1,0);
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
        system_delay_ms(100);
        // 此处编写需要循环执行的代码
}