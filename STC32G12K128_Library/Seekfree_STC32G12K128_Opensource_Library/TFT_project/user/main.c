
#include "bsp.h"



void main()
{
    char tmp[4] = {0};
    int Test_cnt = 0;
    uint32 last_write_time = 0;
    uint32 current_time = 0;
    uint32 last_complete_test_time = 0;
    unsigned char i = 0;
    
    all_init();
    
    uart_write_string(UART_INDEX, "========================================\r\n");
    uart_write_string(UART_INDEX, "  I2C Full Duplex Communication Ready!\r\n");
    uart_write_string(UART_INDEX, "========================================\r\n\r\n");
    
    Diagnose_SCL_Problem();
    
        // 准备从机测试数据
    for(i = 0; i < 8; i++) {
        i2c_slave_receive_buffer[i] = 0xC0 + i;
    }
    slave_receive_count = 8;
    printf("[Slave] Test data prepared: ");
    for(i = 0; i < 8; i++) {
        uart_send_hex_8bit(i2c_slave_receive_buffer[i]);
        uart_write_byte(UART_INDEX, ' ');
    }
    printf("\r\n");

    
    while(1)
    {
        current_time = pit_read();
        
        // 从机处理
        I2C_Slave_Process();
        // 每5秒执行一次完整测试
        if(current_time - last_complete_test_time > 5000) {
            last_complete_test_time = current_time;
            
            gpio_toggle_level(LED1);
            
            printf("=== RUNNING COMPLETE I2C TEST ===\r\n");
            Enhanced_Complete_Test();
        }
//        // 处理从机接收到的数据
//        if(slave_data_ready) {
//            printf("=== SLAVE DATA RECEIVED ===\r\n");
//            printf("Bytes: %d, Data: ", slave_receive_count);
//            for(i = 0; i < slave_receive_count; i++) {
//                uart_send_hex_8bit(i2c_slave_receive_buffer[i]);
//                uart_write_byte(UART_INDEX, ' ');
//            }
//            printf("\r\n");
//            slave_data_ready = 0;
//        }
//        
//        // 每3秒执行一次主机写入测试
//        if(current_time - last_write_time > 3000) {
//            last_write_time = current_time;
//            
//            Test_cnt++;
//            if(Test_cnt > 10000) Test_cnt = 0;

//            tmp[0] = Test_cnt / 1000;
//            tmp[1] = (Test_cnt % 1000) / 100;
//            tmp[2] = (Test_cnt % 100) / 10;
//            tmp[3] = Test_cnt % 10;
//            
//            gpio_toggle_level(LED1);
//            
//            printf("=== MASTER WRITING DATA ===\r\n");
//            WriteNbyte(0x00, tmp, 4);
//        }
//        
        // 每8秒执行一次完整通信测试
//        if(current_time - last_complete_test_time > 8000) {
//            last_complete_test_time = current_time;
//            Enhanced_Complete_Test();
//        }
//        
        system_delay_ms(10);
    }
}
