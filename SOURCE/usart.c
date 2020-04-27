//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
//All rights reserved


////////////////////////////////////////////////////////////////////////////////// 	


 
/* Define to prevent recursive inclusion -------------------------------------*/
#include "sys.h"
//#include "main.h"
#include "usart.h"	
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "delay.h"
#include "stdlib.h"
#include "type.h"
#include "ec20.h"
#include "init.h"


/* Define    -----------------------------------------------------------------*/


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
usart_dma_rx_type uart4_dma_rx;
usart_dma_rx_type usart6_dma_rx;

usart_IrqRx_type usart3_rx_stru;



uint8_t ts_upstream_buf1[TS_UPSTREAM_BUF_SIZE];
uint8_t ts_upstream_buf2[TS_UPSTREAM_BUF_SIZE];
uint8_t ts_downstream_buf1[TS_DOWNSTREAM_BUF_SIZE];
uint8_t ts_downstream_buf2[TS_DOWNSTREAM_BUF_SIZE];




/* Declare Golbal function   -------------------------------------------------*/

 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"					//FreeRTOSʹ��	  
#endif

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
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 
 


u32 fc_trans_buff1_index = 0;
u32 fc_trans_buff2_index = 0;
u8 fc_trans_buff1_lock = 0;
u8 fc_trans_buff2_lock = 0;


uint16_t uart4_rx_index = 0;
u8 fc_rec_one_pack_flag = 0;

usart_IrqTx_type transparent_tx_stru;







// ------------------------------   USART3   -----------------------------------------------------------------//


/*******************************************************************************
* Function Name  : usart3_init
* Description    : 
* Input          : BAUD
* Output         : None
* Return         : 
* date			 : 2019-9-2
* author		 : WangD 
*******************************************************************************/
void usart3_init(u32 baud)
{	// debug 
	USART_TypeDef* USARTx = USART3;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	// GPIO
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA9����ΪUSART3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
	//RCC
	

	//USART
	USART_InitStructure.USART_BaudRate = baud;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure); //��ʼ������1
	
	
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_RXNE , ENABLE);
	//USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);
	USART_Cmd(USARTx, ENABLE);  //ʹ�ܴ��� 
	
	//Usart2 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�2
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}

/*******************************************************************************
* Function Name  : usart3_send
* Description    : 
* Input          : dataָ��,���ݳ���
* Output         : None
* Return         : 
* date			 : 2019-9-2
* author		 : WangD 
*******************************************************************************/

void usart3_send(u8 *str,u8 n)
{
  while(n)
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
      USART_SendData(USART3,*str++);
	  n--;
	}
}

//����3��ӡ����
void usart3_SendStr(char* SendBuf)
{
	while(*SendBuf)
	{
		while((USART3->SR&0X40)==0);//�ȴ�������� 
        USART3->DR = (u8) *SendBuf; 
		SendBuf++;
	}
}

void usart3_irq(void)
{
	u8 Res,clear;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3, USART_IT_RXNE);
		Res = (u8)((USART3->DR) & 0x01ff);
		/*
		if( usart3_rx_stru.idle_flag ==1 )
		{
			usart3_rx_stru.idle_flag = 0 ;
			usart3_rx_stru.buffer[0] = Res;
			usart3_rx_stru.index = 1;			
		}
		else
		{
			if(Res == 0x0a || Res == 0x0d)
			{
				usart3_rx_stru.buffer[usart3_rx_stru.index++] = 0;
				usart3_rx_stru.rc_flag = 1;
			}
			else
		    	usart3_rx_stru.buffer[usart3_rx_stru.index++] = Res;
			
		}
		*/
		if(Res == 0x0d)
		{
			usart3_rx_stru.buffer[usart3_rx_stru.index++] = 0;
			usart3_rx_stru.index = 0;
		}
		else if(Res == 0x0a)
		{
			usart3_rx_stru.rc_flag = 1;
			usart3_rx_stru.index = 0;
		}
		else
			usart3_rx_stru.buffer[usart3_rx_stru.index++] = Res;
	}

	if(USART_GetITStatus( USART3, USART_IT_IDLE) != RESET)
	{
		clear = USART1->SR;
		clear = USART1->DR;
		usart3_rx_stru.idle_flag = 1;
	}
}



