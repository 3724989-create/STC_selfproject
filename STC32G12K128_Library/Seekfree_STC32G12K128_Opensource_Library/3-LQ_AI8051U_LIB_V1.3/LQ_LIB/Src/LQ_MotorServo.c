/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技STC32位核心板
 【编    写】龙邱科技
 【E-mail  】chiusir@163.com
 【软件版本】V1.0 版权所有，单位使用请先联系授权
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 --------------------------------------------------------------------------------
 【  IDE  】 keil C251 V5.60
 【Target 】 STC32G/STC8051U/AI8051U 32位模式
 【SYS CLK】 42 MHz使用内部晶振
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

//#include "LQ_MotorServo.h"
#include "include.h"


#ifdef USE7971

/**************************************************************************************
 * 函 数 名：void Motor_Init(u16 freq)
 * 功    能：电机PWM初始化 适用于7971驱动
 * 参    数：u16 freq：设置产生PWM的频率，也决定输出PWM的满占空比时间
 * 作    者：LQ008
 * 最后更新: 2024年11月17日
 * 软件版本: V1.0
 * 调用举例: MotorInit(12500)  //12.5KHZ   @45.1584MHz时最大占空比时间设置值0~3613
 *           PWM频率 (计算方式：Period或freq = (u16)(MAIN_Fosc/freq)
 **************************************************************************************/
void Motor_Init(u16 freq)
{
	PWMx_InitDefine		PWMx_InitStructure;

    PWMx_Duty PWMB_Duty;
    PWMB_Duty.PWM5_Duty = 0;
    PWMB_Duty.PWM6_Duty = 0;
    PWMB_Duty.PWM7_Duty = 0;
    PWMB_Duty.PWM8_Duty = 0;

    u16 PWMx_Period = (u16)(MAIN_Fosc / freq);

    //PWM5设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO5P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM5, &PWMx_InitStructure);		    //始化PWM5,  PWMB
    //PWM6设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect =ENO6P;                //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM6, &PWMx_InitStructure);			//始化PWM6,   PWMB
    //PWM7设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO7P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM7, &PWMx_InitStructure);           //始化PWM7,   PWMB
    //PWM8设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO8P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM8, &PWMx_InitStructure);           //始化PWM8,  PWMB
    //PWMB 通道5~8设置
	PWMx_InitStructure.PWM_Period        = PWMx_Period;     //周期时间,  0~Period   (计算方式：Period = MAIN_Fosc/freq)
	PWMx_InitStructure.PWM_DeadTime      = 0;               //区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;          //能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);           //始化PWM通用寄存器,  PWMA,PWMB

    //PWMB 通道5-8 输出管脚选择
    PWM5_USE_P50();
    PWM6_USE_P51();
    PWM7_USE_P52();
    PWM8_USE_P53();

    //PWM输出管脚设置（启动PWM功能后输出脚自动设置为推挽输出模式 AI8051U需要手动设置推挽输出）
	P5_MODE_OUT_PP(GPIO_Pin_LOW);               //P50~P53四个管脚PWM输出模式设为 推挽输出

	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);     //PWMA,NVIC中断设置，优先级可根据实际情况适当调整

    UpdatePwm(PWMB, &PWMB_Duty);
}


/*********************************************************************************
 * 函 数 名：void Motor_Ctrl(int16 Motor1, int16 Motor2)
 * 功    能：7971双路电机控制函数
 * 参 数 值：Motor1：电机1占空比；Motor2：电机2占空比
 * 返 回 值：无
 * 作    者：LQ008
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 * 调用举例：MotorCtrl (1000,-2000); //两个电机一正一反转动
 **********************************************************************************/
void Motor_Ctrl (int16 Motor1,int16 Motor2)
{
  if (Motor1 < 0)
  {
      Motor1=0-Motor1;
      UpdatePwmCh(PWM5, 0);
      UpdatePwmCh(PWM6, Motor1);
  }
  else
  {
      UpdatePwmCh(PWM5, Motor1);
      UpdatePwmCh(PWM6, 0);
  }

  if (Motor2 < 0)
  {
      Motor2=0-Motor2;
      UpdatePwmCh(PWM7, 0);
      UpdatePwmCh(PWM8, Motor1);
  }
  else
  {
      UpdatePwmCh(PWM7, Motor1);
      UpdatePwmCh(PWM8, 0);
  }
}


 /*  USE7971*/

