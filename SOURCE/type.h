//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __TYPE_H
#define __TYPE_H

/* Define to prevent recursive inclusion -------------------------------------*/


/* Define    -----------------------------------------------------------------*/


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* COMMOM  -------------------------------------------------*/
typedef enum
{
	false = 0,
	true
}flag_type;

typedef enum
{
	closed = 0,
	open
}State_OC_type;



/* USART   -------------------------------------------------*/


typedef struct{
	u8 *p_buf1;
	u8 *p_buf2;
	u8 *p_tx;
	u16 bufsize;
	u8 dma_buf_used;	//0 : buf1, 1:buf2
	flag_type buf1_full_flag;
	flag_type buf2_full_flag;
}usart_dma_rx_type;

typedef struct {
	u8 *p_buf;
	u16 bufsize;
	u16 count;
}usart_4g_rx_type;


/* USART   -------------------------------------------------*/
typedef struct {
	u32 count;
	flag_type flag;
}tim_typedef;



/*4g   -------------------------------------------------*/
typedef struct{
	flag_type linked;
	State_OC_type server;
	u8 faultCnt;
	u8 SerHeartCnt;
}state_4g_type;




#endif //__TYPE_H

