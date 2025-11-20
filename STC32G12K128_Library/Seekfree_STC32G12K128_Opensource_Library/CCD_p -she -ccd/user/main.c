
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
          Self_LCD_Progress(); // ִ�к�ʱ�� LCD ˢ������
      }

      if (tsl1401_finish_flag)
      {
          tsl1401_finish_flag = 0;
          // 执行数据压缩和处理 (计算中线等)
          ccd_process_data(); 
      }
  }
}
