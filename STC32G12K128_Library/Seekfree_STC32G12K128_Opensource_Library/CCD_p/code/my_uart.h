#ifndef _MY_USRT_H_
#define _MY_USRT_H_

#define UART_INDEX              (DEBUG_UART_INDEX   )                           // 默认 UART_1
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (DEBUG_UART_TX_PIN  )                           // 默认 UART1_TX_P31
#define UART_RX_PIN             (DEBUG_UART_RX_PIN  )       


#include "bsp.h"

void self_UART_init();
void self_UART_process();

#endif // DEBUG