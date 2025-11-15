#include "IIC.h"
#include "my_uart.h"

u8 success = 1;  // 添加success变量定义
u8 DisplayFlag=0;
u8 display_index=0;
u8 found=0;
u8 addr=0;
u16 Test_cnt=0;

bit isda;                                       //设备地址标志
bit isma;                                       //存储地址标志
bit B_1ms;          //1ms标志

sbit I2C_SDA = P0^5;    // 主机SDA - P0.5 (可用)
sbit I2C_SCL = P0^4;    // 主机SCL - P0.4 (可用)

// 从设备引脚也使用可用的引脚
sbit I2C_SLAVE_SDA = P0^7;  // 从机SDA - P0.7 (可用)  
sbit I2C_SLAVE_SCL = P0^6;  // 从机SCL - P0.6 (可用)
// 添加函数声明
// 正确定义地址
#define SLAVE_ADDR_7BIT  0x30    // 7位从机地址
#define SLAVE_ADDR_WRITE 0x60    // 8位写地址 (SLAVE_ADDR_7BIT << 1)
#define SLAVE_ADDR_READ  0x61    // 8位读地址 (SLAVE_ADDR_7BIT << 1 | 0x01)

// 保持向后兼容
#define SLAW SLAVE_ADDR_WRITE
#define SLAR SLAVE_ADDR_READ
#define SLAVE_ADDR SLAVE_ADDR_7BIT  // 从机代码使用7位地址


// 添加I2C测试数据
u8 i2c_test_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
u8 i2c_read_buffer[16] = {0};
u8 i2c_slave_receive_buffer[16] = {0};  // 从设备接收缓冲区
u8 slave_data_ready = 0;  // 从设备数据就绪标志
u8 slave_receive_count = 0;

void uart_send_hex_8bit(u8 dat);
//void SendI2CDataToUart(u8 *data, u8 len);
//void sendI2CStatusToUART(char *status);

void uart_send_hex_8bit(unsigned char dat)
{
    uint8 hex_chars[]="0123456789ABCDEF";
    uart_write_byte(UART_INDEX,hex_chars[dat<<4]);
    uart_write_byte(UART_INDEX,hex_chars[dat&0x0F]);
}

//IIC是开漏输出!!!
void IIC_intit()
{
    WTST = 0;
    EAXFR = 1;
    CKCON = 0;

    // 只配置可用的P0.4-P0.7引脚为开漏模式
    // P0.4(SCL主机), P0.5(SDA主机), P0.6(SCL从机), P0.7(SDA从机)
    P0M1 = 0xF0;   // 1111 0000 - P0.4~P0.7设为开漏
    P0M0 = 0xF0;   // 1111 0000
    
    // 其他端口配置保持不变
    P1M1 = 0x30; P1M0 = 0x30;   
    P2M1 = 0x3c; P2M0 = 0x3c;   
    P3M1 = 0x50; P3M0 = 0x50;
    P4M1 = 0x3c; P4M0 = 0x3c;   
    P5M1 = 0x0c; P5M0 = 0x0c;   
    P6M1 = 0xff; P6M0 = 0xff;   
    P7M1 = 0x00; P7M0 = 0x00;

    I2CCFG = 0x00;

    display_index = 0;
    DisplayFlag = 0;
    isda = 1;
    isma = 1;
    addr = 0;
    
    // 初始化引脚
    I2C_SDA = 1;
    I2C_SCL = 1;
    I2C_SLAVE_SDA = 1;
    I2C_SLAVE_SCL = 1;
    
    EA = 1;
    gpio_set_level(LED1,0);
    printf("I2C Initialized - Using P0.4-P0.7 (P0.0-P0.3 are faulty)\r\n");
}

void I2C_Delay(void)
{
    u16 dly = 20;
    while(--dly);
}

void I2C_Start(void)
{
    I2C_SDA = 1;
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SDA = 0;
    I2C_Delay();
    I2C_SCL = 0;
    I2C_Delay();
}

