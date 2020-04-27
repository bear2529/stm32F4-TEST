//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


/* Define to prevent recursive inclusion -------------------------------------*/
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "init.h"
#include "delay.h"

/* Define    -----------------------------------------------------------------*/


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* Declare Golbal function   -------------------------------------------------*/


/*  function   ---------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : led  
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-11-20
* author		 : WangD
*******************************************************************************/
void led_red_on(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_4);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}

void led_green_on(void)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_5);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
}

void len_off(void)
{
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	GPIO_SetBits(GPIOE,GPIO_Pin_4);
}

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );
	
	//RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE );
	
//	GPIO_DeInit(GPIOA);	//怕从bootloader跳过来引起意外
//	//AD3 : PA3
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);	
	//GPIO_DeInit(GPIOD);
	//N920_RX_SEL : PD10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);         
	GPIO_SetBits(GPIOD,GPIO_Pin_10); //
 
	
	

	//GPIOF9,F10³õÊ¼»¯ÉèÖÃ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//LED0ºÍLED1¶ÔÓ¦IO¿Ú
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//ÆÕÍ¨Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//ÉÏÀ­
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//³õÊ¼»¯GPIO
	GPIO_SetBits(GPIOD,GPIO_Pin_3);			//GPIOF9,F10ÉèÖÃ¸ß£¬µÆÃð

	//RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE );	
	//GPIO_DeInit(GPIOE);
	//LED :PE4, PE5
	//EC20_PW_ON : PE0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	GPIO_SetBits(GPIOE,GPIO_Pin_0); //
	
	led_red_on();
	//delay_ms(1);

	/* LED BAT : PC15,PC14,PC13,PE6 	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
}


/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void EXIT_PD3_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
   
   //Exit line gpio config
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	//输入
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
   GPIO_Init(GPIOD, &GPIO_InitStructure);
   RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE );
   
   //Exit line mode config
   EXTI_ClearITPendingBit(EXTI_Line0);
   //GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
   
   EXTI_InitStructure.EXTI_Line = EXTI_Line0;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);
}



/*******************************************************************************
* Function Name  : LED_BAT_Control
* Description    : 
* Input          : enum 
* Output         : None
* Return         : None
* date			 : 2020-3-17
* author		 : WangD
*******************************************************************************/
void led_bat_control(led_bat_stat_t lb_state)
{
	switch (lb_state)
	{
	    case lb_off :
	        LED_BAT1_OFF;
	        LED_BAT2_OFF;
	        LED_BAT3_OFF;
	        LED_BAT4_OFF;
	        break;
	    case lb_on4 :
	        LED_BAT1_ON;
	        LED_BAT2_ON;
	        LED_BAT3_ON;
	        LED_BAT4_ON;
	        break;
	    case lb_on3 :
	        LED_BAT1_OFF;
	        LED_BAT2_ON;
	        LED_BAT3_ON;
	        LED_BAT4_ON;
	        break;
	    case lb_on2 :
	        LED_BAT1_OFF;
	        LED_BAT2_OFF;
	        LED_BAT3_ON;
	        LED_BAT4_ON;
	        break;
	    case lb_on1 :
	        LED_BAT1_OFF;
	        LED_BAT2_OFF;
	        LED_BAT3_OFF;
	        LED_BAT4_ON;
	        break;
	    case lb_flicker:
	    	
	        LED_BAT1_ON;
	        LED_BAT2_ON;
	        LED_BAT3_ON;
	        LED_BAT4_ON;
	        delay_ms(300);
			LED_BAT1_OFF;
	        LED_BAT2_OFF;
	        LED_BAT3_OFF;
	        LED_BAT4_OFF;
	        delay_ms(300);
	        LED_BAT1_ON;
	        LED_BAT2_ON;
	        LED_BAT3_ON;
	        LED_BAT4_ON;
	        delay_ms(300);
	        LED_BAT1_OFF;
	        LED_BAT2_OFF;
	        LED_BAT3_OFF;
	        LED_BAT4_OFF;
	    	break;
	    default:
	        break;
	}
}



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
#if 0
void RCC_Configuration(void)
{   
  ErrorStatus HSEStartUpStatus; 
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(ENABLE);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  /**********all used Periphial clock must be enabled here**********************/
  // Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks 
   
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB |
							RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD |RCC_APB2Periph_GPIOE|RCC_APB2Periph_ADC1 | 
							  RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE ); //使能ADC1通道时钟，各个管脚时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC最大时间不能超过14M
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA传输
  
  // Enable USART1 clock
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  //Enable TIM1 clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,   ENABLE);
  
  
  /* CAN Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN|RCC_APB1Periph_USART2|RCC_APB1Periph_USART3, ENABLE);//使能CAN、USART2、USART3时钟
  
  
  
}

#endif

