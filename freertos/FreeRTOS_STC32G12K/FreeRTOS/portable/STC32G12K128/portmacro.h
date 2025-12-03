/*
 * FreeRTOS Kernel V10.5.1
 */


#ifndef PORTMACRO_H
#define PORTMACRO_H


#include    "stc32g.h"
#include    <INTRINS.H>



/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		float
#define portLONG		long
#define portSHORT		short
#define portSTACK_TYPE	uint8_t
#define portBASE_TYPE	char

typedef portSTACK_TYPE StackType_t;
typedef signed char BaseType_t;
typedef unsigned int UBaseType_t;


#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif

/*-----------------------------------------------------------*/

/* Hardware specifics. */
#define portSTACK_GROWTH			( 1 )
#define portTICK_PERIOD_MS			( ( uint32_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT			2
/*-----------------------------------------------------------*/


//#define portYIELD()                                 do { PendSv_SetFlag();while(PendSv_GetFlag());  } while( 0 )
#define portYIELD()                                 do { PendSv_SetFlag();NOP8();} while( 0 )
#define portEND_SWITCHING_ISR( xSwitchRequired )    do { if( xSwitchRequired != pdFALSE ) PendSv_SetFlag(); } while( 0 )
#define portYIELD_FROM_ISR( x )                     portEND_SWITCHING_ISR( x )
#define portNOP()	                                _nop_()


extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );
extern StackType_t* OS_Get_STACK_Addr(void);


#define portDISABLE_INTERRUPTS()				EA = 0						//关中断
#define portENABLE_INTERRUPTS()					EA = 1						//开中断
#define portENTER_CRITICAL()					vPortEnterCritical()		//任务进入临界段			
#define portEXIT_CRITICAL()						vPortExitCritical()			//任务退出临界段
#define portSET_INTERRUPT_MASK_FROM_ISR()		((!_testbit_(EA))?0X00:0X80)	//中断进入临界段 (关中断,返回值=关中断之前的中断状态)
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	{IE|=(uint8_t)x;}			    //恢复到进入临界段前的中断状态


/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vTaskFunction, pvParameters ) void vTaskFunction( void *pvParameters )
#define portTASK_FUNCTION( vTaskFunction, pvParameters ) void vTaskFunction( void *pvParameters )
    

//定义汇编宏

/*入栈*/
#define portSAVE_CONTEXT()                  \
{                                           \
    __asm   { PUSH  DR56    }               \
    __asm   { PUSH  DR28    }               \
    __asm   { PUSH  DR24    }               \
    __asm   { PUSH  DR20    }               \
    __asm   { PUSH  DR16    }               \
    __asm   { PUSH  DR12    }               \
    __asm   { PUSH  DR8     }               \
    __asm   { PUSH  DR4     }               \
    __asm   { PUSH  DR0     }               \
    __asm   { PUSH  PSW     }               \
}


/* 出栈*/
#define portRESTORE_CONTEXT()               \
{                                           \
    __asm   { POP   PSW     }               \
    __asm   { POP   DR0     }               \
    __asm   { POP   DR4     }               \
    __asm   { POP   DR8     }               \
    __asm   { POP   DR12    }               \
    __asm   { POP   DR16    }               \
    __asm   { POP   DR20    }               \
    __asm   { POP   DR24    }               \
    __asm   { POP   DR28    }               \
    __asm   { POP   DR56    }               \
}





#endif /* PORTMACRO_H */

