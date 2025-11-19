#include "my_pit.h"


static volatile uint32 systick_count=0;

uint8 pit_state = 0;

void pit_handler ();

void self_pit_init(void)
{
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);  		// 初始化 LED1 输出 默认低电平 推挽输出模式
	
    pit_ms_init(PIT_CH, 10);                          	// 初始化 PIT_CH0 为周期中断 1000ms 周期
		
    interrupt_set_priority(PIT_PRIORITY, 0);            	// 设置 PIT1 对周期中断的中断优先级为 0，0为最低优先级
    
	tim0_irq_handler = pit_handler;							// 设置定时器0周期中断回调函数	
}

void pit_process(void)
{
    	if(pit_state)
        {
            gpio_toggle_level(LED1);
            pit_state = 0;                                                      // 清空周期中断触发标志位
        }
		
}

void led_process(void)
{
    gpio_toggle_level(LED1);
    system_delay_ms(1000);
}

//读取时间常量
uint32 pit_read(void)
{
    return 	systick_count;
}

void pit_handler (void)
{
    pit_state = 1;                                                              // 周期中断触发 标志位置位
    systick_count++;
}