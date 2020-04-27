//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#ifndef _INIT_H
#define _INIT_H

/* Define to prevent recursive inclusion -------------------------------------*/
typedef enum
{
	lb_off = 0,
	lb_flicker,
	lb_on4,
	lb_on3,
	lb_on2,
	lb_on1
	
}led_bat_stat_t;

/* Define    -----------------------------------------------------------------*/
#define LED_WORK_OFF 	(GPIOE->BSRRH = GPIO_Pin_4)		//RESET 
#define LED_WORK_ON 	(GPIOE->BSRRL = GPIO_Pin_4)		//SET 
#define LED_CAP_OFF 	(GPIOE->BSRRH = GPIO_Pin_5)		//RESET 
#define LED_CAP_ON 		(GPIOE->BSRRL = GPIO_Pin_5)		//SET 

#define LED_BAT1_ON 	(GPIOC->BSRRH = GPIO_Pin_15)		
#define LED_BAT1_OFF 	(GPIOC->BSRRL = GPIO_Pin_15)		
#define LED_BAT2_ON 	(GPIOC->BSRRH = GPIO_Pin_14)		
#define LED_BAT2_OFF 	(GPIOC->BSRRL = GPIO_Pin_14)		
#define LED_BAT3_ON 	(GPIOC->BSRRH = GPIO_Pin_13)		
#define LED_BAT3_OFF 	(GPIOC->BSRRL = GPIO_Pin_13)		
#define LED_BAT4_ON 	(GPIOE->BSRRH = GPIO_Pin_6)		
#define LED_BAT4_OFF 	(GPIOE->BSRRL = GPIO_Pin_6)		



/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* Declare Golbal function   -------------------------------------------------*/
extern void EXIT_PD3_Config(void);
extern void GPIO_Configuration(void);
extern void led_red_on(void);
extern void led_green_on(void);
extern void len_off(void);

extern void led_bat_control(led_bat_stat_t lb_state);


#endif	//_INIT_H