void I2C_Stop(void)
{
    I2C_SDA = 0;
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SDA = 1;
    I2C_Delay();
}

void I2C_SendACK(void)
{
    I2C_SDA = 0;
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SCL = 0;
    I2C_Delay();
}

void I2C_SendNoACK(void)
{
    I2C_SDA = 1;
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    I2C_SCL = 0;
    I2C_Delay();
}

bit I2C_CheckACK(void)
{
    bit ack;
    
    I2C_SDA = 1;
    I2C_Delay();
    I2C_SCL = 1;
    I2C_Delay();
    ack = I2C_SDA;
    I2C_SCL = 0;
    I2C_Delay();
    
    return !ack;
}

bit I2C_SendByte(u8 dat)
{
    u8 i;
    
    for(i = 0; i < 8; i++) {
        if(dat & 0x80) 
            I2C_SDA = 1;
        else 
            I2C_SDA = 0;
        dat <<= 1;
        I2C_Delay();
        I2C_SCL = 1;
        I2C_Delay();
        // 调试：打印每一位的状态
         printf("[Master] Sending bit %d: SDA=%d, SCL=%d\n", i, I2C_SDA, I2C_SCL);
        I2C_SCL = 0;
        I2C_Delay();
    }
    
    return I2C_CheckACK();
}

u8 I2C_RecvByte(bit ack)
{
    u8 i, dat = 0;
    
    I2C_SDA = 1;
    
    for(i = 0; i < 8; i++) {
        I2C_SCL = 1;
        I2C_Delay();
        dat <<= 1;
        if(I2C_SDA) 
            dat |= 0x01;
        I2C_SCL = 0;
        I2C_Delay();
    }
    
    if(ack)
        I2C_SendACK();
    else
        I2C_SendNoACK();
        
    return dat;
}

void WriteNbyte(unsigned char addr, unsigned char *p, unsigned char number)   //typedef 问题
{
    unsigned char local_success = 1;  // 使用局部变量
    unsigned char i=0;
    
    //printf("[DEBUG] Starting I2C write...\r\n");
    I2C_Start();
    //printf("[DEBUG] Start condition sent\r\n");
    
    if(!I2C_SendByte(SLAW)) {
        I2C_Stop();
         //printf("Write Failed - No ACK at device address\n\r");
        return;
    }
    
    if(!I2C_SendByte(addr)) {
        I2C_Stop();
          //printf("Write Failed - No ACK at register address\n\r");
        return;
    }

    //printf("Writing I2C: Addr=0x");
    uart_send_hex_8bit(addr);
    //printf(" Data=");
   
    for(i=0;i<number;i++)
    {
       if(!I2C_SendByte(*p)) {
            local_success = 0;
            break;
        }
        uart_send_hex_8bit(*p);
        uart_write_byte(UART_INDEX, ' ');
        p++;
    }

    I2C_Stop();

     if(local_success) {
        printf("- Write OK\r\n");
    } else {
        printf("- Write Failed\r\n");
    }
}

void ReadNbyte(u8 addr, u8 *p, u8 number)
{
    u8 i=0;
    
    I2C_Start();
    if(!I2C_SendByte(SLAW)) {
        I2C_Stop();
        printf("Read Failed - No ACK at device address");
        return;
    }
    
    if(!I2C_SendByte(addr)) {
        I2C_Stop();
         printf("Read Failed - No ACK at register address");
        return;
    }
    
    I2C_Start();
    if(!I2C_SendByte(SLAR)) {
        I2C_Stop();
        printf("Read Failed - No ACK at read command");
        return;
    }
    printf("Reading I2C: Addr=0x");
    uart_send_hex_8bit(addr);
    printf(" Data=");

    for(i = 0; i < number; i++) {
        *p = I2C_RecvByte(i != (number-1));
        uart_send_hex_8bit(*p);
        uart_write_byte(UART_INDEX, ' ');
        p++;
    }
    
    I2C_Stop();
    printf("- Read OK\r\n");
}

