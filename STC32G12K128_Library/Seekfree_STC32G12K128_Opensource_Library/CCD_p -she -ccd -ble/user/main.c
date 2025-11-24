
#include "bsp.h"

void main()
{
    LCD_PrintfLine(1,"the number is %d",2);
    all_init();
  while(1)
  {  
      scheduler_run();
      
      if (lcd_refresh_flag)
      {
          lcd_refresh_flag = 0;
          Self_LCD_Progress(); 
      }

      if (tsl1401_finish_flag)
      {
          tsl1401_finish_flag = 0;
          ccd_process_data(); 
      }
  }
}
