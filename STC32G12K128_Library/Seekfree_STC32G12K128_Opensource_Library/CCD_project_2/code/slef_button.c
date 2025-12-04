#include "self_button.h"



key_t keys[KEY_COUNT]={
    //pin ,current_status,last_status,event_flag,.debounce_count
    {KEY1_PIN,KEY_UP,KEY_UP,0,0},
    {KEY2_PIN,KEY_UP,KEY_UP,0,0},
    {KEY3_PIN,KEY_UP,KEY_UP,0,0},
    {KEY4_PIN,KEY_UP,KEY_UP,0,0}
};

void self_key_init(void)
{
    int i=0;
    
    gpio_init(LED1,GPO,0,GPO_PUSH_PULL);
    gpio_init(LED2,GPO,1,GPO_PUSH_PULL);
    for(i=0;i<KEY_COUNT;i++)
    {
        gpio_init(keys[i].pin,GPI,1,GPI_PULL_UP);
    }
}
uint8 key_value_test=0;
// --- 按键扫描和去抖动逻辑 (建议在定时器中断中周期性调用，例如每 5ms) ---
void key_value_process(void)
{
    key_t* key;
    uint8 read_value=0;
    int i=0;
    
    for(i=0;i<KEY_COUNT;i++){
        key=&keys[i];
        //读取当前IO电平
        read_value=gpio_get_level(key->pin);
        
        
        if(read_value!=key->current_status){
            // IO 电平与当前确认状态不一致，开始/继续计数
            key->debounce_count++;
            if( key->debounce_count>=KEY_DEBOUNCE_CNT)
            {
                
                // 连续多次读取状态相同，确认状态已改变
                //  检测状态变化（核心：抬起检测）
                // 从上次确认按下 (!KEY_UP) 到本次确认抬起 (KEY_UP)
                if(key->current_status ==KEY_DOWN&& read_value==KEY_UP)
                {
                    key->event_flag = (i+1); // 设置标志位，i+1 即为 KeyID (1, 2, 3, 4)
                   
                }
                //更新状态
                key->last_status = key->current_status;
                key->current_status = read_value; // 更新为去抖成功的新状态
                
                key->debounce_count = 0; // 清除计数器
            }
        }
        else{
              key->debounce_count=0;
        }

    }
}


void check_key_event(void)
{
    int i ;
    key_t* key ;
    uint8 key_id;
    
    for (i = 0; i < KEY_COUNT; i++)
    {
        key = &keys[i]; 
        key_id = i+1; // 1, 2, 3, 4
        //key_value_test=key_id;
        // 检查是否有按键抬起事件
        if (key->event_flag == key_id)
        {
            // 1. 清除标志位（最先做，防止重复执行）
            key->event_flag = 0;
            
            // 2. 更新测试值
            key_value_test =  key_id; 
            

            // 3. 执行按键对应的操作 (使用 switch 分离逻辑)
            switch (key_id) {
                case 1: 
                    // KEY1 的操作
                    Menu_Move_Next();
                    // 仅 KEY1 影响 LED1
                    break;
                case 2:
                    // KEY2 的操作
                    Menu_Move_Prev();
                    break;
                case 3:
                    Menu_Select_Action();
                    break;
                case 4:
                    //gpio_toggle_level(LED2);
                    break;
            }
            
            // 4. 更新 LCD (放在最后，包含所有按键的最新状态)
            // 注意：LCD_PrintfLine 可能会阻塞一段时间

            // 可选：如果只需要响应一次按键抬起，可以 break 退出循环
            // break;
        }
    }
}

void LED1_process()
{
    gpio_toggle_level(LED1);
    
}

void LED2_process()
{
    gpio_toggle_level(LED2);
    
}