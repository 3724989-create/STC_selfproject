#include "bsp.h"


//对实时要求高的函数放在ISR(中断)，对实时性要求低的函数放在主循环中

uint8 task_num=0;

//全局任务调度标志位
 bit lcd_refresh_flag =0;   //LCD刷新标志位
 bit ccd_data_ready_flag=0; //CCD数据采集标志位

typedef struct scheduler
{
    void(*task_func)(void);
    uint32 rate_ms;
    uint32 last_run;
}scheduler_task_t;

static scheduler_task_t scheduler_task[]={
  // {led_process,100,0},
   {Servo_test_Process,10,0},
   //{self_ble_process,10,0}
  };  //1表示10ms

//初始化
void scheduler_Init(void)
{
    task_num=sizeof(scheduler_task)/sizeof(scheduler_task_t);
}

void all_init(void)
{
    //底层初始化
    clock_init(SYSTEM_CLOCK_30M);
	debug_init();
    //gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);
    scheduler_Init();   //任务调度器记得先初始化
    LCD_gpio_Init();
    //用户初始化
    self_pit_init();
    self_UART_init();
    //IIC_intit();
    Servo_init();
    CCD_init();
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
}


//CPU微延时函数
void delay_cpu_cycle(u8 cycles)
{
    while (cycles--);   //占用CPU周期实现微小延时
    
}



//执行任务表
   //hal_tick函数不更新容易卡

void scheduler_run(void)    //只执行轻量级任务
{
    int i;
    for(i=0;i<task_num;i++)
    {
      uint32 Now_Time=pit_read();
        
        if(Now_Time>=scheduler_task[i].rate_ms+scheduler_task[i].last_run)
        {
            scheduler_task[i].last_run=Now_Time;
            //更新最后时间
            scheduler_task[i].task_func();
            //执行相应代码
        }
    }
}