#include "self_button.h"

#define KEY1_PIN    IO_P51
#define KEY2_PIN    IO_P53
#define KEY3_PIN    IO_P52

// 开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;

// 上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;

// 开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;

uint8 key_value_test=0;

void self_key_init(void)
{
   	gpio_init(KEY1_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY2_PIN, GPI, 1, GPI_PULL_UP);
	gpio_init(KEY3_PIN, GPI, 1, GPI_PULL_UP); 
}

void key_value_process(void)
{
   		// 读取当前按键状态
        key1_status = gpio_get_level(KEY1_PIN);
        key2_status = gpio_get_level(KEY2_PIN);
        key3_status = gpio_get_level(KEY3_PIN); 

        //检测到按键按下之后  并放开置位标志位
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;

        if(key1_flag)   
        {
             system_delay_ms(10);
            key1_flag = 0;//使用按键之后，应该清除标志位
            key_value_test=1;
            //test1++;
        }
        
        if(key2_flag)   
        {
            system_delay_ms(10);
            key2_flag = 0;//使用按键之后，应该清除标志位
            key_value_test=2;
            //test2++;
        }
        
        if(key3_flag)   
        {
            system_delay_ms(10);
            key3_flag = 0;//使用按键之后，应该清除标志位
            key_value_test=3;
            //test3++;
        }
        
}