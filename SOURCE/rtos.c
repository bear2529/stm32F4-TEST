//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


/* Define to prevent recursive inclusion -------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "rtos.h"
#include "usart.h"
#include "ec20.h"
#include "idle_task.h"

/* Define    -----------------------------------------------------------------*/

//4g task
#define EC20_TASK_PRIO		2				//�������ȼ�
#define EC20_STK_SIZE 		256  			//�����ջ��С	

// usart task
#define TRANSPARENT_TASK_PRIO		2		//�������ȼ�
#define TRANSPARENT_STK_SIZE 		128  	//�����ջ��С

// idle task
#define IDLE_TASK_PRIO		2		//�������ȼ�
#define IDLE_STK_SIZE 		128  	//�����ջ��С


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* Declare Golbal function   -------------------------------------------------*/

//������
TaskHandle_t StartTask_Handler;
TaskHandle_t EC20Task_Handler;
TaskHandle_t TransparentTask_Handler;
TaskHandle_t IdleTask_Handler;


//������
void ec20_task(void *pvParameters);
void Transparent_task(void *pvParameters);
void Idle_task(void *pvParameters);


/*  function   ------------------------------------------------------------*/


//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����TASK1����
	
    xTaskCreate((TaskFunction_t )ec20_task,             
                (const char*    )"ec20_task",           
                (uint16_t       )EC20_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EC20_TASK_PRIO,        
                (TaskHandle_t*  )&EC20Task_Handler);   
				
	
	xTaskCreate((TaskFunction_t )Transparent_task,             
                (const char*    )"Transparent_task",           
                (uint16_t       )TRANSPARENT_STK_SIZE,       
                (void*          )NULL,                  
                (UBaseType_t    )TRANSPARENT_TASK_PRIO,        
                (TaskHandle_t*  )&TransparentTask_Handler); 
				

	xTaskCreate((TaskFunction_t )Idle_task,             
                (const char*    )"Idle_task",           
                (uint16_t       )IDLE_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )IDLE_TASK_PRIO,        
                (TaskHandle_t*  )&IdleTask_Handler);
	
	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


void ec20_task(void *pvParameters)
{
	ec20_loop();
}



//Transparent ������
void Transparent_task(void *pvParameters)
{
	transparent_loop();
}

void Idle_task(void *pvParameters)
{
	idle_loop();
}


