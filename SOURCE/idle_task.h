//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef __IDLE_TASK_H
#define __IDLE_TASK_H


/* Define to prevent recursive inclusion -------------------------------------*/
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "type.h"


/* Define    -----------------------------------------------------------------*/



/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
extern tim_typedef adcTime;



/* Declare Golbal function   -------------------------------------------------*/
extern void idle_loop(void);




#endif