// 简化的起始条件检测 - 非阻塞
int I2C_Slave_CheckStart(void)
{
    static bit last_sda = 1;
    static bit last_scl = 1;
    
    bit current_sda = I2C_SLAVE_SDA;
    bit current_scl = I2C_SLAVE_SCL;
    
    // 检测起始条件：SCL高电平期间SDA从高到低
    if(last_scl && current_scl &&  // SCL保持高
       last_sda && !current_sda) { // SDA从高变低
        last_sda = current_sda;
        last_scl = current_scl;
        return 1;
    }
    
    last_sda = current_sda;
    last_scl = current_scl;
    return 0;
}

// 简化的停止条件检测 - 非阻塞
int I2C_Slave_CheckStop(void)
{
    static bit last_sda = 1;
    static bit last_scl = 1;
    
    bit current_sda = I2C_SLAVE_SDA;
    bit current_scl = I2C_SLAVE_SCL;
    
    // 检测停止条件：SCL高电平期间SDA从低到高
    if(last_scl && current_scl &&  // SCL保持高
       !last_sda && current_sda) { // SDA从低变高
        last_sda = current_sda;
        last_scl = current_scl;
        return 1;
    }
    
    last_sda = current_sda;
    last_scl = current_scl;
    return 0;
}
bit I2C_Slave_ReceiveBit(u8 *byte, u8 bit_position)
{
    static u32 last_check_time = 0;
    u32 current_time = pit_read(); // 需要实现获取系统时间
    
    // 避免过于频繁的检查
    if(current_time - last_check_time < 1) {
        return 0;
    }
    last_check_time = current_time;
    
    // 等待时钟变高
    if(I2C_SLAVE_SCL) {
        // 读取数据位
        if(I2C_SLAVE_SDA) {
            *byte |= (1 << (7 - bit_position));
        }
        // 等待时钟变低
        while(I2C_SLAVE_SCL) {
            // 空循环等待
        }
        return 1; // 成功接收一个bit
    }
    
    return 0; // 尚未接收到完整的bit
}


void I2C_Slave_SendACK(void)
{
    // 设置SDA为低电平（ACK）
    I2C_SLAVE_SDA = 0;
    
    // 产生一个时钟脉冲
    I2C_SLAVE_SCL = 1;
    I2C_Delay();
    I2C_SLAVE_SCL = 0;
    I2C_Delay();
    
    // 释放SDA
    I2C_SLAVE_SDA = 1;
}

