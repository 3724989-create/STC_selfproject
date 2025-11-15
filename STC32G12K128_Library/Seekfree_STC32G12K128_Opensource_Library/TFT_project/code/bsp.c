#include "bsp.h"


uint8 task_num=0;

typedef struct scheduler
{
    void(*task_func)(void);
    uint32 rate_ms;
    uint32 last_run;
}scheduler_task_t;

static scheduler_task_t scheduler_task[]={
  {led_process,1,0},
  {self_UART_process,10,0},
};

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
    scheduler_Init();
    //用户初始化
    self_pit_init();
    self_UART_init();
    IIC_intit();
}


//执行任务表
   //hal_tick函数不更新容易卡

void scheduler_run(void)
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