#ifndef _BSP_H_
#define _BSP_H_

#define PIT_CH                  (TIM0_PIT )                	// 使用的周期中断编号 如果修改 需要同步对应修改周期中断编号与 isr.c 中的调用
#define PIT_PRIORITY            (TIMER0_IRQn)              	// 对应周期中断的中断编号

#define LED1                    (IO_P34)

#include <stdlib.h>
#include <stdio.h>
#include "intrins.h"

#include "zf_common_headfile.h"
#include "bsp.h"
#include "my_pit.h"
#include "my_uart.h"
#include "IIC.h"

typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

#define MAIN_Fosc        24000000UL
#define Timer0_Reload   (65536UL -(MAIN_Fosc / 1000))       //Timer 0 中断频率, 1000次/秒
#define DIS_DOT     0x20
#define DIS_BLACK   0x10
#define DIS_        0x11


void all_init(void);
void scheduler_run(void);

#endif // DEBUG