void I2C_Slave_Process(void)
{
    static u8 debug_counter = 0;
    static unsigned char slave_state = 0;
    static unsigned char current_bit = 0;
    static unsigned char received_byte = 0;
    unsigned char i;
    unsigned long wait_start;
    unsigned long heartbeat_counter;
     u8 received_7bit_addr;
    static u8 address_byte = 0;
    static u8 is_write_operation = 0;
    static u8 send_byte = 0;
    static u8 send_data_index = 0;
    static u8 last_bit_count;
    
    // 大幅减少调试输出
    if(heartbeat_counter++ > 100) {
        heartbeat_counter = 0;
        gpio_toggle_level(LED1);
    }
    
    if(debug_counter++ > 200) {
        debug_counter = 0;
        // uart_write_string(UART_INDEX,"[Slave] Active\r\n");
    }
    
    switch(slave_state) {
        case 0: // 等待起始条件
            if(I2C_Slave_CheckStart()) {
                slave_state = 1;
                current_bit = 0;
                received_byte = 0;
                if(debug_counter > 50) {
                    debug_counter = 0;
                    uart_write_string(UART_INDEX,"[Slave] Start detected\r\n");
                }         
            }   
            break;
            
        case 1: // 接收地址字节
            if(current_bit < 8) {
                // 简化接收逻辑
                if(I2C_SLAVE_SCL) {
                    received_byte = (received_byte << 1) | (I2C_SLAVE_SDA ? 1 : 0);
                    
                    last_bit_count = 0;
                    if(current_bit != last_bit_count) {
                        last_bit_count = current_bit;
                        printf("[Slave] Receiving bit %d, SDA=%d, current byte=0x%02X\r\n", 
                               7 - current_bit, I2C_SLAVE_SDA, received_byte);
                    }
                    
                    current_bit++;
                    // 等待SCL变低，有超时保护
                    wait_start = pit_read();  // 在循环内部获取时间
                    while(I2C_SLAVE_SCL) {
                        if(pit_read() - wait_start > 10) { // 10ms超时
                            slave_state = 0; // 超时复位
                            break;
                        }
                    }  // 添加缺失的括号
                }
            } else {
                // 检查地址匹配
                 received_7bit_addr = received_byte >> 1;
                printf("[Slave] Received address: 0x%02X, Expected: 0x%02X\r\n", received_7bit_addr, SLAVE_ADDR);
                if((received_byte >> 1) == SLAVE_ADDR) {
                I2C_Slave_SendACK();
            
                // 根据读写位决定进入接收模式还是发送模式
                if(received_byte & 0x01) {
                // 读操作 - 进入发送模式 (state = 3)
                slave_state = 3;
                current_bit = 0;
                send_data_index = 0;
                // 准备要发送的数据
                    if(slave_receive_count > 0) {
                        send_byte = i2c_slave_receive_buffer[0];
                    } else {
                        send_byte = 0xAA; // 默认测试数据
                    }
                uart_write_string(UART_INDEX, "[Slave] Entering transmit mode\r\n");
                printf("[Slave] Will send: 0x%02X\r\n", send_byte);
                } else {
                // 写操作 - 进入接收模式 (state = 2)
                slave_state = 2;
                slave_receive_count = 0;
                uart_write_string(UART_INDEX, "[Slave] Entering receive mode\r\n");
                }
            } else {
                slave_state = 0; // 地址不匹配，回到等待
            }
            current_bit = 0;
        }
            break;
            
        case 2: // 接收数据
            if(current_bit < 8) {
                if(I2C_SLAVE_SCL) {
                    received_byte = (received_byte << 1) | (I2C_SLAVE_SDA ? 1 : 0);
                    current_bit++;
                    // 等待SCL变低，有超时保护
                    wait_start = pit_read();
                    while(I2C_SLAVE_SCL) {
                        if(pit_read() - wait_start > 10) { // 10ms超时
                            slave_state = 0;
                            break;
                        }
                    }  // 添加缺失的括号
                }
            } else {
                // 存储数据
                if(slave_receive_count < sizeof(i2c_slave_receive_buffer)) {
                    i2c_slave_receive_buffer[slave_receive_count++] = received_byte;
                }
                I2C_Slave_SendACK();
                current_bit = 0;
                received_byte = 0;
            }
            // 检查停止条件
            if(I2C_Slave_CheckStop()) {
                if(slave_receive_count > 0) {
                    slave_data_ready = 1;
                }
                slave_state = 0;
                if(debug_counter > 25) {
                    uart_write_string(UART_INDEX,"[Slave] Stop detected\r\n");
                }
            }
            break;
         case 3: // 发送数据模式（主机读取）
            // 在SCL低电平时设置数据位
            if(!I2C_SLAVE_SCL && current_bit < 8) {
                // 设置当前数据位
                u8 bit_value = (send_byte >> (7 - current_bit)) & 0x01;
                I2C_SLAVE_SDA = bit_value;
                current_bit++;
                 printf("[Slave Debug] After setting - SDA pin: %d\r\n", I2C_SLAVE_SDA);
                // 调试输出
                if(current_bit == 1) {
                    printf("[Slave] Starting to send byte: 0x%02X\r\n", send_byte);
                }
            }
            
            // 检查是否完成一个字节的发送
            if(current_bit >= 8) {
                // 等待主机提供第9个时钟（ACK位）
                if(I2C_SLAVE_SCL) {
                    // 释放SDA，让主机发送ACK/NACK
                    I2C_SLAVE_SDA = 1;
                    
                    // 短暂延时确保信号稳定
                    system_delay_ms(1);
                    
                    // 检查主机的ACK
                    if(!I2C_SLAVE_SDA) {
                        // 主机发送了ACK，准备下一个字节
                        send_data_index++;
                        current_bit = 0;
                        
                        // 准备下一个要发送的字节
                        if(send_data_index < slave_receive_count) {
                            send_byte = i2c_slave_receive_buffer[send_data_index];
                        } else {
                            // 发送测试数据
                            send_byte = 0x50 + send_data_index;
                        }
                        
                        printf("[Slave] ACK received, next byte: 0x%02X\r\n", send_byte);
                    } else {
                        // 主机发送了NACK，停止发送
                        printf("[Slave] NACK received, stopping transmission\r\n");
                        slave_state = 0;
                    }
                }
            }
            
            // 检查停止条件
            if(I2C_Slave_CheckStop()) {
                printf("[Slave] Stop during transmission\r\n");
                slave_state = 0;
            }
            break;    
            
    }
}
    
