//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rtos.h"
#include "init.h"


/* Define Golbal variable  in const -------------------------------------*/
//�汾��
const u32 Version = 0x0100;	//V1.00
/**********************************************************************
* V1.00
		author �� wangd
		2019-8-30
		create the projcet ��






**********************************************************************/



int main(void)
{ 
	
	//EXIT_PD3_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//����ϵͳ�ж����ȼ�����4
	delay_init(168);								//��ʼ����ʱ����
	//TIM3_Init(1000 - 1,8400 -1);	//0.1s
	GPIO_Configuration();
	
	__disable_irq() ; 
	delay_ms(200);
	__enable_irq() ; 
    	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}





