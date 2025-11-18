#ifndef _MY_PIT_H_
#define _MY_PIT_H_

#include "bsp.h"

extern uint8 pit_state;

void self_pit_init(void);
void pit_process(void);
void led_process(void);
uint32 pit_read(void);

#endif // DEBUG