void ScanI2CDevices(void)
{
    u8 i;

    printf("Scanning I2C devices (0x08-0x77)...\r\n");
        for(addr = 0x08; addr <= 0x77; addr++) {
        I2C_Start();
        if(I2C_SendByte(addr << 1)) {
            printf("Found device at: 0x");
            uart_send_hex_8bit(addr);
            printf("\r\n");
            found++;
        }
        I2C_Stop();
       // 短暂延时，避免总线冲突
        for( i = 0; i < 50; i++);
    }
    if(!found) {
    printf("No I2C devices found!\r\n");
    } else {
        printf("Scan completed. Found ");
        uart_send_hex_8bit(found);
        printf(" device(s)\r\n");
    }
}
void I2C_AutoTestProcess(void)
{
    u32 msecound=pit_read();
    static u8 test_phase=0;
    static u32 last_test_time=0;

//    if(msecound-last_test_time<3000){
//        retrun; //每三秒执行一次测试
//    }

    last_test_time=msecound;
    
    switch(test_phase){
        case 0:
            // 阶段0：写入测试数据
            printf("=== I2C Auto Test Phase 1: Writing Data ===\r\n");
            WriteNbyte(0x00, i2c_test_data, sizeof(i2c_test_data));
            break;
        case 1:
            // 阶段1：读取验证数据
            printf("=== I2C Auto Test Phase 2: Reading Data ===\r\n");
            ReadNbyte(0x00, i2c_read_buffer, sizeof(i2c_test_data));
            
            // // 验证数据是否正确
            // printf("Data Verification: ");
            // u8 verify_ok = 1;
            // for(u8 i = 0; i < sizeof(i2c_test_data); i++) {
            //     if(i2c_read_buffer[i] != i2c_test_data[i]) {
            //         verify_ok = 0;
            //         break;
            //     }
            // }
            // if(verify_ok) {
            //     printf("PASS\r\n");
            // } else {
            //     printf("FAIL\r\n");
            // }

            break;
        case 2:
            // 阶段2：扫描I2C设备
            printf("=== I2C Auto Test Phase 3: Scanning Devices ===\r\n");
            ScanI2CDevices();
            break;
        }
     test_phase++;
    if(test_phase > 2) {
        test_phase = 0;
    }

}