#elif defined USEDRV8701
/**************************************************************************************
 * 函 数 名：void Motor_Init(u16 freq)
 * 功    能：电机PWM初始化   适用于8701驱动
 * 返 回 值：无
 * 作    者：LQ008
 * 最后更新: 2024年11月17日
 * 软件版本: V1.0
 * 调用举例: MotorInit();  //12.5KHz @45.1584MHz时最大占空比时间设置值为0~3613
 *           PWM频率 计算方式：Period = (u16)(MAIN_Fosc/freq)  自适应主频
 **************************************************************************************/
void Motor_Init(u16 freq)
{
	PWMx_InitDefine		PWMx_InitStructure;

    u16 PWMx_Period = (u16)(MAIN_Fosc / freq);
    //PWM5设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO5P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM5, &PWMx_InitStructure);		    //始化PWM5,  PWMB
//    //PWM6设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect =ENO6P;                //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM6, &PWMx_InitStructure);			//始化PWM6,   PWMB
    //PWM7设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO7P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM7, &PWMx_InitStructure);           //始化PWM7,   PWMB
//    //PWM8设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect = ENO8P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM8, &PWMx_InitStructure);           //始化PWM8,  PWMB
    //PWMA 通道1~4设置
	PWMx_InitStructure.PWM_Period        = PWMx_Period;     //周期时间,  0~Period   (计算方式：Period = MAIN_Fosc/freq)
	PWMx_InitStructure.PWM_DeadTime      = 0;               //区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;          //能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMB, &PWMx_InitStructure);           //始化PWM通用寄存器,  PWMA,PWMB
    
#ifdef AI8051U_32Bit   //如果是STC32,选择PWM管脚后 需要对应修改GIO初始化，以下主要针对AI8051U，使用STC32G的自行修改
    //PWMB 通道5-8 输出管脚选择
    PWM5_USE_P50();
//    PWM6_USE_P51();       //设为IO,做8701方向控制IO1
    PWM7_USE_P52();
//    PWM8_USE_P53();       //设为IO,做8701方向控制IO2
#endif
    //PWM输出管脚设置（启动PWM功能后输出脚自动设置为推挽输出模式 AI8051U需要手动设置推挽输出）
	P5_MODE_OUT_PP(GPIO_Pin_0|GPIO_Pin_2);      //PWM输出管脚模式设为 推挽输出

    //电机驱动方向控制IO初始化
    P5_MODE_OUT_PP(GPIO_Pin_1|GPIO_Pin_3);      //设为IO,做8701方向控制IO,推挽输出默认高电平
    GPIO_Write_Pin(Motor_Dir1,RESET);           //设为低电平
    GPIO_Write_Pin(Motor_Dir2,RESET);

	NVIC_PWM_Init(PWMB,DISABLE,Priority_0);     //PWMA,NVIC中断设置，优先级可根据实际情况适当调整
    UpdatePwmCh(PWM5, 0);                       //要更新的通道以及占空比时间
    UpdatePwmCh(PWM7, 0);
//    UpdatePwm(PWMB, &PWMB_Duty);      //此法适合一次更新一组多通道，但需要先初始化结构体并给每个通道赋值，eg:PWMx_Duty PWMB_Duty;PWMB_Duty.PWM5_Duty=1200;
}


/**************************************************************************************
 * 函 数 名：void Motor_Ctrl(int16 Motor1,int16 Motor2)
 * 功    能：LQDRV8701双路电机控制函数
 * 参    数：Motor1：电机1占空比；Motor2：电机2占空比
 * 返 回 值：无
 * 作    者：LQ008
 * 最后更新: 2024年11月17日
 * 软件版本: V1.0
 * 调用举例: MotorInit(12500)  //12.5KHz @45.1584MHz
 *           PWM频率 计算方式：Period = (u16)(MAIN_Fosc/freq)
 **************************************************************************************/
