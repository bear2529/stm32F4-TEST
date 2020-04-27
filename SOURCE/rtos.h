//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) 北京德知航创科技有限责任公司 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


/* Define to prevent recursive inclusion -------------------------------------*/


/* Define    -----------------------------------------------------------------*/
#define START_TASK_PRIO		1					//任务优先级
#define START_STK_SIZE 		128  			//任务堆栈大小	


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* Declare Golbal function   -------------------------------------------------*/
extern TaskHandle_t StartTask_Handler;

//start任务函数
extern void start_task(void *pvParameters);
//任务函数
extern void ec20_task(void *pvParameters);
extern void ec20_task(void *pvParameters);



