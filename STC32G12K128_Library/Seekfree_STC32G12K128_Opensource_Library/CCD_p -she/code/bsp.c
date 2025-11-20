#include "bsp.h"


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
  {led_process,100,0},
  //{self_UART_process,1,0},
  {Servo_test_Process,1,0},
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