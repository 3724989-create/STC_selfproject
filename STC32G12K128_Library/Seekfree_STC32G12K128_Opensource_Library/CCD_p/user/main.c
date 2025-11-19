
#include "bsp.h"

void main()
{
    all_init();
  while(1)
  {  
     Self_LCD_Progress(); //对应的色号不同应该
     led_process();
  }
}