void Usart3_Data_Executive(void)
{
	u8 len = 0,length = 50;	
	u8  strRx[50],strTx[50],tmp[50];
	//u8 const *p;
	//u16 i;
	u32 tmpMyaddr;

	
	if( !(usart3_rx_stru.buffer[0] == 'A' && usart3_rx_stru.buffer[1] == 'T') )
	{
		return;
	}

	
	len = strlen((char*)usart3_rx_stru.buffer);
	
	memset(strRx,0,length);
	memset(strTx,0,length);
	memset(tmp,0,length);

	strncpy((char*)strRx,(char*)usart3_rx_stru.buffer,7);
	if(strcmp((char*)strRx,"AT+SER?") == 0)
	{
		//strTx[0] = '+';
		//strncat(strTx,"canid:");
		sprintf((char*)strTx,"+server_id:%02d;\r\n",server_id);
		//strncat(strTx,tmp);
		usart3_SendStr(strTx); 
	}
	else if ( strcmp((char*)strRx,"AT+SER=") == 0 )	//AT+CAN=201,200
	{
	    memset((char*)tmp,0,length);		
		strncpy((char*)tmp,(char*)(&usart3_rx_stru.buffer[7]),len-7);
		//memcpy(tmp,&UART1stru.rxData[7],len-7);
		sscanf((char*)tmp,"%d",&tmpMyaddr);
		//sum = tmpMyaddr + tmpFcaddr;
		if( server_id != (u8)tmpMyaddr )
		{
			server_id = (u8)tmpMyaddr;
			usart3_SendStr("+ser changed \r\n"); 
			state_4g.linked = false;
			return;
		}
		usart3_SendStr("+ser no changed \r\n"); 
	}
	
}




// ------------------------------   USART4   -----------------------------------------------------------------//
/*
uart4	:  	downstream 
buf		:	8192
baud	:	115200
*/

/*******************************************************************************
* Function Name  : usart4_init
* Description    : 
* Input          : BAUD
* Output         : None
* Return         : 
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void  uart4_init(u32 baud)
{
	USART_TypeDef* USARTx = UART4;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

	//buf
	uart4_dma_rx.p_buf1 = ts_downstream_buf1;
	uart4_dma_rx.p_buf2 = ts_downstream_buf2;
	uart4_dma_rx.bufsize = TS_DOWNSTREAM_BUF_SIZE;
	uart4_dma_rx.dma_buf_used = BUF1;

	// GPIO
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); //GPIOD8����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
	

	//USART
	USART_InitStructure.USART_BaudRate = baud;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//��ģʽ
	USART_Init(USARTx, &USART_InitStructure); //��ʼ������1
	
	
	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	USART_ClearFlag(USARTx, USART_FLAG_TC);

	
	
	
}

/*******************************************************************************
* Function Name  : DMA_UART4_Rx_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void DMA_UART4_Rx_Init(void)
{
	//DMA1,Channel4,stream2
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	
	//Usart NVIC setup
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream2_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;			//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ enable
	NVIC_Init(&NVIC_InitStructure);								
	
	DMA_DeInit(DMA1_Stream2);
	
	DMA_InitStructure.DMA_Channel             = DMA_Channel_4;               
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(UART4->DR));   
    //������ִ��uart4_init()
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)uart4_dma_rx.p_buf1;           
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralToMemory; //DMA_DIR_MemoryToPeripheral;    
    DMA_InitStructure.DMA_BufferSize          = uart4_dma_rx.bufsize;                                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;      
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;     
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;//        DMA_Mode_Circular     
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;             
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;

	DMA_Init(DMA1_Stream2, &DMA_InitStructure);
	//
	DMA_ITConfig(DMA1_Stream2, DMA_IT_TC, ENABLE);
	//enable usart dma receive
	USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA1_Stream2, ENABLE);	
}


void uart4_dma_rx_irq(void)
{
	if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TCIF2) != RESET)  
	{
		DMA_Cmd(DMA1_Stream2,DISABLE);
		DMA_ClearFlag(DMA1_Stream2,DMA_FLAG_TCIF2);	//?? TODO
		if(uart4_dma_rx.dma_buf_used == BUF1)
		{
			uart4_dma_rx.dma_buf_used = BUF2;
			DMA_SetCurrDataCounter(DMA1_Stream2, uart4_dma_rx.bufsize);
			DMA_MemoryTargetConfig(DMA1_Stream2, (uint32_t)uart4_dma_rx.p_buf2,DMA_Memory_0);
			
			uart4_dma_rx.buf1_full_flag = true;
		}
		else
		{
			uart4_dma_rx.dma_buf_used = BUF1;
			DMA_SetCurrDataCounter(DMA1_Stream2, uart4_dma_rx.bufsize);
			DMA_MemoryTargetConfig(DMA1_Stream2, (uint32_t)uart4_dma_rx.p_buf1,DMA_Memory_0);
			
			uart4_dma_rx.buf2_full_flag = true;
		}
	
		DMA_Cmd(DMA1_Stream2, ENABLE); 
	}
}



// ----------------------------------   USART6   -----------------------------------------------------------------//

/*
uart6	:  	upstream 
buf		:	1024
baud	:	115200
*/


/*******************************************************************************
* Function Name  : usart6_init
* Description    : 
* Input          : BAUD
* Output         : None
* Return         : 
* date			 : 
* author		 : 
*******************************************************************************/
void usart6_init(u32 baud)
{
	USART_TypeDef* USARTx = USART6;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure;

	//buf
	usart6_dma_rx.p_buf1 = ts_upstream_buf1;
	usart6_dma_rx.p_buf2 = ts_upstream_buf2;
	usart6_dma_rx.bufsize = TS_UPSTREAM_BUF_SIZE;
	usart6_dma_rx.dma_buf_used = BUF1;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	// GPIO
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA9����ΪUSART3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10
	
	
	//USART
	USART_InitStructure.USART_BaudRate = baud;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx ;	//�շ�ģʽ
	USART_Init(USARTx, &USART_InitStructure); //��ʼ������1
	
	
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	
	
}


