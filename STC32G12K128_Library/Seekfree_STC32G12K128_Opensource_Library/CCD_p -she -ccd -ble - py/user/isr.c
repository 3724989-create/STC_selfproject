

#include "zf_common_headfile.h"

void DMA_UART1_IRQHandler(void) interrupt 4
{
    static vuint8 dwon_count = 0;

    if (DMA_UR1R_STA & 0x01) // 接收完成
    {
        DMA_UR1R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_1); // 设置下一次接收，务必保留

        // 程序自动下载
        if (uart_rx_buff[UART_1][0] == 0x7F)
        {
            if (dwon_count++ > 20)
            {
                IAP_CONTR = 0x60;
            }
        }
        else
        {
            dwon_count = 0;
        }

        if (uart1_irq_handler != NULL)
        {
            uart1_irq_handler(uart_rx_buff[UART_1][0]);
        }
    }

    if (DMA_UR1R_STA & 0x02) // 数据丢弃
    {
        DMA_UR1R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_1); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART2_IRQHandler(void) interrupt 8
{
    if (DMA_UR2R_STA & 0x01) // 接收完成
    {
        DMA_UR2R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_2); // 设置下一次接收，务必保留

        if (uart2_irq_handler != NULL)
        {
            uart2_irq_handler(uart_rx_buff[UART_2][0]);
        }
    }

    if (DMA_UR2R_STA & 0x02) // 数据丢弃
    {
        DMA_UR2R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_2); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART3_IRQHandler(void) interrupt 17
{
    if (DMA_UR3R_STA & 0x01) // 接收完成
    {
        DMA_UR3R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_3); // 设置下一次接收，务必保留

        if (uart3_irq_handler != NULL)
        {
            uart3_irq_handler(uart_rx_buff[UART_3][0]);
        }
    }

    if (DMA_UR3R_STA & 0x02) // 数据丢弃
    {
        DMA_UR3R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_3); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void DMA_UART4_IRQHandler(void) interrupt 18
{
    if (DMA_UR4R_STA & 0x01) // 接收完成
    {
        DMA_UR4R_STA &= ~0x01;      // 清标志位
        uart_rx_start_buff(UART_4); // 设置下一次接收，务必保留

        if (uart4_irq_handler != NULL)
        {
            uart4_irq_handler(uart_rx_buff[UART_4][0]);
        }
    }

    if (DMA_UR4R_STA & 0x02) // 数据丢弃
    {
        DMA_UR4R_STA &= ~0x02;      // 清标志位
        uart_rx_start_buff(UART_4); // 设置下一次接收，务必保留
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
        // 如果进入了这个中断，则代表UART的数据在没有取走之前被覆盖!
    }
}

void TM0_IRQHandler() interrupt 1       //PIT
{
    TIM0_CLEAR_FLAG;

    if (tim0_irq_handler != NULL)
    {
        tim0_irq_handler();
    }
}

void TM1_IRQHandler() interrupt 3   //CCD
{
    TIM1_CLEAR_FLAG;

    if (tim1_irq_handler != NULL)
    {
        tim1_irq_handler();
    }
}

void TM2_IRQHandler() interrupt 12  //SERVO_pid
{
    TIM2_CLEAR_FLAG;

    if (tim2_irq_handler != NULL)
    {
        tim2_irq_handler();
    }
}

void TM3_IRQHandler() interrupt 19  //motor_pid
{
    TIM3_CLEAR_FLAG;

    if (tim3_irq_handler != NULL)
    {
        tim3_irq_handler();
    }
}

void TM4_IRQHandler() interrupt 20  //lazer_pid
{
    TIM4_CLEAR_FLAG;

    if (tim4_irq_handler != NULL)
    {
        tim4_irq_handler();
    }
}


//#define     INT0_VECTOR             0       //0003H
//#define     TMR0_VECTOR             1       //000BH
//#define     INT1_VECTOR             2       //0013H
//#define     TMR1_VECTOR             3       //001BH
//#define     UART1_VECTOR            4       //0023H
//#define     ADC_VECTOR              5       //002BH
//#define     LVD_VECTOR              6       //0033H
////#define   PCA_VECTOR              7       //003BH
//#define     UART2_VECTOR            8       //0043H
//#define     SPI_VECTOR              9       //004BH
//#define     INT2_VECTOR             10      //0053H
//#define     INT3_VECTOR             11      //005BH
//#define     TMR2_VECTOR             12      //0063H
//#define     USER_VECTOR             13      //006BH
//#define     BRK_VECTOR              14      //0073H
//#define     ICEP_VECTOR             15      //007BH
//#define     INT4_VECTOR             16      //0083H
//#define     UART3_VECTOR            17      //008BH
//#define     UART4_VECTOR            18      //0093H
//#define     TMR3_VECTOR             19      //009BH
//#define     TMR4_VECTOR             20      //00A3H
//#define     CMP_VECTOR              21      //00ABH
////#define   PWM_VECTOR              22      //00B3H
////#define   PWMFD_VECTOR            23      //00BBH
//#define     I2C_VECTOR              24      //00C3H
//#define     USB_VECTOR              25      //00CBH
//#define     PWMA_VECTOR             26      //00D3H
//#define     PWMB_VECTOR             27      //00DBH
//#define     CAN1_VECTOR             28      //00E3H
//#define     CAN2_VECTOR             29      //00EBH
//#define     LIN_VECTOR              30      //00F3H

//#define     RTC_VECTOR              36      //0123H
//#define     P0INT_VECTOR            37      //012BH
//#define     P1INT_VECTOR            38      //0133H
//#define     P2INT_VECTOR            39      //013BH
//#define     P3INT_VECTOR            40      //0143H
//#define     P4INT_VECTOR            41      //014BH
//#define     P5INT_VECTOR            42      //0153H
//#define     P6INT_VECTOR            43      //015BH
//#define     P7INT_VECTOR            44      //0163H
//#define     DMA_M2M_VECTOR          47      //017BH
//#define     DMA_ADC_VECTOR          48      //0183H
//#define     DMA_SPI_VECTOR          49      //018BH
//#define     DMA_UR1T_VECTOR         50      //0193H
//#define     DMA_UR1R_VECTOR         51      //019BH
//#define     DMA_UR2T_VECTOR         52      //01A3H
//#define     DMA_UR2R_VECTOR         53      //01ABH
//#define     DMA_UR3T_VECTOR         54      //01B3H
//#define     DMA_UR3R_VECTOR         55      //01BBH
//#define     DMA_UR4T_VECTOR         56      //01C3H
//#define     DMA_UR4R_VECTOR         57      //01CBH
//#define     DMA_LCM_VECTOR          58      //01D3H
//#define     LCM_VECTOR              59      //01DBH
//#define     DMA_I2CT_VECTOR         60      //01E3H
//#define     DMA_I2CR_VECTOR         61      //01EBH
//#define     I2S_VECTOR              62      //01F3H
//#define     DMA_I2ST_VECTOR         63      //01FBH
//#define     DMA_I2SR_VECTOR         64      //0203H
