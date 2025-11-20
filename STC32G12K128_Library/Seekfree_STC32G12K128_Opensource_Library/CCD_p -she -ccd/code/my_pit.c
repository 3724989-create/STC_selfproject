#include "my_pit.h"


static volatile uint32 systick_count=0;

uint8 pit_state = 0;

void pit_handler ();

void self_pit_init(void)
{
    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);  		// 初始化 LED1 输出 默认低电平 推挽输出模式
	
    pit_ms_init(PIT_CH, 1);                          	// 初始化 PIT_CH0 为周期中断 1000ms 周期
		
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
    //实现非阻塞的500ms切换
    static u32 last_toggle_time=0;
    const u32 toggle_rate_ms=500;

    u32 current_time=pit_read();
    if(current_time-last_toggle_time>=toggle_rate_ms)
    {
        gpio_toggle_level(LED1);//切换颜色状态
        last_toggle_time=current_time;
    }
}

//读取时间常量
uint32 pit_read(void)
{
    return 	systick_count;
}

void pit_handler (void)
{
    static u8 lcd_count=0;
    static u8 ccd_count=0;

    //pit_state = 1;                                                              // 周期中断触发 标志位置位
    systick_count++;
    //50ms计数器
   
    if(++lcd_count>=10)
    {
        lcd_refresh_flag=1;//设置标志位
        lcd_count=0;
    }
    
    if(++ccd_count>=10)
    {
         tsl1401_collect_pit_handler();
         ccd_count=0;
    }
}