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
#include "init.h"
#include "time.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_adc.h"


/* Define    -----------------------------------------------------------------*/
#define ADC_SAMPLE_PRIOD	1200		//120s



/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/
tim_typedef adcTime;
u16 batteryV = 0;


/* Declare Golbal function   -------------------------------------------------*/




/*  - function      -----------------------------------------------------------*/
/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : ����ADC
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-10
* author		 : WangD
*******************************************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );

	//GPIO  PA3	AD3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADC1ʱ��
	ADC_DeInit();

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;	//����ADC��Ƶ��ΪAPB2/2 ��84/2= 42M
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;	//DMAʧ��
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//��ɨ��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None; //�ⲿ����ת���ر�
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ENABLE; //ģ��ת������������ת��ģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	//ADC_InitStructure.ADC_NbrOfChannel = 1; //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
	ADC_TempSensorVrefintCmd(ENABLE);	//�����ڲ��¶ȴ�����

	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,1, ADC_SampleTime_144Cycles);/*���ù���ͨ��3 һ������ ����ʱ�� */
	
	ADC_Cmd(ADC1, ENABLE); //ʹ��ָ����ADC1
}


/*******************************************************************************
* Function Name  : getADC
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-10
* author		 : WangD
*******************************************************************************/
u16 getADC(void)
{
	ADC_SoftwareStartConv(ADC1);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}


u16 getADC_avg(u8 num)
{
	u16 sum = 0;
	u8 i ;
	for ( i = 0; i < num ; i++ )
	{
	    sum += getADC();
	}
	return (sum / num);
}

u16 getBatteryV(void)
{
	u32 value = getADC_avg(16);
	u16 V = (u16)((value * 3000) / 4096);	//mV	
	return (V * 5);
}

/*******************************************************************************
* Function Name  : show_battery
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* date			 : 2020-3-17
* author		 : WangD
*******************************************************************************/
void show_battery(u16 batV)
{
	u16 Vcell = batV / 3;	//3��
	
	if(Vcell > 3830)
	{
	    led_bat_control(lb_on4);
	}
	else if ( Vcell > 3700 )
	{
	    led_bat_control(lb_on3);
	}
	else if ( Vcell > 3600 )
	{
	    led_bat_control(lb_on2);
	}
	else  
	{
	    led_bat_control(lb_on1);
	}
}




/*******************************************************************************
* Function Name  : idle_loop
* Description    : idle����ѭ��
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-10
* author		 : WangD
*******************************************************************************/
void idle_loop(void)
{
	ADC_Configuration();
	TIM3_Init(1000 - 1, 8400 - 1);
	adcTime.count = ADC_SAMPLE_PRIOD + tim3_count;
	adcTime.flag = true;

	led_bat_control(lb_flicker);
	usart3_SendStr("idle task run...\r\n");
	
	while(1)
	{
		if(adcTime.flag == true)
		{	
			adcTime.flag = false;
			adcTime.count = tim3_count + ADC_SAMPLE_PRIOD;
			batteryV = getBatteryV();
			show_battery(batteryV);
		}
		vTaskDelay( 500 );
	}
}







