
#include "bsp.h"

void main()
{
    all_init();
  while(1)
  {  
      scheduler_run();
      
      if (lcd_refresh_flag)
      {
          lcd_refresh_flag = 0;
          Self_LCD_Progress(); // 执行耗时的 LCD 刷新任务
      }
     //led_process();
  }
}
