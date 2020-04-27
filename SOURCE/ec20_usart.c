//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


/* Define to prevent recursive inclusion -------------------------------------*/
#include "sys.h"
#include "usart.h"	
//#include "FreeRTOS.h"
//#include "task.h"
#include "string.h"
#include "delay.h"
//#include "stdio.h"
#include "stdlib.h"
#include "type.h"
#include "ec20_usart.h"


/* Define    -----------------------------------------------------------------*/



/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
usart_4g_rx_type usart_4g;

u8 ec20_rx_buf[EC20_RXBUF_SIZE];

volatile int32_t ITM_RxBuffer;


/* Declare local function   -------------------------------------------------*/
/*******************************************************************************
* Function Name  : change_1byte_to_2ascii
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-9-9
* author		 : WangD 
*******************************************************************************/
u16 change_1byte_to_2ascii(u8 byte)
{
	u16 ret = 0;
	ret = ((byte >> 4) < 0x0a) ? ((byte >> 4) + 0x30) : ((byte >> 4) + 0x37);
	ret <<= 8;
	ret |= ((byte & 0x0f) < 0x0a) ? ((byte & 0x0f) + 0x30) : ((byte & 0x0f) + 0x37);
	return ret;
}

/*******************************************************************************
* Function Name  : changeTAB_bytes_to_ascii
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-9-9
* author		 : WangD 
*******************************************************************************/
void changeTAB_bytes_to_ascii(const u8 * bytes_Tab, u8 * ascii_Tab, u8 len)
{
	u8 i = 0,j = 0;
	ascii_Tab[i++] = '^';
	ascii_Tab[i++] = '&';
	for(j = 0 ; j < len ; j++)
	{
		ascii_Tab[i++] = ((bytes_Tab[j] >> 4) < 0x0a) ? ((bytes_Tab[j] >> 4) + 0x30) : ((bytes_Tab[j] >> 4) + 0x37);
		ascii_Tab[i++] = ((bytes_Tab[j] & 0x0f) < 0x0a) ? ((bytes_Tab[j] & 0x0f) + 0x30) : ((bytes_Tab[j] & 0x0f) + 0x37);
	}
	ascii_Tab[i++] = '&';
	ascii_Tab[i++] = '^';
	//ascii_Tab[i++] = '\r';
	//ascii_Tab[i++] = '\n';
	ascii_Tab[i++] = 0;
}

/*******************************************************************************
* Function Name  : changeTAB_bytes_to_ascii
* Description    : 不添加 包头包尾
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-9-9
* author		 : WangD 
*******************************************************************************/
void changeTAB_bytes_to_ascii_nohead(const u8 * bytes_Tab, u8 * ascii_Tab, u8 len)
{
	u8 i = 0,j = 0;
	
	for(j = 0 ; j < len ; j++)
	{
		ascii_Tab[i++] = ((bytes_Tab[j] >> 4) < 0x0a) ? ((bytes_Tab[j] >> 4) + 0x30) : ((bytes_Tab[j] >> 4) + 0x37);
		ascii_Tab[i++] = ((bytes_Tab[j] & 0x0f) < 0x0a) ? ((bytes_Tab[j] & 0x0f) + 0x30) : ((bytes_Tab[j] & 0x0f) + 0x37);
	}
	ascii_Tab[i++] = 0;
}



// ------------------------------   USART1   -----------------------------------------------------------------//

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 0
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

// ------------------------------   ITM Debug serial  -----------------------------------------------------------------//

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	      
	return ITM_SendChar(ch);
}

//重定义 fgetc
int fgetc(FILE *f)
{
	while ( ITM_CheckChar() != 1 )
	{
	    __NOP();
	}
	return (ITM_ReceiveChar());
}

int __backspace()
{
 
	return 0;
 
}


#endif


/*******************************************************************************
* Function Name  : usart1_init
* Description    : 
* Input          : BAUD
* Output         : None
* Return         : 
* date			 : 
* author		 : 
*******************************************************************************/
void usart1_init(u32 baud)
{
	//USART_TypeDef* USARTx = USART1;

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	// GPIO
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
	

	//USART
	USART_InitStructure.USART_BaudRate = baud;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	
	USART_Cmd(USART1, ENABLE);  //使能串口1 
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	//Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级2
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	usart_4g.p_buf = ec20_rx_buf;
	usart_4g.bufsize = EC20_RXBUF_SIZE;
	
}

/*******************************************************************************
* Function Name  : EC20_USART_Config_Baud
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-9-2
* author		 : WangD 
*******************************************************************************/
void EC20_USART_Config_Baud(u32 baud)
{
	USART_InitTypeDef USART_InitStructure; 
	USART_Cmd(USART1, DISABLE);
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_Cmd(USART1, ENABLE);
}


