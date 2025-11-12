#include "my_uart.h"

uint8       uart_get_data[64] = {0};                                                  // 串口接收数据缓冲区
uint8       fifo_get_data[64] = {0};                                                 // fifo 输出读出缓冲区


uint32      fifo_data_count = 0;                                                // fifo 数据个数

fifo_struct uart_data_fifo = {0};

void uart_rx_interrupt_handler (uint8 dat)
{
//    get_data = uart_read_byte(UART_INDEX);                                      // 接收数据 while 等待式 不建议在中断使用
    uart_query_byte(UART_INDEX, &dat);                                     // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&uart_data_fifo, &dat, 1);                           // 将数据写入 fifo 中
}


void self_UART_init()
{
    fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, uart_get_data, 64);              // 初始化 fifo 挂载缓冲区

    uart_init(UART_INDEX, UART_BAUDRATE, UART_TX_PIN, UART_RX_PIN);             // 初始化串口
    
	// UART1的中断优先级不能设置，为最低优先级值0
	uart_rx_interrupt(UART_INDEX, ZF_ENABLE);                                   // 开启 UART_INDEX 的接收中断
    

	// 设置中断回调函数
	uart1_irq_handler = uart_rx_interrupt_handler;
	
	
	uart_write_string(UART_INDEX, "UART Text.");                                // 输出测试信息
    uart_write_byte(UART_INDEX, '\r');                                          // 输出回车
    uart_write_byte(UART_INDEX, '\n');                                          // 输出换行

}

void self_UART_process()
{
        // fifo_data_count = fifo_used(&uart_data_fifo);                           // 查看 fifo 是否有数据
        // if(fifo_data_count != 0)                                                // 读取到数据了
        // {
        //    fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
        //    uart_write_string(UART_INDEX, "UART get data:");                     // 输出测试信息
        //    uart_write_buffer(UART_INDEX, fifo_get_data, (uint16)fifo_data_count);       // 将读取到的数据发送出去
        //    uart_write_string(UART_INDEX, "\r\n");                               // 输出测试信息
        // }
        uint32 time=0;

        time=pit_read();
        printf("the time now is:%d",time);
        printf("\r\n");

        system_delay_ms(1000);
        // 此处编写需要循环执行的
}

