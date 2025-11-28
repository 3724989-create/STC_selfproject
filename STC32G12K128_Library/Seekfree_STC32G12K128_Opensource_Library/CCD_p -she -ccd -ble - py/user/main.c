
#include "bsp.h"

void main()
{
   
  all_init();
  while(1)
  {  
      check_key_event();
      //CCD_process();
      if (lcd_refresh_flag)
      {
          lcd_refresh_flag = 0;
          Self_LCD_Progress(); 
      }

      if (tsl1401_finish_flag)
      {
          tsl1401_finish_flag = 0;
          tsl1401_collect_pit_handler();
          CCD_process();
      }
  }
}