uint32 msecond=0;
// 修改IIC_Process函数，添加自动测试
void IIC_Process(void)
{
    u8 i=0;

    u8 tmp[4]={0};
    
    
//    if(DisplayFlag)
//    {
//        DisplayFlag = 0;
//    }
//    
//    // 处理I2C从设备通信
      I2C_Slave_Process();

//     // 如果从设备接收到数据，通过串口发送
//    if(slave_data_ready) {
//        printf("New Slave Data: ");
//        for(i = 0; i < 8; i++) {
//            uart_send_hex_8bit(i2c_slave_receive_buffer[i]);
//            uart_write_byte(UART_INDEX, ' ');
//        }
//        printf("\r\n");
//        slave_data_ready = 0;
//    }
//    

//    if(pit_state)
//    {
//        pit_state = 0;
    
//        if(msecond >= 1000)        //测试
//        {
//            msecond = 0;
            Test_cnt++;
            if(Test_cnt>10000) Test_cnt = 0;

            tmp[0] = Test_cnt / 1000;
            tmp[1] = (Test_cnt % 1000) / 100;
            tmp[2] = (Test_cnt % 100) / 10;
            tmp[3] = Test_cnt % 10;
            gpio_toggle_level(LED1);
            
            // // 执行I2C写入操作，并通过串口发送结果
            WriteNbyte(0, tmp, 4);
            
//        }
    //} 
}
void Test_I2C_Wiring(void)
{
    uart_write_string(UART_INDEX, "=== Testing I2C Wiring ===\r\n");
    
    // 测试1：主机发送起始条件，从机应该能检测到
    uart_write_string(UART_INDEX, "Sending START condition...\r\n");
    I2C_Start();
    system_delay_ms(100);
    I2C_Stop();
    
    // 测试2：检查从机引脚状态
    uart_write_string(UART_INDEX, "Checking slave pin states:\r\n");
    printf("SLAVE_SCL: %d, SLAVE_SDA: %d\r\n", I2C_SLAVE_SCL, I2C_SLAVE_SDA);
    
    // 测试3：简单的数据发送测试
    uart_write_string(UART_INDEX, "Sending test byte...\r\n");
    I2C_Start();
    if(I2C_SendByte(0x30)) {
        uart_write_string(UART_INDEX, "ACK received - Wiring OK!\r\n");
    } else {
        uart_write_string(UART_INDEX, "NO ACK - Check wiring!\r\n");
    }
    I2C_Stop();
    
    uart_write_string(UART_INDEX, "=== Wiring Test Complete ===\r\n\r\n");
}

void Check_Pin_Configuration(void)
{
    uart_write_string(UART_INDEX, "=== Pin Configuration Check ===\r\n");
    
    // 检查主机引脚
    printf("MASTER - SDA(P0.1): %d, SCL(P0.0): %d\r\n", I2C_SDA, I2C_SCL);
    
    // 检查从机引脚  
    printf("SLAVE - SDA(P0.3): %d, SCL(P0.2): %d\r\n", I2C_SLAVE_SDA, I2C_SLAVE_SCL);
    
    // 测试引脚输出能力
    uart_write_string(UART_INDEX, "Testing pin output...\r\n");
    
    // 测试主机SDA
    I2C_SDA = 0; system_delay_ms(100);
    printf("Master SDA set to 0: %d\r\n", I2C_SDA);
    I2C_SDA = 1; system_delay_ms(100);
    printf("Master SDA set to 1: %d\r\n", I2C_SDA);
    
    // 测试从机SDA
    I2C_SLAVE_SDA = 0; system_delay_ms(100);
    printf("Slave SDA set to 0: %d\r\n", I2C_SLAVE_SDA);
    I2C_SLAVE_SDA = 1; system_delay_ms(100);
    printf("Slave SDA set to 1: %d\r\n", I2C_SLAVE_SDA);
    
    uart_write_string(UART_INDEX, "=== Pin Check Complete ===\r\n\r\n");
}