/*******************************************************************************
* Function Name  : DMA_USART6_Rx_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
//DMA2 ,STREAM1, CHANNEL5
void DMA_USART6_Rx_Init(void)
{
	//DMA2,Channel5,stream1
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	//Usart NVIC setup
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;			//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//IRQ enable
	NVIC_Init(&NVIC_InitStructure);								
	
	DMA_DeInit(DMA2_Stream1);
	
	DMA_InitStructure.DMA_Channel             = DMA_Channel_5;               
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART6->DR));   
    //������ִ��usart6_init()
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)usart6_dma_rx.p_buf1;           
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_PeripheralToMemory; //DMA_DIR_MemoryToPeripheral;    
    DMA_InitStructure.DMA_BufferSize          = usart6_dma_rx.bufsize;                                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;      
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;     
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;//        DMA_Mode_Circular     
    DMA_InitStructure.DMA_Priority            = DMA_Priority_High;             
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
	//
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
	//enable usart dma receive
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA2_Stream1, ENABLE);	
}

/*******************************************************************************
* Function Name  : usart6_dma_rx_irq
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/

void usart6_dma_rx_irq(void)
{
	if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) != RESET)  
	{
		DMA_Cmd(DMA2_Stream1,DISABLE);
		DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);	//?? TODO
		if(usart6_dma_rx.dma_buf_used == BUF1)
		{
			usart6_dma_rx.dma_buf_used = BUF2;
			DMA_SetCurrDataCounter(DMA2_Stream1, usart6_dma_rx.bufsize);
			DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t)usart6_dma_rx.p_buf2,DMA_Memory_0);
			
			usart6_dma_rx.buf1_full_flag = true;
		}
		else
		{
			usart6_dma_rx.dma_buf_used = BUF1;
			DMA_SetCurrDataCounter(DMA2_Stream1, usart6_dma_rx.bufsize);
			DMA_MemoryTargetConfig(DMA2_Stream1, (uint32_t)usart6_dma_rx.p_buf1,DMA_Memory_0);
			
			usart6_dma_rx.buf2_full_flag = true;
		}
	
		DMA_Cmd(DMA2_Stream1, ENABLE); 
	}
}

void debug_usart(void)
{
	usart3_SendStr("AA");
}

// ------------------------------   Transparent task   -----------------------------------------------------------------//
/*******************************************************************************
* Function Name  : transparent_loop
* Description    : ͸������ѭ��
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
u32 up_count = 0,down_count = 0;
u8 tmpcount[100];

void transparent_loop(void)
{
	usart3_init(115200);
	uart4_init(115200);
	usart6_init(115200);
	DMA_UART4_Rx_Init();
	DMA_USART6_Rx_Init();

	//EC20_POWERON;
	usart3_SendStr("usart task run...\r\n");		
	
	while(1)
	{
		//debug_usart();
		if(usart3_rx_stru.rc_flag == 1)
		{
			usart3_rx_stru.rc_flag = 0;
			Usart3_Data_Executive();
			
		}
		
		//down stream
		if(uart4_dma_rx.buf1_full_flag == true )
		{
			uart4_dma_rx.buf1_full_flag = false;
			is_downstream_buf1_full = true;
			if(UART1_debug == 1)
			{
				down_count += TS_DOWNSTREAM_BUF_SIZE;
				sprintf((char*)tmpcount,"down = %d\r\n",down_count);
				usart3_SendStr(tmpcount);
			}
		}
		if(uart4_dma_rx.buf2_full_flag == true)
		{
			uart4_dma_rx.buf2_full_flag = false;
			is_downstream_buf2_full = true;
			if(UART1_debug == 1)
			{
				down_count += TS_DOWNSTREAM_BUF_SIZE;
				sprintf((char*)tmpcount,"down = %d\r\n",down_count);
				usart3_SendStr(tmpcount);
			}
		}
		//up stream
		if(usart6_dma_rx.buf1_full_flag == true)
		{
			usart6_dma_rx.buf1_full_flag = false;
			is_upstream_buf1_full = true;
			if(UART1_debug == 1)
			{
				up_count += TS_UPSTREAM_BUF_SIZE;
				sprintf((char*)tmpcount,"+++++up = %d\r\n",up_count);
				usart3_SendStr(tmpcount);
			}
		}
		if(usart6_dma_rx.buf2_full_flag == true)
		{
			usart6_dma_rx.buf2_full_flag = false;
			is_upstream_buf2_full = true;
			if(UART1_debug == 1)
			{
				up_count += TS_UPSTREAM_BUF_SIZE;
				sprintf((char *)tmpcount,"+++++up = %d\r\n",up_count);
				usart3_SendStr(tmpcount);
			}
		}
	}
}





