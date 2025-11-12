#ifndef _BSP_H_
#define _BSP_H_

#define PIT_CH                  (TIM0_PIT )                	// 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIMER0_IRQn)              	// 对应周期中断的中断编号

#define LED1                    (IO_P34)

#include <stdlib.h>
#include <stdio.h>

#include "zf_common_headfile.h"
#include "bsp.h"
#include "my_pit.h"
#include "my_uart.h"

void all_init(void);
void scheduler_run(void);

#endif // DEBUG