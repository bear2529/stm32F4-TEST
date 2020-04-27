//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __EC20_USART_H
#define __EC20_USART_H


/* Define to prevent recursive inclusion -------------------------------------*/
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "stm32f4xx_dma.h"
#include "type.h"


/* Define    -----------------------------------------------------------------*/
#define EC20_RXBUF_SIZE				256
#define get_ec20_usart_flag(uflag) USART_GetFlagStatus(USART1, uflag)


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
extern usart_4g_rx_type usart_4g;



/* Declare Golbal function   -------------------------------------------------*/
extern void usart1_init(u32 baud);
extern void usart1_irq(void);
extern void ec20_send_hex(u8 *data, u16 len);
extern u16 change_1byte_to_2ascii(u8 byte);
extern void changeTAB_bytes_to_ascii(const u8 * bytes_Tab, u8 * ascii_Tab, u8 len);
extern void changeTAB_bytes_to_ascii_nohead(const u8 * bytes_Tab, u8 * ascii_Tab, u8 len);

extern void EC20_USART_Config_Baud(u32 baud);



#endif


