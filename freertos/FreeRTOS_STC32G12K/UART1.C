/*---------------------------------------------------------
UART1.C
----------------------------------------------------------*/

/*---------------------------------------------------------
文件包含
----------------------------------------------------------*/
#include "UART1.H"
#include ".\library\STC32G_Switch.H"
#include ".\library\STC32G_GPIO.h"

/*---------------------------------------------------------
发送部分定义及变量
----------------------------------------------------------*/
static u8 xdata  TX_BUF[UART1_TX_BUF_SIZE]; //数据池
static u16  TX_W_ADDR=0;        //W指针
static u16  TX_R_ADDR=0;        //R指针
static u16  TX_DMA_SIZE =0;     //正在DMA处理的字节数量
static bit  TX_IDLE=1;          //

/*---------------------------------------------------------
接收部分定义及变量
----------------------------------------------------------*/
static u8 xdata     RX_BUF[UART1_RX_BUF_SIZE];  //数据池
static u16          RX_R_ADDR=0;                //数据读到此处的指针


/*---------------------------------------------------------
DMA_UART1TX中断
----------------------------------------------------------*/
void DMA_UART1TX_ISR_Handler (void) interrupt 13
{
    DMA_UR1T_STA = 0;           //清除中断标志
    TX_R_ADDR+=TX_DMA_SIZE;     //发送指针递加
    if(TX_R_ADDR>=UART1_TX_BUF_SIZE)TX_R_ADDR=0;    //指针循环调整
    if(TX_W_ADDR!=TX_R_ADDR)    //如果有数据要发出
    {
        TX_DMA_SIZE = (TX_W_ADDR>TX_R_ADDR)? TX_W_ADDR-TX_R_ADDR : UART1_TX_BUF_SIZE-TX_R_ADDR;
        DMA_UR1T_AMTH = (TX_DMA_SIZE-1)>>8;             //字节数
        DMA_UR1T_AMT  = (TX_DMA_SIZE-1);                //字节数
        DMA_UR1T_TXAH = (u16)(&TX_BUF[TX_R_ADDR])>>8;   //地址
        DMA_UR1T_TXAL = (u16)(&TX_BUF[TX_R_ADDR]);      //地址
        DMA_UR1T_CR   = 0XC0;                           //启动DMA
    }
    else{
        TX_IDLE = 1;
    }
}


/*---------------------------------------------------------
DMA_UART1RX中断
----------------------------------------------------------*/
void DMA_UART1RX_ISR_Handler (void) interrupt 22    //借用22中断
{
    DMA_UR1R_STA = 0;   //清除中断标志
    DMA_UR1R_CR = 0XA1; //开DMA接收
}


/*---------------------------------------------------------
返回发送区容量
----------------------------------------------------------*/
u16 UART1_GetCapacity( void )
{
    u16 tx_r_addr;

    portENTER_CRITICAL();
    tx_r_addr = TX_R_ADDR;
    portEXIT_CRITICAL();
    return ( (TX_W_ADDR>=tx_r_addr)? UART1_TX_BUF_SIZE-(TX_W_ADDR-tx_r_addr)-1 : tx_r_addr-TX_W_ADDR-1);    //-1表示缓冲区留一字节,不全写满, 因为写满后TX_R_ADDR==TX_W_ADDR, 会被判断为无数据
}


/*---------------------------------------------------------
串口发送数据,缓冲区容量不足返回1
----------------------------------------------------------*/
u8 UART1_Send( void *pt, u16 Size)
{    
    u8 *buf = pt;
    u16 tx_w_addr;
    
    if(UART1_GetCapacity()<Size) return 1;
    
    tx_w_addr = TX_W_ADDR;
    
    if((tx_w_addr+Size)<UART1_TX_BUF_SIZE){      //复制数据
        memcpy(&TX_BUF[tx_w_addr],buf,Size);
        tx_w_addr+=Size;
    }
    else{
        u16 len = UART1_TX_BUF_SIZE-tx_w_addr;
        memcpy(&TX_BUF[tx_w_addr],buf,len);
        buf+=len;
        len=Size-len;
        memcpy(&TX_BUF[0],buf,len);
        tx_w_addr=len;
    }
    
    portENTER_CRITICAL();
    TX_W_ADDR = tx_w_addr;
    if(TX_IDLE){
        if(TX_W_ADDR!=TX_R_ADDR){
            TX_IDLE=0;
            TX_DMA_SIZE = (TX_W_ADDR>TX_R_ADDR)? TX_W_ADDR-TX_R_ADDR : UART1_TX_BUF_SIZE-TX_R_ADDR;
            DMA_UR1T_AMTH = (TX_DMA_SIZE-1)>>8;             //字节数
            DMA_UR1T_AMT  = (TX_DMA_SIZE-1);                //字节数
            DMA_UR1T_TXAH = (u16)(&TX_BUF[TX_R_ADDR])>>8;   //地址
            DMA_UR1T_TXAL = (u16)(&TX_BUF[TX_R_ADDR]);      //地址
            DMA_UR1T_CR   = 0XC0;                           //启动DMA
        }
    }
    portEXIT_CRITICAL();
        
    return 0;
}

