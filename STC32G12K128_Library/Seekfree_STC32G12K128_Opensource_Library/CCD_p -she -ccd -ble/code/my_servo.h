#ifndef _MY_SERVO_H_
#define _MY_SERVO_H_

#include "bsp.h"

extern uint8 pit_state;

#define PWM_SERVO               (PWMA_CH2P_P54)

void Servo_init();
void Servo_test_Process();

#endif // DEBUG