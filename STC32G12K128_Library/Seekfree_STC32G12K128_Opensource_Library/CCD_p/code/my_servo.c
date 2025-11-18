#include "my_servo.h"

void Servo_init()
{
    pwm_init(PWM_SERVO, 50, 0); 
}

void Servo_test_Process()
{
        pwm_set_duty(PWM_SERVO, 300);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, 515);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, 730);
        system_delay_ms(500);
        pwm_set_duty(PWM_SERVO, 515);
        system_delay_ms(500);
}