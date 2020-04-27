//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////



/* Define to prevent recursive inclusion -------------------------------------*/
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#include "type.h"
#include "time.h"
#include "idle_task.h"

/* Define    -----------------------------------------------------------------*/




/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
u32 tim3_count = 0;
tim_typedef heartTime;
tim_typedef serverLinkTime;


/* Declare Golbal function   -------------------------------------------------*/




/*  - function      -----------------------------------------------------------*/


// ------------------------------   TIM3  -----------------------------------------------------------------//

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数.
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
// tim3 使用 APB1, APB1=42M, tim时钟被2倍频=84M
// arr = 1000 -1 ; psc = 8400 - 1; 0.1s进一次
//这里使用的是定时器3!
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04; //抢占优先级4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器3中断服务函数
void tim3_irq(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET) //溢出中断
	{
		tim3_count++;
		if(tim3_count == heartTime.count)
			heartTime.flag = true;

		if(tim3_count == adcTime.count)
			adcTime.flag = true;
		
		if(tim3_count == serverLinkTime.count)
			serverLinkTime.flag = true;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

