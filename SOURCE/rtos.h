//////////////////////////////////////////////////////////////////////////////////	 
//Techx 2019-8-30
//author : WangD
//
//Copyright(C) ������֪�����Ƽ��������ι�˾ 2018-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////


/* Define to prevent recursive inclusion -------------------------------------*/


/* Define    -----------------------------------------------------------------*/
#define START_TASK_PRIO		1					//�������ȼ�
#define START_STK_SIZE 		128  			//�����ջ��С	


/* Define Golbal variable  in const ------------------------------------------*/



/* Define Golbal variable  in sdram ------------------------------------------*/



/* Declare Golbal function   -------------------------------------------------*/
extern TaskHandle_t StartTask_Handler;

//start������
extern void start_task(void *pvParameters);
//������
extern void ec20_task(void *pvParameters);
extern void ec20_task(void *pvParameters);



