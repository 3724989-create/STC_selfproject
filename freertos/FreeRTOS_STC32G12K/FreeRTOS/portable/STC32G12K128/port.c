/*-----------------------------------------------------------*
FreeRTOS FOR 251
参考了STC的资料及程序进行改进
*-----------------------------------------------------------*/
#define  OS_CPU_GLOBALS


#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"


/*-----------------------------------------------------------*
变量声明
*-----------------------------------------------------------*/
__asm   
{   
    EXTRN         EDATA (pxCurrentTCB) 
    ?STACK      SEGMENT   EDATA
}


/*-----------------------------------------------------------*
滴答时钟初始化
*-----------------------------------------------------------*/
#define portTM0PS_VALUE    ( configCPU_CLOCK_HZ / configTICK_RATE_HZ / 65536UL )
#define portRELOAD_VALUE   ( 65536UL - configCPU_CLOCK_HZ / (portTM0PS_VALUE+1) / configTICK_RATE_HZ )
static void prvPortSetupTimerInterrupt( void )
{
	AUXR	|=	0x80;	//1T模式
	TMOD	&=	0XF0;	//模式0
	TM0PS	=	portTM0PS_VALUE;	//分频系数
    TL0 = ( uint8_t )( portRELOAD_VALUE );		//装载值
    TH0 = ( uint8_t )( portRELOAD_VALUE >> 8 );	//装载值
	TF0 = 0;	//清除标志
	ET0 = 1;	//使能中断
	TR0 = 1;	//开启定时器
}

/*-----------------------------------------------------------*
第一次切换到任务里去,使用这个函数
*-----------------------------------------------------------*/
BaseType_t xPortStartScheduler( void )
{
    prvPortSetupTimerInterrupt();	//滴答时钟初始化
    prvPortPendSvInit();			//PendSv中断初始化
    
	//读取任务的SP
    __asm   { MOV   DR4,pxCurrentTCB    }   
    __asm   { MOV   WR2,@WR6+0x2        }   
    __asm   { MOV   DR60,DR0    }
    
    //寄存器出栈
    portRESTORE_CONTEXT();
	
    //把返回地址格式做成ERET的返回格试
    //此处R4,R5,R6未被用于传递参数,可以使用,不影响结果
    __asm   { POP   R4      }               
    __asm   { POP   R6      }               
    __asm   { POP   R5      }               
    __asm   { POP   PSW1    }
    __asm   { PUSH  R5    }               
    __asm   { PUSH  R4    }               
    __asm   { PUSH  R6    }
    
    //开中断,开中断后至少会往下执行一条指令,执行完ERET指令后才能响应中断.
    __asm   { SETB  EA      }
    
    //用ERET指令转移到任务中去, 0XAA是ERET指令的编码
    __asm   { DB    0AAH    } 
		
    return pdTRUE;
}


/*-----------------------------------------------------------*
PendSv 中断做任务切换
*-----------------------------------------------------------*/
void PendSvIsr( void )
{
    __asm{  PendSvIsr_Entrance:     }
    __asm{  CLR     EA  }

    //寄存器入栈
    portSAVE_CONTEXT();
		
	//SP保存到任务控制块                        
    __asm{  MOV   DR0,DR60          }           
    __asm{  MOV   DR4,pxCurrentTCB  }   
    __asm{  MOV   @WR6+0x2,WR2      }

    PendSv_ClearFlag(); 
		
	//找出即将要运行的任务
	vTaskSwitchContext();  
		
	//读取任务的SP
    __asm{  MOV   DR4,pxCurrentTCB  }   
    __asm{  MOV   WR2,@WR6+0x2      }   
    __asm{  MOV   DR60,DR0          }           
		
    //寄存器出栈
    portRESTORE_CONTEXT();
	
    __asm{  SETB    EA  }
	__asm{  RETI        }              
}


/*-----------------------------------------------------------*
时钟滴答中断
*-----------------------------------------------------------*/
void Timer0_ISR_Handler (void) interrupt 1		
{
    EA=0;
    portYIELD_FROM_ISR( xTaskIncrementTick() );
    EA=1;
}


/*-----------------------------------------------------------*
进出临界区(任务级,可嵌套)
*-----------------------------------------------------------*/
static uint8_t data uxCriticalNesting = 0;
static bit _bEA;
void vPortEnterCritical( void ){
    if (!_testbit_(EA)) {
        if (uxCriticalNesting == 0) {
            _bEA = 0;
        }
    } else {
        if (uxCriticalNesting == 0) {
            _bEA = 1;
        }
    }
    uxCriticalNesting++;
}
void vPortExitCritical( void ){
    uxCriticalNesting--;
	if(uxCriticalNesting==0)EA=_bEA;
}

/*-----------------------------------------------------------*
以指针的形式返回#?STACK的值.
进入任务以后,主堆栈被闲置
实际上,函数返回一个大小为STACKSIZE的内存块可供自由使用.
*-----------------------------------------------------------*/
static StackType_t edata* c_stack =  0;
StackType_t* OS_Get_STACK_Addr(void)
{
    __asm   { MOV   WR0,     #WORD0(?STACK)   } 
    __asm   { MOV   c_stack, WR0                } 
    return c_stack;
}


/*-----------------------------------------------------------*
关闭任务调度器,这里不适用
*-----------------------------------------------------------*/
void vPortEndScheduler( void )
{
    /* Not implemented for this port. */
}

/*-----------------------------------------------------------*
初始化任务的栈数据,(模仿中断的动作)
*-----------------------------------------------------------*/
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
	uint32_t ulAddress;
	uint8_t index;

	ulAddress = ( uint32_t ) pxCode;
	*pxTopOfStack = 0;                                      /* PSW1 */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress >> 16 );    /* PC[23:16] */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress );          /* PC[7:0] */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress >> 8 );     /* PC[15:8] */

	pxTopOfStack++;
	*pxTopOfStack = 0;                                      /* R56 */
	pxTopOfStack++;
	*pxTopOfStack = 1;                                      /* R57(DPXL) */
	pxTopOfStack++;
	*pxTopOfStack = 0;                                      /* R58(DPH) */
	pxTopOfStack++;
	*pxTopOfStack = 0;                                      /* R59(DPL) */

	for(index = 0; index < 28; index++)
	{
		pxTopOfStack++;
		*pxTopOfStack = 0;
	}

	pxTopOfStack++;
	ulAddress = ( uint32_t ) pvParameters;
	*pxTopOfStack = ( StackType_t ) ( ulAddress >> 24 );    /* R0 */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress >> 16 );    /* R1 */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress >> 8 );     /* R2 */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) ( ulAddress );          /* R3 */
	pxTopOfStack++;
	*pxTopOfStack = ( StackType_t ) 0;                      /* PSW */

    return pxTopOfStack;
}

/*-----------------------------------------------------------*
C51 地址结构混乱, 这里主要是占用地址 (xdata*)NULL 和 (xdata*)NULL+1,
有较小的机会正好一个OS变量分配到 ((xdata*) 0) 时 ,
判断空地址语句将会出错!
*-----------------------------------------------------------*/
static uint16_t xdata _bNULL _at_ 0;


/*-----------------------------------------------------------*
PendSv  向量入口引导
*-----------------------------------------------------------*/
__asm{ 
    CSEG    AT  PendSv_EntryAddress  
    JMP     PendSvIsr_Entrance
}
   
    
    