void Motor_Ctrl(int16 Motor1,int16 Motor2)
{
  if (Motor1 < 0)
  {
      Motor1 = 0-Motor1;
      Motor_Dir1 = SET;
      UpdatePwmCh(PWM5, Motor1);      //0~3614@45.158MHz主频时
  }
  else
  {
      Motor_Dir1 = RESET;
      UpdatePwmCh(PWM5, Motor1);      //0~3614
  }

  if (Motor2 < 0)
  {
      Motor2 = 0-Motor2;
      Motor_Dir2 = SET;
      UpdatePwmCh(PWM7, Motor2);
  }
  else
  {
      Motor_Dir2 = RESET;
      UpdatePwmCh(PWM7, Motor2);
  }
}


#endif    /*  USE8701*/

/**************************************************************************************
 *  函 数 名：Test_Motor(void)
 *  功    能：测试标定输出PWM控制电机 ,注意使用哪个驱动需要在头文件中切换宏定义
 *  参    数：无
 *  返 回 值：无
 *  作    者：LQ008
 *  最后更新：2024年11月17日
 *  软件版本：V1.0
 *  调用举例：
 *  注意事项：注意，一定要先确定电池电压与电机额定电压 然后对电机输出进行限制，以免损坏电机等器件
------------------------------------------------------------------------------------
 * 使用龙邱母板测试电机及驱动流程：
 * 1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
 * 2.接好母板到驱动板的信号线及电源线；
 * 3.接好驱动板到电机的导线；
 * 4.烧写程序并运行，确定电机能正常转动后，开启驱动板电源开关；
 * 5.按键K0/K1确定电机转动速度及方向；
 * 6.如果出现疯转，按下K1键返回低速模式，或者直接关闭驱动板电源！
 **************************************************************************************/
void Test_Motor (void)
{
  short duty = 00;
  char txt[16];
  Motor_Init(Motor_FREQ);   //初始化电机PWM，参数PWM频率 
  //如果是STC32,选择PWM管脚后 需要对应修改GIO初始化，以下主要针对AI8051U，使用STC32G的自行修改

  OLED_Init();
  OLED_CLS();   						// 清屏
  OLED_P6x8Str(2, 0, "LQ Motor Test");
//  Motor_Init(Motor_FREQ);                          // 电机初始化

  printf("F:%.0f\n",MAIN_Fosc/12500.0);
  while (1)
  {
    if (KEY_Read(KEY0) == 0)            // 按下KEY0键，占空比减小
    {
		if (duty > -3614)               // 满占空比为3614 @45.1584 MHz主频 MAIN_Fosc/12500
        duty -= 50;
    }
    if (KEY_Read(KEY2) == 0)            // 按下KEY2键，占空比加大
    {
		if (duty <3614)
        duty += 50;
    }
    if (KEY_Read(KEY1) == 0)            // 按下KEY1键，占空比中值
    {
      duty = 50;
    }


    Motor_Ctrl(duty,duty);

    sprintf(txt, "PWM: %05d;", duty);
    OLED_P6x8Str(0, 5, txt);  		// 字符串显示
    UART1_PutStr(txt);

    LED_Ctrl(LED0, RVS);       		// 电平翻转,LED闪烁
    delay_ms(100);              	// 延时等待
  }

}







/**********************************************************************************
 * 函 数 名：void Servo_Init(u16 freq)
 * 功    能：舵机PWM初始化
 * 参 数 值：舵机PWM的频率  一般50或100 Hz
 * 返 回 值：无
 * 作    者：chiusir
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 * 调用举例：驱动舵机 ServoInit(100);  //100Hz
 *           PWM频率 计算方式：Period = (u16)(MAIN_Fosc/freq)  自适应主频
 ***********************************************************************************/
