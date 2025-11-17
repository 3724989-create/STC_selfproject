

#include "zf_common_headfile.h"

#define CHANNEL_NUMBER          (4)

#define PWM_CH1                 (PWMA_CH1P_P10)
#define PWM_CH2                 (PWMA_CH2P_P22)
#define PWM_CH3                 (PWMA_CH3P_P64)
#define PWM_CH4                 (PWMA_CH4P_P34)
#define PWM_SERVO               (PWMA_CH2P_P54)

int16 duty = 0;
uint8 channel_index = 0;
pwm_channel_enum channel_list[CHANNEL_NUMBER] = {PWM_CH1, PWM_CH2, PWM_CH3, PWM_CH4};


void main()
{
    int i;
    
    clock_init(SYSTEM_CLOCK_30M);
	debug_init();

    pwm_init(PWM_SERVO, 50, 0);                                                // 初始化 PWM 通道 频率 50HZ 初始占空比 0%
    
    //元器件要共地才有用

    while(1)
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
}



