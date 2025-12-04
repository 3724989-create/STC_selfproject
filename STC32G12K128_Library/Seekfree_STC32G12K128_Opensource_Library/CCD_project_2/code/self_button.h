#ifndef _SELF_BUTTON_H_
#define _SELF_BUTTON_H_
#include "bsp.h"

// 定义按键数量
#define KEY_COUNT 4
// 定义去抖计数阈值 (假设 key_value_process 每 5ms 调用一次，这里是 20ms)
#define KEY_DEBOUNCE_CNT 2

#define KEY1_PIN    IO_P40
#define KEY2_PIN    IO_P41
#define KEY3_PIN    IO_P42
#define KEY4_PIN    IO_P43

extern uint8 key_value_test;

typedef enum{
    KEY_UP=1,   //未按下
    KEY_DOWN=0  //已按下
}KeyState;

//单个按键结构体定义
typedef struct{
    uint8 pin;
    uint8 current_status;
    uint8 last_status;              
    volatile uint8 event_flag;      //事件标志位
    volatile uint8 debounce_count;           //去抖计数器
}key_t;

extern key_t keys[KEY_COUNT];
//extern int key_value_test;

// 函数声明
void self_key_init(void);
void key_value_process(void);
void check_key_event(void); // 用于在主循环中处理按键事件
void LED1_process();
void LED2_process();

#endif // DEBUG