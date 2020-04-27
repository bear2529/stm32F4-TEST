//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
/////////////////////////////////////////////////////////////////////////////////

#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "stm32f4xx_dma.h"
#include "type.h"

//#include "FreeRTOS.h"
//#include "task.h"


//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收


#define dma_buffer_size 512

typedef struct{
	u8 *pbuff;
	u8 buffer[512];
	u16 txlen;
	u16 index;
	volatile u8 tc_flag;	//传输完成标志
}usart_IrqTx_type;

typedef struct{
	u8 *pbuff;
	u8 buffer[200];
	u16 rxlen;
	u16 index;
	volatile u8 rc_flag;	//接收完成标志
	volatile u8 idle_flag;
}usart_IrqRx_type;


	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	


extern u32 fc_trans_buff1_index ;
extern u32 fc_trans_buff2_index ;
extern u8 fc_trans_buff1_lock ;
extern u8 fc_trans_buff2_lock ;
extern uint8_t uart4_rx_buffer[512];
extern uint16_t uart4_rx_index ;
extern u8 fc_rec_one_pack_flag ;
extern usart_IrqTx_type transparent_tx_stru;

void uart_init(u32 bound);
extern void uart4_IRQHandler(void);



/* Define to prevent recursive inclusion -------------------------------------*/


/* Define    -----------------------------------------------------------------*/
#define	BUF1	0
#define BUF2	1
#define TS_UPSTREAM_BUF_SIZE  		8000	//2048	//透传上行buf size
#define TS_DOWNSTREAM_BUF_SIZE 		8000	//8192




/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
extern usart_dma_rx_type uart4_dma_rx;
extern usart_dma_rx_type usart6_dma_rx;
extern usart_IrqRx_type usart3_rx_stru;


/* Declare Golbal function   -------------------------------------------------*/

extern void uart4_dma_rx_irq(void);
extern void usart6_dma_rx_irq(void);

extern void usart3_init(u32 baud);
extern void usart3_irq(void);
extern void usart3_SendStr(char* SendBuf);
//extern void usart3_send(u8 *str,u8 n);


extern void transparent_loop(void);
extern void Usart3_Data_Executive(void);


#endif


