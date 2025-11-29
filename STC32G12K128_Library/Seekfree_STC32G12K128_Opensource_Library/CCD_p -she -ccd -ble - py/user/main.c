
#include "bsp.h"

void main()
{
   
  all_init();
  while(1)
  {  
      check_key_event();
      //CCD_process();
      CCD_main_process();
      
      if (lcd_refresh_flag)
      {
          lcd_refresh_flag = 0;
          Self_LCD_Progress(); 
      }

  }
}
