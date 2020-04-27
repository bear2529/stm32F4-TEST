//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
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

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��.
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
// tim3 ʹ�� APB1, APB1=42M, timʱ�ӱ�2��Ƶ=84M
// arr = 1000 -1 ; psc = 8400 - 1; 0.1s��һ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);	//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04; //��ռ���ȼ�4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��3�жϷ�����
void tim3_irq(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET) //����ж�
	{
		tim3_count++;
		if(tim3_count == heartTime.count)
			heartTime.flag = true;

		if(tim3_count == adcTime.count)
			adcTime.flag = true;
		
		if(tim3_count == serverLinkTime.count)
			serverLinkTime.flag = true;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}

