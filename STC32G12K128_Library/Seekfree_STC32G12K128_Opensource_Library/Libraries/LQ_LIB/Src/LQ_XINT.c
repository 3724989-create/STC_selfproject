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
#include "include.h"


/***********************************************************************************
 * 函 数 名：void Exti_Init(void)
 * 功    能：STC传统外部中断初始化，管脚初始化和中断模式、使能等设置
 * 参    数：EXT_INTx，外部中断号
 * 返 回 值：无
 * 说    明：INT脚位于P3口：INTO:P3.2,INT1:P3.3,INT2:P3.6,INT3:P3.7,INT4:P3.0
 *           INT2、INT2、INT4仅支持下降沿触发中断
 **********************************************************************************/
void Exti_Init(u8 EXT_INTx)
{
	EXTI_InitTypeDef	Exti_InitStructure;		//结构定义
    //中断IO配置
    switch(EXT_INTx)
    {
        case EXT_INT0:
        {
            //INT0中断管脚设置
            P3_MODE_IO_PU(GPIO_Pin_2);                      //P3.2 设置为准双向口
            P3_PULL_UP_ENABLE(GPIO_Pin_2);                  //P3.2 口内部上拉电阻使能
            //中断模式初始化配置
            Exti_InitStructure.EXTI_Mode = EXT_MODE_Fall;   //中断模式设为下降沿触发, EXT_MODE_RiseFall,EXT_MODE_Fall
            Ext_Inilize(EXT_INT0,&Exti_InitStructure);      //初始化，EXT_INT0连接外部P3.2管脚
            NVIC_INT0_Init(ENABLE,Priority_0);              //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        //	NVIC_INT0_Init(ENABLE,NULL);		            //中断使能, ENABLE/DISABLE; 无优先级
        }break;
        case EXT_INT1:
        {
            //INT1中断管脚设置
            P3_MODE_IO_PU(GPIO_Pin_3);                      //P3.3 设置为准双向口
            P3_PULL_UP_ENABLE(GPIO_Pin_3);                  //P3.3 口内部上拉电阻使能
            //中断模式初始化配置
            Exti_InitStructure.EXTI_Mode = EXT_MODE_Fall;   //中断模式设为下降沿触发, EXT_MODE_RiseFall,EXT_MODE_Fall
            Ext_Inilize(EXT_INT1,&Exti_InitStructure);      //初始化，  EXT_INT1连接外部P3.3管脚
            NVIC_INT1_Init(ENABLE,Priority_0);              //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        //	NVIC_INT1_Init(ENABLE,NULL);                    //中断使能, ENABLE/DISABLE; 无优先级
        }break;
        case EXT_INT2:
        {
            //INT1中断管脚设置
            P3_MODE_IO_PU(GPIO_Pin_6);                      //P3.6 设置为准双向口
            P3_PULL_UP_ENABLE(GPIO_Pin_6);                  //P3.6 口内部上拉电阻使能
            //中断初始化配置
            Ext_Inilize(EXT_INT2,&Exti_InitStructure);      //仅进入函数清标志位，使能，默认下降沿触发，用户不可配置
            NVIC_INT2_Init(ENABLE,Priority_0);              //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        //	NVIC_INT2_Init(ENABLE,NULL);		            //中断使能, ENABLE/DISABLE; 无优先级
        }break;
        case EXT_INT3:
        {
            //INT1中断管脚设置
            P3_MODE_IO_PU(GPIO_Pin_7);                      //P3.6 设置为准双向口
            P3_PULL_UP_ENABLE(GPIO_Pin_7);                  //P3.6 口内部上拉电阻使能
            //中断初始化配置
            Ext_Inilize(EXT_INT3,&Exti_InitStructure);      //初始化仅进入函数清标志位，使能，默认下降沿触发，用户不可配置
            NVIC_INT3_Init(ENABLE,Priority_0);              //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        }break;
        case EXT_INT4:
        {
            //INT1中断管脚设置
            P3_MODE_IO_PU(GPIO_Pin_0);                      //P3.0 设置为准双向口
            P3_PULL_UP_ENABLE(GPIO_Pin_0);                  //P3.0 口内部上拉电阻使能
            //中断初始化配置
            Ext_Inilize(EXT_INT4,&Exti_InitStructure);      //初始化仅进入函数清标志位，使能，默认下降沿触发，用户不可配置
            NVIC_INT4_Init(ENABLE,Priority_0);              //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
        }break;
        default: //非法字段
            break;
    }
}


/***********************************************************************************
 * 函 数 名：void Test_ExINT()
 * 功    能：STC传统外部中断测试函数，中断服务函数中LED0状态
 * 参    数：无
 * 返 回 值：无
 * 说    明：INT脚位于P3口：INTO:P3.2,INT1:P3.3,INT2:P3.6,INT3:P3.7,INT4:P3.0
 *           INT2、INT2、INT4仅支持下降沿触发中断
 **********************************************************************************/
void Test_ExINT()
{
    GPIO_LED_Init();
    
    Exti_Init(EXT_INT1);    //初始化外部中断1 请前往 外部中断服务函数，编写中断服务程序
    
    OLED_Init();
    OLED_CLS();   			// 清屏
    OLED_P8x16Str(0,0, "Test EXINT ->P33");
    
    while(1)
    {
        ;
    }
}



//----------------------- 以下为 IO中断初始化相关内容 ，所有管脚皆可中断-------------------------
//  使用时可以注释掉用不到的，节省空间，若时STC32G12K128则无需担心空间问题


/***********************************************************************************
 * 函 数 名：u8 GPIO_INT_InitE(u8 GPIO, GPIO_InitTypeDef *GPIOx)
 * 功    能：初始化GPIO 端口中断,精确到每个IO口，初始化后立即使能，eg: P0INTE  的7个位对应P0.0到P0.7
 * 参    数：GPIO端口，I/O结构体（管脚和中断模式配置）
 * 返 回 值：无
 * 时    间：2024-11-18
 * 软件版本：V1.0
 **********************************************************************************/
u8 GPIO_INT_InitE(u8 GPIO, GPIO_InitTypeDef *GPIOx)
{
	if(GPIO > GPIO_P7)	return FAIL;	                //错误
	if(GPIOx->Mode > PxINT_MODE_HIGH)	return FAIL;	//错误
	if(GPIO == GPIO_P0)
	{
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P0IM1 &= ~(GPIOx->Pin),   P0IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P0IM1 &= ~(GPIOx->Pin),   P0IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P0IM1 |=  (GPIOx->Pin),   P0IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P0IM1 |=  (GPIOx->Pin),   P0IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P0_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P0INTE |= GPIOx->Pin;        //使能P0.Pin中断
    }
	if(GPIO == GPIO_P1)
	{
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P1IM1 &= ~(GPIOx->Pin),   P1IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P1IM1 &= ~(GPIOx->Pin),   P1IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P1IM1 |=  (GPIOx->Pin),   P1IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P1IM1 |=  (GPIOx->Pin),   P1IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P1_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P1INTE |= GPIOx->Pin;        //使能P1.Pin中断
	}
	if(GPIO == GPIO_P2)
	{
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P2IM1 &= ~(GPIOx->Pin),   P2IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P2IM1 &= ~(GPIOx->Pin),   P2IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P2IM1 |=  (GPIOx->Pin),   P2IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P2IM1 |=  (GPIOx->Pin),   P2IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P2_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P2INTE |= GPIOx->Pin;        //使能P2.Pin中断
	}
	if(GPIO == GPIO_P3)
	{
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P3IM1 &= ~(GPIOx->Pin),   P3IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P3IM1 &= ~(GPIOx->Pin),   P3IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P3IM1 |=  (GPIOx->Pin),   P3IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P3IM1 |=  (GPIOx->Pin),   P3IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P3_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P3INTE |= GPIOx->Pin;        //使能P3.Pin中断
	}
	if(GPIO == GPIO_P4)
	{
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P4IM1 &= ~(GPIOx->Pin),   P4IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P4IM1 &= ~(GPIOx->Pin),   P4IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P4IM1 |=  (GPIOx->Pin),   P4IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P4IM1 |=  (GPIOx->Pin),   P4IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P4_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P4INTE |= GPIOx->Pin;        //使能P4.Pin中断
	}
	if(GPIO == GPIO_P5)
	{
//        if(GPIOx->Pin > 6||GPIOx->Pin > 7) return FAIL;  //AI8051U 没有P5.6  P5.7
		if(GPIOx->Mode == PxINT_MODE_Fall)		{P5IM1 &= ~(GPIOx->Pin),   P5IM0 &= ~(GPIOx->Pin);}	 //下降沿中断
		if(GPIOx->Mode == PxINT_MODE_Rise)		{P5IM1 &= ~(GPIOx->Pin),   P5IM0 |=  (GPIOx->Pin);}	 //上升沿中断
		if(GPIOx->Mode == PxINT_MODE_LOW)		{P5IM1 |=  (GPIOx->Pin),   P5IM0 &= ~(GPIOx->Pin);}	 //低电平中断
		if(GPIOx->Mode == PxINT_MODE_HIGH)		{P5IM1 |=  (GPIOx->Pin),   P5IM0 |=  (GPIOx->Pin);}	 //高点平中断
        P5_MODE_IO_PU(GPIOx->Pin);   //准双向口
        P5INTE |= GPIOx->Pin;        //使能P5.Pin中断
	}
   /* 若有P6,P7还可添加 */
    return SUCCESS;	        //成功
}


/***********************************************************************************
 * 函 数 名: NVIC_PxINT_Init
 * 描    述: PxINT嵌套向量中断控制器初始化.
 * 参    数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
 * 返    回: 执行结果 SUCCESS/FAIL.
 * 版    本: V1.0, 2024-11-20
 **********************************************************************************/
u8 NVIC_PxINT_Init(u8 GPIO_Px, u8 Priority)
{
    if(GPIO_Px > GPIO_P7)	return FAIL;
    if(Priority <= Priority_3)
    {
        if(0 == Priority) PINIPH &= ~(0x01<<GPIO_Px), PINIPL &= ~(0x01<<GPIO_Px);
        if(1 == Priority) PINIPH &= ~(0x01<<GPIO_Px), PINIPL |=  (0x01<<GPIO_Px);
		if(2 == Priority) PINIPH |=  (0x01<<GPIO_Px), PINIPL &= ~(0x01<<GPIO_Px);
		if(3 == Priority) PINIPH |=  (0x01<<GPIO_Px), PINIPL |=  (0x01<<GPIO_Px);
    }
    else  return FAIL; //Px端口的中断优先级


//    switch(GPIO_Px)  //两种方式都可以
//    {
//        case GPIO_P0: PO_INT_Priority(Priority); break;
//        case GPIO_P1: P1_INT_Priority(Priority); break;
//        case GPIO_P2: P2_INT_Priority(Priority); break;
//        case GPIO_P3: P3_INT_Priority(Priority); break;
//        case GPIO_P4: P4_INT_Priority(Priority); break;
//        case GPIO_P5: P5_INT_Priority(Priority); break;
//        case GPIO_P6: P6_INT_Priority(Priority); break;
//        case GPIO_P7: P7_INT_Priority(Priority); break;
//        default: 
//            break;
//    }
//
	return SUCCESS;
}


/***********************************************************************************
 * 函 数 名：void GPIO_PxINT_Init()
 * 功    能：IO端口中断，初始化，设置中断IO，以及中断模式、端口优先级等
 * 参    数：无
 * 返 回 值：无
 * 说    明：AI8051U或STC32G支持所有普通I/O可中断，切记需要手动清除中断标志位（详见中断服务函数）
 **********************************************************************************/
void GPIO_PxINT_Init()
{
    GPIO_InitTypeDef  GPIO_PxINT_Structure;

    GPIO_PxINT_Structure.Pin  = GPIO_Pin_0|GPIO_Pin_3;  //要初始化的I/O  eg: GPIO_Pin_0|GPIO_Pin_2;
    GPIO_PxINT_Structure.Mode = PxINT_MODE_Fall;        //初始化的I/O 模式 下降沿触发中断
    GPIO_INT_InitE(GPIO_P0,&GPIO_PxINT_Structure);      //初始化管脚中断模式行 并 >使能< 其管脚中断！！！
//    P0_ENABLE_IOINT(Pinx);                              //  使能P0.Pinx管脚 IO中断，失能：P0_DISENABLE_IOINT(Pin)
    //根据初始化的中断模式选择是否需要开启，内部上下拉电阻
    P0_PULL_UP_ENABLE(GPIO_Pin_0);                      //使能P0.0 口内部上拉电阻！！！

    NVIC_PxINT_Init(GPIO_P0,Priority_0);                //中断优先级配置,对象是端口 PO_INT_Priority(Priority_0)；
}


/***********************************************************************************
 * 函 数 名：void Test_GPIO_PxINT()
 * 功    能：STC GPIO中断测试函数，中断服务函数中LED0状态
 * 参    数：无
 * 返 回 值：无
 * 说    明：AI8051U或STC32G支持所有普通I/O可中断，切记需要手动清除中断标志位（详见中断服务函数）
 **********************************************************************************/
void Test_GPIO_PxINT()
{

    GPIO_LED_Init();
    OLED_Init();
    GPIO_PxINT_Init();          //请前往 AI8051U_GPIO_ISR.C 中对应的中断服务函数中编写中断服务程序
    Global_IRQ_Enable();

    OLED_CLS();
    OLED_P8x16Str(0, 0,"Test P0_0 INT");

    while(1)
    {
        ;
    }
}