/*---------------------------------------------------------
串口发送一字节,无容量返回1
----------------------------------------------------------*/
u8 UART1_Send_U8(u8 val)
{
    return UART1_Send(&val,1);
}

/*---------------------------------------------------------
串口发送一字,无容量返回1
----------------------------------------------------------*/
u8 UART1_Send_U16(u16 val)
{
    u8 buf[2];
    buf[1] = val;   val>>=8;
    buf[0] = val;
    return UART1_Send(buf,2);
}

/*---------------------------------------------------------
串口发送一字,无容量返回1
----------------------------------------------------------*/
u8 UART1_Send_U32(u16 val)
{
    u8 buf[4];
    buf[3] = val;   val>>=8;
    buf[2] = val;   val>>=8;
    buf[1] = val;   val>>=8;
    buf[0] = val;
    return UART1_Send(buf,4);
}


/*---------------------------------------------------------
返回DMA_UR1R_DONE
----------------------------------------------------------*/
static u16 DmaUr1rDoneNu(void)
{
    u8 a,b,c;
    portENTER_CRITICAL();
    do{
        a = DMA_UR1R_DONEH;
        b = DMA_UR1R_DONE;
        c = DMA_UR1R_DONEH;
    }while(a!=c);
    portEXIT_CRITICAL();
    return ((u16)c<<8)+(u16)b;
}


/*---------------------------------------------------------
读串口数据
----------------------------------------------------------*/
u16 UART1_Receive(u8 *buf, u16 Size)
{
    u16 w_addr,nu=0;
    if(Size==0)return 0;
    w_addr=DmaUr1rDoneNu();
    if(RX_R_ADDR==w_addr)return 0;
    nu = (w_addr>RX_R_ADDR)? w_addr-RX_R_ADDR : w_addr+UART1_RX_BUF_SIZE-RX_R_ADDR;
    nu = (nu<Size)? nu:Size;
    if((RX_R_ADDR+nu)<UART1_RX_BUF_SIZE){
        memcpy(buf,&RX_BUF[RX_R_ADDR],nu);
        RX_R_ADDR+=nu;
        return nu;
    }
    else{
        u16 len = UART1_RX_BUF_SIZE-RX_R_ADDR;
        memcpy(buf,&RX_BUF[RX_R_ADDR],len);
        buf+=len;
        len=nu-len;
        memcpy(buf,&RX_BUF[0],len);
        RX_R_ADDR=len;
        return nu;
    }
}

/*---------------------------------------------------------
DMA_UART1_Init
----------------------------------------------------------*/
static void DMA_UART1_Init(void)            
{
    DMA_UR1T_STA = 0x00;                            //清除标志
	DMA_UR1T_CFG = 0x8A;		                    //允许DMA中断, 中断Priority_2优先级,传输Priority_2优先级
    
    DMA_UR1R_STA  = 0x00;                           //清除标志
    DMA_UR1R_AMTH = (UART1_RX_BUF_SIZE-1)>>8;       //字节数
    DMA_UR1R_AMT  = (UART1_RX_BUF_SIZE-1);	        //字节数		
    DMA_UR1R_RXAH = (u16)RX_BUF>>8;                 //地址
    DMA_UR1R_RXAL = (u16)RX_BUF;                    //地址
	DMA_UR1R_CFG  = 0x8A;		                    //允许DMA中断, 中断Priority_2优先级,传输Priority_2优先级
    DMA_UR1R_CR = 0XA1;                             //开DMA接收
}


/*---------------------------------------------------------
初始化串口 
----------------------------------------------------------*/
void UART1_Init(u32 btl)
{
	P3_MODE_IO_PU(GPIO_Pin_0|GPIO_Pin_1);   //RXD TXD 设置为准双向口
    P3_PULL_UP_ENABLE(GPIO_Pin_1);          //TXD 4.1K上拉
    UART1_SW(UART1_SW_P30_P31);             //切换引脚
    
	SCON = 0x50;		                    //8位数据,可变波特率
	AUXR |= 0x01;		                    //串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		                    //定时器时钟1T模式
	T2L = (65536-(MAIN_Fosc/4/btl));        //设置定时初始值
	T2H = (65536-(MAIN_Fosc/4/btl))>>8;     //设置定时初始值
	AUXR |= 0x10;		                    //定时器2开始计时
    
    ES   = 0;                               //不开中断
    DMA_UART1_Init();                       //初始化DMA
}


/*---------------------------------------------------------
重写putchar
----------------------------------------------------------*/
#pragma functions (static)
char putchar (char c)  {
	UART1_Send(&c,1);
	return c;
}
/*---------------------------------------------------------
end
----------------------------------------------------------*/