void Servo_Init(u16 freq)
{
	PWMx_InitDefine		PWMx_InitStructure;
	/* 注意这里的管脚选择与输出使能，PWA具有互补输出功能，因此用不到互补管脚输出，则需要ENOxN禁止输出*/
    /* PWM_EnoSelect = ENO1P|ENO1N;  //即通道1输出互补PWM，两个管脚占空比互补*/

    PWMA_Prescaler((MAIN_Fosc / 1000000 - 1));              //PWM时钟 预分频器设置 PWM分频到1M范围
    //PWM1设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO1P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM1, &PWMx_InitStructure);           //初始化PWM1,  PWMA
    //PWM2设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO2P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM2, &PWMx_InitStructure);           //初始化PWM2,  PWMA
//    //PWM3设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect = ENO3P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM3, &PWMx_InitStructure);           //初始化PWM3,  PWMA
//    //PWM4设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect = ENO4P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM4, &PWMx_InitStructure);           //初始化PWM4,  PWMA
    //PWMA 通道1~4设置
	PWMx_InitStructure.PWM_Period        = (u16)(1000000/freq);    //周期时间,   0~Period
	PWMx_InitStructure.PWM_DeadTime      = 0;               //死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;          //使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);           //初始化PWM通用寄存器,  PWMA

    //PWMA 通道1-4 输出管脚选择
    PWM1_USE_P10P11(); //仅使能P10输出即可，不需要互补管脚P11的输出，互补管脚还可做其他普通IO使用
    PWM2_USE_P12P13();
//    PWM3_USE_P14P15();
//    PWM4_USE_P16P17();

    //PWM输出管脚设置（启动PWM功能后输出脚自动设置为推挽输出模式 AI8051U需要手动设置推挽输出）
	P1_MODE_OUT_PP(GPIO_Pin_0|GPIO_Pin_2);
	NVIC_PWM_Init(PWMA,DISABLE,Priority_0);                     //PWMA,NVIC中断设置
    UpdatePwmCh(PWM1, 0);                        //要更新的通道以及占空比时间
    UpdatePwmCh(PWM2, 0);
//    UpdatePwmCh(PWM3, 0);                       //要更新的通道以及占空比时间
//    UpdatePwmCh(PWM4, 0);
//    UpdatePwm(PWMA, &PWMA_Duty);

}
/**********************************************************************************
 * 函 数 名：void Servo_Ctrl (u16 duty)
 * 功    能：舵机转角函数，由于小车拉杆范围限制，较小
 * 参 数 值：short Sduty，占空比，范围0--10000；
 * 作    者：LQ008
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 * 调用举例：ServoCtrl(1500);//舵机中值
 **********************************************************************************/
void Servo_Ctrl(u8 Sx, u16 Sduty)
{
  if (Sduty <= Servo_Median-1000)                  //限制幅值,根据实际情况进行调整
  {
      Sduty = Servo_Median-1000;
  }
  else if (Sduty >= Servo_Median+1000)            //限制幅值
  {
      Sduty = Servo_Median+1000;
  }
  if(Servo1==Sx)
      UpdatePwmCh(PWM1, Sduty);
  if(Servo1==Sx)
      UpdatePwmCh(PWM2, Sduty);
}


/******************************************************************************************
  * 函 数 名：Test_Servo(void)
  * 功    能：舵机PWM控制测试函数，测试标定输出PWM控制SD5/S3010舵机
  * 参 数 值：无
  * 返 回 值：无
  * 作    者：LQ008
  * 最后更新：2024年11月17日
  * 软件版本：V1.0
  * 注意事项：注意，一定要对舵机打角进行限制   0.5ms~2.5ms 对应duty在500~2600之间
    ----------------------------------------------------------------------------
  * 使用龙邱母板测试舵机流程：
  *  1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
  *  2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电，SD012舵机用5V供电！！！
  *  3.把舵机的舵盘去掉，让舵机可以自由转动；
  *  4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
  *  5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
  *  6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
 ******************************************************************************************/
