
#include "bsp.h"

void main()
{

	all_init();
	
    while(1)
    {
        // 清屏为红色
        LCD_Clear(RED);
        system_delay_ms(500);

        // 填充一个蓝色矩形
        LCD_Fill(50, 50, 150, 150, BLUE);
        system_delay_ms(500);

        // 清屏为绿色
        LCD_Clear(GREEN);
        system_delay_ms(500);

        // 绘制一个白点
        LCD_DrawPoint(120, 160, WHITE);
        system_delay_ms(500);
    }
}