void I2C_Slave_Simple_Test(void)
{
    static u32 last_print = 0;
   // 检测起始条件（简化版）
   static bit last_sda = 1, last_scl = 1;
   bit current_sda = I2C_SLAVE_SDA;
   bit current_scl = I2C_SLAVE_SCL;
    u32 current_time = pit_read();
    
    // 每500ms打印一次引脚状态
    if(current_time - last_print > 500) {
        last_print = current_time;
        
        printf("SLAVE PINS - SCL: %d, SDA: %d\r\n", 
               I2C_SLAVE_SCL, I2C_SLAVE_SDA);
        

        
        // 检测起始条件
        if(last_scl && current_scl && last_sda && !current_sda) {
            uart_write_string(UART_INDEX, "*** START Condition Detected! ***\r\n");
        }
        
        // 检测停止条件
        if(last_scl && current_scl && !last_sda && current_sda) {
            uart_write_string(UART_INDEX, "*** STOP Condition Detected! ***\r\n");
        }
        
        last_sda = current_sda;
        last_scl = current_scl;
    }
}
void Test_Without_External_Pullup(void)
{
    uart_write_string(UART_INDEX, "=== Testing with Internal Weak Pull-up ===\r\n");
    
    // 设置为准双向模式（使用内部弱上拉）
    P0M1 = 0x00;   // 所有P0口设为准双向
    P0M0 = 0x00;
    
    // 测试总线电平
    printf("Bus levels - SDA: %d, SCL: %d\r\n", I2C_SDA, I2C_SCL);
    
    // 简单通信测试
    I2C_Start();
    if(I2C_SendByte(0x30)) {  // 从机地址 + 写位
        uart_write_string(UART_INDEX, "ACK with weak pull-up!\r\n");
    } else {
        uart_write_string(UART_INDEX, "No ACK even with weak pull-up\r\n");
    }
    I2C_Stop();
    
    // 恢复开漏模式
    P0M1 = 0x0F;
    P0M0 = 0x0F;
}

void Complete_I2C_Communication_Test(void)
{
    u8 test_data[] = {0xAA, 0x55, 0x01, 0x02, 0x03};
    u8 read_buffer[5] = {0};
    int i;
    
    uart_write_string(UART_INDEX, "=== Complete I2C Communication Test ===\r\n");
    
    // 测试1：主机向从机写入数据
    printf("1. Master writing 5 bytes to slave...\r\n");
    I2C_Start();
    if(I2C_SendByte(0x60)) {  // 从机地址 + 写位
        uart_write_string(UART_INDEX, "   Address ACK received\r\n");
        
        // 发送数据
        for(i = 0; i < 5; i++) {
            if(I2C_SendByte(test_data[i])) {
                printf("   Data 0x%02X ACK received\r\n", test_data[i]);
            } else {
                printf("   Data 0x%02X NO ACK\r\n", test_data[i]);
            }
        }
    } else {
        uart_write_string(UART_INDEX, "   Address NO ACK\r\n");
    }
    I2C_Stop();
    
    // 短暂延时让从机处理数据
    system_delay_ms(10);
    
    // 测试2：主机从从机读取数据
    printf("2. Master reading from slave...\r\n");
    I2C_Start();
    if(I2C_SendByte(0x61)) {  // 从机地址 + 读位
        uart_write_string(UART_INDEX, "   Read address ACK received\r\n");
        
        // 读取数据
        for(i = 0; i < 5; i++) {
            read_buffer[i] = I2C_RecvByte(i != 4);  // 最后一个字节发送NACK
            printf("   Read data 0x%02X\r\n", read_buffer[i]);
        }
    } else {
        uart_write_string(UART_INDEX, "   Read address NO ACK\r\n");
    }
    I2C_Stop();
    
    printf("=== Complete Test Finished ===\r\n\r\n");
}

