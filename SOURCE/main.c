//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
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
//版本号
const u32 Version = 0x0100;	//V1.00
/**********************************************************************
* V1.00
		author ： wangd
		2019-8-30
		create the projcet 。






**********************************************************************/



int main(void)
{ 
	
	//EXIT_PD3_Config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//设置系统中断优先级分组4
	delay_init(168);								//初始化延时函数
	//TIM3_Init(1000 - 1,8400 -1);	//0.1s
	GPIO_Configuration();
	
	__disable_irq() ; 
	delay_ms(200);
	__enable_irq() ; 
    	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}