void Test_Servo(void)
{
  char txt[36];
  u16 duty = Servo_Median;
  Servo_Init(Servo_FREQ);		// 舵机初始化，频率为50Hz,满占空比20000
  Servo_Ctrl(Servo1,Servo_Median); 	// 舵机中值
  Servo_Ctrl(Servo2,Servo_Median); 	// 舵机中值

  OLED_Init();
  OLED_CLS();   				// 清屏
  OLED_P6x8Str(22, 0, "LQ Servo Test");

  while (1)
  {
    if (!KEY_Read(KEY0))
    {
      if (duty > 500)  	 // 防止duty超
        duty -= 20;     // 标定的时候，可以把步长改小点，比如10
    }
    if (!KEY_Read(KEY1))
    {
      duty = Servo_Median;
    }
    if (!KEY_Read(KEY2))
    {
        if(duty<2600)
            duty += 20;
    }
    Servo_Ctrl(Servo1,duty);           //舵机1输出
    Servo_Ctrl(Servo2,duty);           //舵机1输出

    sprintf(txt, "Ser_duty:%05d   ", duty);
    OLED_P6x8Str(1, 2, txt); 	// 显示
    sprintf(txt, "S_Center:%05d ", Servo_Median);//显示当前宏定义舵机中值
    OLED_P6x8Str(1, 4, txt); 	// 显示
    LED_Ctrl(LED0, RVS);   		// LED闪烁;
    delay_ms(100);
  }

}







/******************************************************************************************
 * 函 数 名：void BLmotor_Init(u16 freq)
 * 功    能：无刷电机控制引脚初始化，频率需要和舵机控制频率保持一致，推荐50Hz
 * 参 数 值：u16 freq：无感无刷PWM输出的频率，范围40--300，建议50Hz
 * 返 回 值：无
 * 作    者：LQ008
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 * 调用举例：BLmotor_Init(50);  //初始化P20 P22引脚为PWM，频率50Hz,满占空比20000
 ******************************************************************************************/
void BLmotor_Init(u16 freq)
{
	PWMx_InitDefine		PWMx_InitStructure;
	/* 注意这里的管脚选择与输出使能，PWA具有互补输出功能，因此用不到互补管脚输出，则需要ENOxN禁止输出*/
    PWMA_Prescaler((MAIN_Fosc / 1000000 - 1));              //PWM时钟 预分频器设置 PWM分频到1M范围
//    //PWM1设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect = ENO1P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM1, &PWMx_InitStructure);           //初始化PWM1,  PWMA
//    //PWM2设置
//	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
//	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
//	PWMx_InitStructure.PWM_EnoSelect = ENO2P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
//	PWM_Configuration(PWM2, &PWMx_InitStructure);           //初始化PWM2,  PWMA
    //PWM3设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式, CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO3P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM3, &PWMx_InitStructure);           //初始化PWM3,  PWMA
    //PWM4设置
	PWMx_InitStructure.PWM_Mode      = CCMRn_PWM_MODE1;     //模式,	CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	PWMx_InitStructure.PWM_Duty      = 0;                   //PWM占空比时间, 0~Period
	PWMx_InitStructure.PWM_EnoSelect = ENO4P;               //输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	PWM_Configuration(PWM4, &PWMx_InitStructure);           //初始化PWM4,  PWMA
    //PWMA 通道1~4设置
	PWMx_InitStructure.PWM_Period        = 1000000/freq;    //周期时间,   0~Period，最高可设置65535
	PWMx_InitStructure.PWM_DeadTime      = 0;               //死区发生器设置, 0~255
	PWMx_InitStructure.PWM_MainOutEnable = ENABLE;          //主输出使能, ENABLE,DISABLE
	PWMx_InitStructure.PWM_CEN_Enable    = ENABLE;          //使能计数器, ENABLE,DISABLE
	PWM_Configuration(PWMA, &PWMx_InitStructure);           //初始化PWM通用寄存器,  PWMA

    //PWMA 通道1-4 输出管脚选择
//    PWM1_USE_P10P11(); //仅使能P10输出即可，不需要互补管脚P11的输出，互补管脚还可做其他普通IO使用
//    PWM2_USE_P12P13();
    PWM3_USE_P14P15();
    PWM4_USE_P16P17();

    //PWM输出管脚设置（启动PWM功能后输出脚自动设置为推挽输出模式 AI8051U需要手动设置推挽输出）
	P1_MODE_OUT_PP(GPIO_Pin_4|GPIO_Pin_6);

	NVIC_PWM_Init(PWMA,DISABLE,Priority_0);                     //PWMA,NVIC中断设置
//    UpdatePwmCh(PWM1, 5000);                       //要更新的通道以及占空比时间
//    UpdatePwmCh(PWM2, 5000);
    UpdatePwmCh(PWM3, 0);                       //要更新的通道以及占空比时间,初始化后设为0
    UpdatePwmCh(PWM4, 0);
//    UpdatePwm(PWMA, &PWMA_Duty);
}