void DMA_USART1_Tx_Init(void)
{
	//使用DMA2,Channel4,stream7
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	DMA_DeInit(DMA2_Stream7);
	
	DMA_InitStructure.DMA_Channel             = DMA_Channel_4;               
    DMA_InitStructure.DMA_PeripheralBaseAddr  = (uint32_t)(&(USART1->DR));   
    DMA_InitStructure.DMA_Memory0BaseAddr     = (uint32_t)transparent_tx_stru.buffer;             
    DMA_InitStructure.DMA_DIR                 = DMA_DIR_MemoryToPeripheral;    
    DMA_InitStructure.DMA_BufferSize          = 512;                                  
    DMA_InitStructure.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    
    DMA_InitStructure.DMA_MemoryInc           = DMA_MemoryInc_Enable;         
    DMA_InitStructure.DMA_PeripheralDataSize  = DMA_MemoryDataSize_Byte;      
    DMA_InitStructure.DMA_MemoryDataSize      = DMA_MemoryDataSize_Byte;     
    DMA_InitStructure.DMA_Mode                = DMA_Mode_Normal;             
    DMA_InitStructure.DMA_Priority            = DMA_Priority_Medium;	//DMA_Priority_High;             
    DMA_InitStructure.DMA_FIFOMode            = DMA_FIFOMode_Disable;          
    DMA_InitStructure.DMA_FIFOThreshold       = DMA_FIFOThreshold_HalfFull; 
    DMA_InitStructure.DMA_MemoryBurst         = DMA_MemoryBurst_Single;       
    DMA_InitStructure.DMA_PeripheralBurst     = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	//使能DMA中断
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
	//使能串口的 DMA发送接口
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	//配置DMA中断优先级
	NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Stream7_IRQn;           
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;          
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(DMA2_Stream7, DISABLE);	//发送时打开
}

/*******************************************************************************
* Function Name  : ec20_send_hex
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-10-18
* author		 : WangD 
*******************************************************************************/
void ec20_send_hex(u8 *data, u16 len)
{
	u16 i = 0;
	for(i = 0; i < len ;i++)
	{
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
		USART1->DR = data[i];    	
	}
}


/*******************************************************************************
* Function Name  : usart1_irq
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* date			 : 2019-9-2
* author		 : WangD 
*******************************************************************************/

void usart1_irq(void)                	//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = (u8)((USART1->DR) & 0x01ff);	//读取接收到的数据
		usart_4g.p_buf[usart_4g.count++] = Res;
		if(usart_4g.count >= EC20_RXBUF_SIZE)
		{
			usart_4g.count = 0 ;
		}
	} 
	
} 


#if 0
void DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)  
{  
   
    DMA_Cmd(DMA_Streamx, DISABLE);  
	DMA_ClearFlag(DMA_Streamx, DMA_IT_TCIF7);
      
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}    
          
    DMA_SetCurrDataCounter(DMA_Streamx,ndtr);             
   
    DMA_Cmd(DMA_Streamx, ENABLE);                      
}   

void DmaSendDataProc_IRQ(DMA_Stream_TypeDef *DMA_Streamx,uint32_t txbuff_addr ,u16 ndtr)  
{  
   
    DMA_Cmd(DMA_Streamx, DISABLE);  
	DMA_ClearFlag(DMA_Streamx, DMA_IT_TCIF7);
      
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}    
    DMA_MemoryTargetConfig(DMA_Streamx, (uint32_t)txbuff_addr,DMA_Memory_0);     
    DMA_SetCurrDataCounter(DMA_Streamx,ndtr);             
   
    DMA_Cmd(DMA_Streamx, ENABLE);                      
}   

void usart1_dma_send(u8* pdata, u16 len)
{
	u16 i;
	//if(pdata == NULL || len == 0)
		//return ;
	
	while( transparent_tx_stru.tc_flag )
	{
		//if(i++ > 10000)
			//break;
	}
	
	transparent_tx_stru.tc_flag  = 1;	
	memcpy(transparent_tx_stru.buffer, pdata, len);
	//transparent_tx_stru.pbuff = transparent_tx_stru.buffer;
	
	DmaSendDataProc(DMA2_Stream7,len);
}


void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET)   
	{
		DMA_ClearFlag(DMA2_Stream7,DMA_IT_TCIF7);
		DMA_Cmd(DMA2_Stream7,DISABLE);
		transparent_tx_stru.tc_flag  = 0;
	}
}


 

void usartx_tx_irq(USART_TypeDef* USARTx,usart_IrqTx_type* tx_type,u8* data,u16 len)
{
	
	while( tx_type->tc_flag == 1);
	tx_type->tc_flag  = 1;	
	memcpy(tx_type->buffer, data, len);
	tx_type->pbuff = tx_type->buffer;
	tx_type->txlen = len;
	tx_type->index = 1;
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_ITConfig(USARTx, USART_IT_TC , ENABLE);
	USART_SendData(USARTx, *(tx_type->pbuff++) ); 
	
}

#endif


