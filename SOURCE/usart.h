//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
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
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����


#define dma_buffer_size 512

typedef struct{
	u8 *pbuff;
	u8 buffer[512];
	u16 txlen;
	u16 index;
	volatile u8 tc_flag;	//������ɱ�־
}usart_IrqTx_type;

typedef struct{
	u8 *pbuff;
	u8 buffer[200];
	u16 rxlen;
	u16 index;
	volatile u8 rc_flag;	//������ɱ�־
	volatile u8 idle_flag;
}usart_IrqRx_type;


	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	


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
#define TS_UPSTREAM_BUF_SIZE  		8000	//2048	//͸������buf size
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


