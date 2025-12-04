#include "my_servo.h"

#define S_PWM_Fre 50    //舵机频率
#define S_PWM_Min 300   //最小舵机值
#define S_PWM_Max 730   //最大舵机值
#define S_PWM_Mid 515   //中间舵机值

void Servo_init()
{
    pwm_init(PWM_SERVO, S_PWM_Fre, 0); 
}

void Servo_test_Process()   //本身有延时
{
        pwm_set_duty(PWM_SERVO, S_PWM_Min);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, S_PWM_Mid);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, S_PWM_Max);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, S_PWM_Mid);
        system_delay_ms(500);
}