//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __TIME_H
#define __TIME_H


/* Define to prevent recursive inclusion -------------------------------------*/
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "type.h"


/* Define    -----------------------------------------------------------------*/



/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
extern u32 tim3_count ;
extern u32 tim3_count ;
extern tim_typedef heartTime;
extern tim_typedef serverLinkTime;

/* Declare Golbal function   -------------------------------------------------*/
extern void TIM3_Init(u16 arr,u16 psc);
extern void tim3_irq(void);



#endif

