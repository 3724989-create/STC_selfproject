/*---------------------------------------------------------------------*
os_pendsv_config.h

配制PendSv
*---------------------------------------------------------------------*/

#ifndef    __OS_PENDSV_251_H__
#define    __OS_PENDSV_251_H__



//模拟PendSv中断行为的相关宏定义
#define  PendSv_InterruptNumber         20      //PendSv使用的中断编号 (TIMTE4)  (使用纯数字,不要带修饰,例如　20u 编译通不过)
#define  PendSv_SetFlag() 	            T4IF=1  //设置PendSv标志对应的指令或函数
#define  PendSv_ClearFlag() 			T4IF=0  //清除PendSv标志对应的指令或函数
//#define  PendSv_GetFlag()               T4IF    //返回PendSv标志的表达式或函数


/*
//模拟PendSv中断行为的相关宏定义 (使用IO中断例子 @STC32G12K128)
#define  PendSv_InterruptNumber         42      //PendSv使用的中断向量号 ( IO中断 P00 IO引脚必须闲置 )      (使用纯数字,不要带修饰,例如　42u 编译通不过)
#define  PendSv_SetFlag() 	            P54=0   //设置PendSv标志对应的指令或函数 ( IO口低电平触发IO中断 )
#define  PendSv_ClearFlag() 			{ P54=1; _nop_(); _nop_(); _nop_(); _nop_(); P5INTF&=~BIT(4); }   //清除PendSv标志对应的指令或函数
//#define  PendSv_GetFlag()               (P5INTF&BIT(4))    //返回PendSv标志的表达式或函数
*/




#define  PendSv_EntryAddress            PendSv_InterruptNumber*8+3      //自动计算向量号对应的入口地址,此句无须修改

void prvPortPendSvInit(void);





#ifdef  OS_CPU_GLOBALS   //该宏控制以下的函数在整个项目中只被编译一次

/*-----------------------------------------------------------*
PendSv中断初始化
使用Timer4, 如改其它中断号需替换初始化代码
*-----------------------------------------------------------*/
void prvPortPendSvInit(void)
{
    //设置PendSv为最低优先级
    //其它相关代码
    T4IF=0;     //清除中断标志
    ET4=1;      //使能中断
}



/*-----------------------------------------------------------*
PendSv中断初始化 (使用IO中断的例子 @STC32G12K128)
*-----------------------------------------------------------*/
/*
#include "STC32G_GPIO.h"
void prvPortPendSvInit(void)
{
    //设置PendSv为最低优先级
    PINIPL  &= ~BIT(5);//最低优先级
    PINIPH  &= ~BIT(5);//最低优先级

    //其它相关代码
    P5IM0 &= ~BIT(4);   //低电平触发
    P5IM1 |= BIT(4);    //低电平触发
    P5_MODE_OUT_PP(GPIO_Pin_4); //IO口设置为推挽输出, 提高IO速度, IO口必须闲置
    
    P54 = 1;    _nop_();    _nop_();    _nop_();    _nop_();    //先IO写高电平,然后再清除中断标志, 写入有延迟, 等四个时钟再清中断标志, 否则可能清除标志后又被硬件重新设置
    P5INTF &= ~BIT(4);  //清除中断标志
    
    P5INTE |= BIT(4);   //使能中断
}
*/



#endif




#endif