void Enhanced_Complete_Test(void)
{
    u8 test_data[] = {0xAA, 0x55, 0x01, 0x02, 0x03};
    u8 read_buffer[8] = {0};
    u8 verify_ok = 1;
    int i;
    
    uart_write_string(UART_INDEX, "=== Enhanced Complete I2C Test ===\r\n");
    
    // 阶段1：主机向从机写入数据
    printf("1. MASTER WRITING 5 bytes to slave...\r\n");
    I2C_Start();
    if(I2C_SendByte(0x60)) {
        uart_write_string(UART_INDEX, "   Address ACK received\r\n");
        
        for(i = 0; i < 5; i++) {
            if(I2C_SendByte(test_data[i])) {
                printf("   Data 0x%02X ACK received\r\n", test_data[i]);
            } else {
                printf("   Data 0x%02X NO ACK\r\n", test_data[i]);
            }
        }
    }
    I2C_Stop();
    
    // 等待从机处理数据
    system_delay_ms(50);
    
    // 阶段2：主机从从机读取数据
    printf("2. MASTER READING from slave...\r\n");
    I2C_Start();
    if(I2C_SendByte(0x61)) {
        uart_write_string(UART_INDEX, "   Read address ACK received\r\n");
        
        // 读取8个字节（从机应该返回之前写入的数据+测试数据）
        for( i = 0; i < 8; i++) {
            read_buffer[i] = I2C_RecvByte(i != 7);  // 最后一个字节发送NACK
            printf("   Read data[%d]: 0x%02X\r\n", i, read_buffer[i]);
        }
//     read_buffer[0] = I2C_RecvByte(0); // 发送NACK，表示只读取一个字节
//     printf("   Read data[0]: 0x%02X\r\n", read_buffer[0]);
    }
    I2C_Stop();
    
    // 阶段3：验证数据
    printf("3. DATA VERIFICATION:\r\n");
    
    for( i = 0; i < 5; i++) {
        if(read_buffer[i] == test_data[i]) {
            printf("   Byte[%d]: 0x%02X == 0x%02X true\r\n", i, read_buffer[i], test_data[i]);
        } else {
            printf("   Byte[%d]: 0x%02X != 0x%02X false\r\n", i, read_buffer[i], test_data[i]);
            verify_ok = 0;
        }
    }
    
    if(verify_ok) {
        uart_write_string(UART_INDEX, "   DATA VERIFICATION: PASSED! \r\n");
    } else {
        uart_write_string(UART_INDEX, "   DATA VERIFICATION: FAILED\r\n");
    }
    
    printf("=== Enhanced Test Complete ===\r\n\r\n");
}

void Diagnose_SCL_Problem(void)
{
    printf("=== TESTING NEW I2C PINS (P0.4-P0.7) ===\r\n" );
    
    // 检查SCL引脚状态
    printf("Initial SCL states:\r\n");
    printf("Master SCL: %d, Slave SCL: %d\r\n", I2C_SCL, I2C_SLAVE_SCL);
    
    // 测试主机SCL输出能力
    printf("Testing master SCL output:\r\n");
    
    I2C_SCL = 0;
    system_delay_ms(10);
    printf("Master SCL set to 0: Master=%d, Slave=%d\r\n", I2C_SCL, I2C_SLAVE_SCL);
    
    I2C_SCL = 1;
    system_delay_ms(10);
    printf("Master SCL set to 1: Master=%d, Slave=%d\r\n", I2C_SCL, I2C_SLAVE_SCL);
    
    // 测试从机SCL输出能力
    printf("Testing slave SCL output:\r\n");
    
    I2C_SLAVE_SCL = 0;
    system_delay_ms(10);
    printf("Slave SCL set to 0: Master=%d, Slave=%d\r\n", I2C_SCL, I2C_SLAVE_SCL);
    
    I2C_SLAVE_SCL = 1;
    system_delay_ms(10);
    printf("Slave SCL set to 1: Master=%d, Slave=%d\r\n", I2C_SCL, I2C_SLAVE_SCL);
    
    printf("=== SCL DIAGNOSIS COMPLETE ===\r\n\r\n");
}

void Diagnose_All_P0_Pins(void)
{
    int pin;
    u8 port_state;
    int i;
    printf("=== COMPLETE P0 PORT DIAGNOSIS ===\r\n");
    
    // 测试P0口所有引脚的输出能力
    for(pin = 0; pin < 8; pin++) {
        // 设置引脚为输出
        P0 = (1 << pin);  // 设置单个引脚为高，其他为低
        
        system_delay_ms(100);
        
        // 读取整个P0口的状态
        port_state = P0;
        
        printf("P0.%d set to 1 -> Port reads: 0x%02X (", pin, port_state);
        
        // 二进制显示
        for(i = 7; i >= 0; i--) {
            printf("%d", (port_state >> i) & 1);
        }
        printf(")\r\n");
    }
    
    // 重置P0口
    P0 = 0xFF;
    printf("=== P0 PORT DIAGNOSIS COMPLETE ===\r\n\r\n");
}