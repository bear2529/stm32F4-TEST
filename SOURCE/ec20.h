//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////





#ifndef __EC20_H
#define __EC20_H

/* Define to prevent recursive inclusion -------------------------------------*/


/* Define    -----------------------------------------------------------------*/
#define EC20_TCP_BUF_SIZE 1460	
#define EC20_POWERON (GPIOE->BSRRH = GPIO_Pin_0)	//RESET PE0
#define EC20_POWEROFF (GPIOE->BSRRL = GPIO_Pin_0)	//SET PE0
#define EC20_HEART_PRIOD	600		//60s
#define EC20_ServerLink_Priod	100		//30s

#define UART1_debug 0	//0 ：用于ec20， 1： 用于透传

/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
extern volatile flag_type is_upstream_buf1_full;	//usart6
extern volatile flag_type is_upstream_buf2_full;
extern volatile flag_type is_downstream_buf1_full;	//uart4
extern volatile flag_type is_downstream_buf2_full;

extern u16 EarthStation_ID;
extern u8 EC20_pack_sn;

extern u8 server_id;
extern state_4g_type state_4g;

/* Declare Golbal function   -------------------------------------------------*/
extern void ec20_loop(void);


#endif //__EC20_H