/******************************************************************************************
 * 函 数 名：void BLmotor_Ctrl(u16 BLmotor1,u16 BLmotor2)
 * 功    能：BLmotor 无感无刷电调控制程序，当PWM高电平时间在1-2ms区间内，无刷电机旋转，2ms时转速最高
 * 参 数 值：u16 BLmotor1，占空比，范围0--10000；
 * 作    者：LQ008
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 * 调用举例：BLmotor_Ctrl(1000，1000);//当频率为50时，设置高电平时间为1ms
 ******************************************************************************************/
void BLmotor_Ctrl(u16 BLmotor1,u16 BLmotor2)
{
 if (BLmotor1 <= 900)                  //限制幅值,根据实际情况进行调整
 {
    BLmotor1 = 900;
 }
 else if (BLmotor2 >= 1990)            //限制幅值
 {
    BLmotor2 = 1990;
 }
    UpdatePwmCh(PWM3, BLmotor1);
    UpdatePwmCh(PWM4, BLmotor2);
}
/*****************************************************************************************
 * 函 数 名：Test_BLSmotor(void)
 * 功    能：无感无刷电机测试程序，控制无刷电机转动
 * 参 数 值：无
 * 返 回 值：无
 * 最后更新：2024年11月17日
 * 软件版本：V1.0
 ---------------------------------------------------------------------------------------
 * 注意事项：信号线接无感无刷驱动板后，需要将控制板和驱动板共地
 *  无刷电机控制函数,;PWM高电平时间在1-2ms内电机旋转，
 *  由于无刷电机旋转需要初速度，所以当duty达到初速度后，电机开始转动
 *  设置初始化频率时，频率需要与舵机频率保持一致，推荐50Hz频率，满占空比为20000
 ****************************************************************************************/
void Test_BLSmotor(void)
{
  char txt[36];
  u16 duty = 1000;

  BLmotor_Init(Servo_FREQ);	    // 频率为50Hz，此处频率需要与舵机频率保持一致,满占空比20000
  BLmotor_Ctrl(1000,1000); 	    // 设置初始duty，高电平时间为1ms，电机速度为0
//  Servo_Init(50);				// 舵机初始化，频率为50Hz,满占空比20000
//  Servo_Ctrl(Servo_Center); 	// 舵机中值

  OLED_Init();
  OLED_CLS();   				// 清屏
  OLED_P6x8Str(10, 0, "LQ BLSmotor Test");

  while (1)
  {
    if (!KEY_Read(KEY0))
    {
        if(duty >=1000)
            duty -= 100;
    }
    if (!KEY_Read(KEY1))
    {
        duty = 1000;
    }
    if (!KEY_Read(KEY2))
    {
        if(duty <=2000)
            duty += 100;
    }
    BLmotor_Ctrl(duty,duty); //无刷电机控制函数,;PWM高电平时间在1-2ms内电机旋转，
                              //由于无刷电机旋转需要初速度，所以当duty达到初速度后，电机开始转动
    sprintf(txt, "BLS_Duty:%05d ", duty);
    OLED_P6x8Str(10, 3, txt); 	// 显示
    LED_Ctrl(LED0, RVS);   		// LED闪烁;
    delay_ms(100);
  }

}
