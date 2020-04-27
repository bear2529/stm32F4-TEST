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
#include "usart.h"
#include "EC20.h"
#include "stdlib.h"
#include "string.h"
#include "usart.h"	
#include "sys.h"
#include "ec20_usart.h"
#include "delay.h"
#include "time.h"
#include "init.h"

/* Define    -----------------------------------------------------------------*/


#define PACK_HEAD_1		'^'
#define PACK_HEAD_2		'&'
#define PACK_END_1		'&'
#define PACK_END_2		'^'

#define EARTH_STATION_ID 		"TECHX-ES-001"			//12B
#define PACK_EC20_SIZE	60

#define EC20_MODE_PACK 				1	//����ģʽ�� �ְ�
#define EC20_MODE_TRANSPARENT 		2	//͸��
#define TRANSPORT_DIR_UP			3	//���䷽������
#define TRANSPORT_DIR_DOWN			4

#define _SERVER_ID	2	//0: ���Լ��ģ� 1�� �����ʱ 	2�� ������


/* Define Golbal variable  in const ------------------------------------------*/
u8 const ser_ip1_const[] = "AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9999,0,1\r\n";
u8 const ser_ip2_const[] = "AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9090,0,1\r\n";
u8 const ser_ip3_const[] = "AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.182\042,9090,0,1\r\n";



/* Define Golbal variable  in sdram ------------------------------------------*/
volatile flag_type is_upstream_buf1_full = false;
volatile flag_type is_upstream_buf2_full = false;
volatile flag_type is_downstream_buf1_full = false;
volatile flag_type is_downstream_buf2_full = false;

u8 Pack_start[PACK_EC20_SIZE];
u8 Pack_heart[PACK_EC20_SIZE];
//u8 Pack_cmd_up[PACK_EC20_SIZE];
//u8 Pack_cmd_down[PACK_EC20_SIZE];

u16 EarthStation_ID = 0x1235;	//0x1234;
u8 EC20_pack_sn = 0;
u8 EC20_packup_sn = 0;
u8 EC20_packdown_sn = 0;

state_4g_type state_4g;

u8 server_id = _SERVER_ID;

/* Declare Golbal function   -------------------------------------------------*/



/*  - function      -----------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Enter_ec20_Transparent
* Description    : 4g����ѭ��
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void Enter_ec20_Transparent()
{
	printf("AT+QISWTMD=0,2\r\n");
    delay_ms(500);
}

void Exit_ec20_Transparent()
{
	printf("+++");//�˳�͸��
	delay_ms(1000);
}


/*******************************************************************************
* Function Name  : ec20_loop
* Description    : 4g����ѭ��
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void Clear_Buffer(u8 *buf,u16 *count)//���4g�� rx����
{
    u16 i;
    u16 len = *count;
    if(UART1_debug == 0) 
    	usart3_SendStr(buf);
    for(i = 0; i < len; i++)
		buf[i] = 0;
    *count = 0;
    //IWDG_Feed();//�0�2�0�1�0�1��
}

/*******************************************************************************
* Function Name  : EC20_Init_Transparent
* Description    : ec20��ʼ����͸��ģʽ
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/

void  EC20_Init_Transparent()
{
	char *strx,*extstrx;
	u8 const *ser_p ;
	char buf1[100],buf2[100];
	u16 i = 0;
	strx = buf1;
	extstrx = buf2;
    printf("AT\r\n"); 
    usart3_SendStr("AT...\r\n");
    delay_ms(500);
    strx = strstr((const char*)usart_4g.p_buf,(const char*)"OK");//return OK
  	while(strx==NULL)
    {
        Clear_Buffer(usart_4g.p_buf, &usart_4g.count);	
        printf("AT\r\n"); 
		//usart3_SendStr("AT...\r\n");
        delay_ms(500);
        strx = strstr((const char*)usart_4g.p_buf,(const char*)"OK");
    }
    printf("ATE0\r\n"); //�رջ���
    usart3_SendStr("ATE0...\r\n");
    delay_ms(500);
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);	
	printf("AT+IPR?\r\n");
	usart3_SendStr("AT+IPR?...\r\n");
	delay_ms(500);
	Clear_Buffer(usart_4g.p_buf, &usart_4g.count);	
    printf("AT+CSQ\r\n"); //���CSQ
    delay_ms(500);
    printf("ATI\r\n"); //���ģ��İ汾��
    delay_ms(500);
    /////////////////////////////////
    printf("AT+CPIN?\r\n");//���sim���Ƿ���λ��
    delay_ms(500);
    strx = strstr((const char*)usart_4g.p_buf,(const char*)"+CPIN: READY");//�鿴�Ƿ񷵻�ready
  	while(strx == NULL)
    {
        Clear_Buffer(usart_4g.p_buf, &usart_4g.count);	
        printf("AT+CPIN?\r\n");
        delay_ms(500);
        strx = strstr((const char*)usart_4g.p_buf,(const char*)"+CPIN: READY");//���sim�����ʶ�𲻵�����ʣ��Ĺ�����û������
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    ///////////////////////////////////
    printf("AT+CREG?\r\n");//�鿴�Ƿ�ע��Gsm����
    delay_ms(500);
    strx = strstr((const char*)usart_4g.p_buf,(const char*)"+CREG: 0,1");//��������
    extstrx = strstr((const char*)usart_4g.p_buf,(const char*)"+CREG: 0,5");//��������������
  	while( strx == NULL && extstrx == NULL)
    {
        Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
        printf("AT+CREG?\r\n");//�鿴�Ƿ�ע��Gsm����
        delay_ms(500);
        strx = strstr((const char*)usart_4g.p_buf,(const char*)"+CREG: 0,1");
        extstrx = strstr((const char*)usart_4g.p_buf,(const char*)"+CREG: 0,5");
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    /////////////////////////////////////
    printf("AT+CGREG?\r\n");//�鿴�Ƿ�ע��GPRS����
    delay_ms(500);
    strx = strstr((const char*)usart_4g.p_buf, (const char*)"+CGREG: 0,1");
    extstrx = strstr((const char*)usart_4g.p_buf, (const char*)"+CGREG: 0,5");
  	while(strx==NULL && extstrx==NULL)
    {
        Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
        printf("AT+CGREG?\r\n");
        delay_ms(500);
        strx = strstr((const char*)usart_4g.p_buf,(const char*)"+CGREG: 0,1");
        extstrx = strstr((const char*)usart_4g.p_buf,(const char*)"+CGREG: 0,5");
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+COPS?\r\n");//�鿴��Ӫ��
    delay_ms(500);
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+QICLOSE=0\r\n");//�ر�socket����
    usart3_SendStr("AT+QICLOSE=0\r\n");
    delay_ms(500);
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
	//�ƶ��� apn
    printf("AT+QICSGP=1,1,\042CMNET\042,\042\042,\042\042,0\r\n");//����APN,���û�������
	//���� apn
	//printf("AT+QICSGP=1,1,\042CTLTE\042,\042\042,\042\042,0\r\n");//����APN,���û�������
    delay_ms(1000);
    strx = strstr((const char*)usart_4g.p_buf,(const char*)"OK");
  	while(strx == NULL)
    {
        delay_ms(500);
        strx=strstr((const char*)usart_4g.p_buf,(const char*)"OK");
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+QIDEACT=1\r\n");//ȥ����
    usart3_SendStr("AT+QIDEACT=1\r\n");
    delay_ms(500);
    strx = strstr((const char*)usart_4g.p_buf,(const char*)"OK");
	while(strx == NULL)
    {
        delay_ms(500);
        strx = strstr((const char*)usart_4g.p_buf,(const char*)"OK");
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+QIACT=1\r\n");//����
    usart3_SendStr("AT+QIACT=1\r\n");
    delay_ms(500);
    strx=strstr((const char*)usart_4g.p_buf,(const char*)"OK");
	while(strx == NULL)
    {
        delay_ms(500);
        strx=strstr((const char*)usart_4g.p_buf,(const char*)"OK");
    }
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+QIACT?\r\n");//��ȡ��ǰ����ip��ַ
    usart3_SendStr("AT+QIACT?\r\n");
    delay_ms(500);
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);

    #if 0 
	if(SERVER_ID == 0)	//bear �����	
		printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9999,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
	else if(SERVER_ID == 1)//��������
		printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
	else if(SERVER_ID == 2)//������
		printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.182\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
	/*
	//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9999,0,2\r\n");//��������Ҫ��¼��ip������ģʽ2��transparents
	//printf("AT+QIOPEN=1,0,\042TCP\042,\042bear2529.xicp.net\042,17470,0,2\r\n");
	*/
	
    delay_ms(500);
	
	
    strx=strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");//����Ƿ��¼�ɹ�
	while(strx==NULL)
    {
        strx=strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");
        delay_ms(100);
    }

    delay_ms(500);
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
	#endif
	
	state_4g.linked = false;
    while(state_4g.linked != true)
	{
		printf("AT+QICLOSE=0\r\n");//�ر�socket����
		usart3_SendStr("AT+QICLOSE=0\r\n");
		delay_ms(1000);
		Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		
		if(server_id == 0)	//bear �����	
			ser_p = ser_ip1_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9999,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
		else if(server_id == 1)//��������
			ser_p = ser_ip2_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
		else if(server_id == 2)//������
			ser_p = ser_ip3_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.182\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode

		printf((char *)ser_p);
		usart3_SendStr((u8 *)ser_p);
		
		strx = strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");//����Ƿ��¼�ɹ�
		i = 0;
		while((strx == NULL) && (i < 300))
		{
			strx = strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");
			delay_ms(100);
			i++;
			//Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		}
		Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		if(i < 300)
			state_4g.linked = true;
	}
}		

/*******************************************************************************
* Function Name  : EC20_ReLink_Transparent
* Description    : ec20����������
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-10-16
* author		 : WangD
*******************************************************************************/
void EC20_ReLink_Transparent(void)
{
	char buf1[100];
	char *strx = buf1;
	u8 const *ser_p ;
	u16 i = 0;
	Exit_ec20_Transparent();
	delay_ms(500);
	while(state_4g.linked != true)
	{
		printf("AT+QICLOSE=0\r\n");//�ر�socket����
		usart3_SendStr("AT+QICLOSE=0\r\n");
		delay_ms(2000);
		Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		
		if(server_id == 0)	//bear �����	
			ser_p = ser_ip1_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,9999,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
		else if(server_id == 1)//��������
			ser_p = ser_ip2_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.179\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode 
		else if(server_id == 2)//������
			ser_p = ser_ip3_const;
			//printf("AT+QIOPEN=1,0,\042TCP\042,\042182.48.105.182\042,8888,0,1\r\n");//��������Ҫ��¼��ip������ģʽ1��Direct push mode

		printf((char *)ser_p);
		usart3_SendStr((u8 *)ser_p);
		
		strx = strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");//����Ƿ��¼�ɹ�
		i = 0;
		while((strx == NULL) && (i < 300))
		{
			strx = strstr((const char*)usart_4g.p_buf,(const char*)"+QIOPEN: 0,0");
			delay_ms(100);
			i++;
			//Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		}
		Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		if(i < 300)
			state_4g.linked = true;
	}
	
	//state_4g.linked = true;
	Enter_ec20_Transparent();
}

/*******************************************************************************
* Function Name  : EC20_Send_Str
* Description    : ec20����string��ʹ��atָ��
* Input          : string data buf
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/

void EC20_Send_Str(u8 *bufferdata)
{
	//u8 untildata=0xff;
	u8 tmp[50];
	char *strx;
	memset(tmp,0,50);
	//sprintf((char *)tmp,"AT+QISEND=0,%d\r\n",EC20_TCP_BUF_SIZE);
	printf("AT+QISEND=0\r\n");
	//printf((const char *)tmp);
	delay_ms(20);	//100
	printf((const char *)bufferdata);
    delay_ms(20);	//100
    USART_SendData(USART1, (u8) 0x1a);//���0�4�0�9�0�1�0�1���0�6�0�7�0�2�0�4�0�8�0�5
  	while(get_ec20_usart_flag(USART_FLAG_TC) == RESET)
    {
    }
	delay_ms(20);	//100
    strx = strstr((char*)usart_4g.p_buf,(char*)"SEND OK");//�Ƿ���ȷ
  	while(strx == NULL)
    {
        strx = strstr((char*)usart_4g.p_buf,(char*)"SEND OK");//�Ƿ���ȷ
        delay_ms(2);
    }
    delay_ms(2);//100
    Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    printf("AT+QISEND=0,0\r\n");
    delay_ms(2);	//200
    strx = strstr((char*)usart_4g.p_buf,(char*)"+QISEND:");
	
	Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
}

/*******************************************************************************
* Function Name  : EC20Send_hexData_Transparent
* Description    : ��͸��ģʽ�£���������
* Input          : string data buf
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
u16 tmpcrc = 0;
void EC20Send_hexData_Transparent(u8* hexdata, u16 len)
{
	//��һ���ֽڲ�ֳ�2���ֽ�ascii�뷢��
	u8 asc1 = 0,crc1,crc2;
	u16 i = 0;
	tmpcrc = 0;
	for(i = 0; i < len; i ++)
	{
		asc1 = ((hexdata[i] >> 4) < 0x0a) ? ((hexdata[i] >> 4) + 0x30) : ((hexdata[i] >> 4) + 0x37);
		while((USART1->SR&0X40)==0);
		USART1->DR = (u8) asc1;
		asc1 = ((hexdata[i] & 0x0f) < 0x0a) ? ((hexdata[i] & 0x0f) + 0x30) : ((hexdata[i] & 0x0f) + 0x37);
		while((USART1->SR&0X40)==0);
		USART1->DR = (u8) asc1;
		tmpcrc += hexdata[i];
	}
	#if 0
	crc1 = (u8)(tmpcrc & 0x00ff);
	crc2 = (u8)(tmpcrc >> 8);
	//����һ���費��Ҫ�� 0d 0a
	asc1 = ((crc1 >> 4) < 0x0a) ? ((crc1 >> 4) + 0x30) : ((crc1 >> 4) + 0x37);
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) asc1;
	asc1 = ((crc1 & 0x0f) < 0x0a) ? ((crc1 & 0x0f) + 0x30) : ((crc1 & 0x0f) + 0x37);
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) asc1;
	asc1 = ((crc2 >> 4) < 0x0a) ? ((crc2 >> 4) + 0x30) : ((crc2 >> 4) + 0x37);
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) asc1;
	asc1 = ((crc2 & 0x0f) < 0x0a) ? ((crc2 & 0x0f) + 0x30) : ((crc2 & 0x0f) + 0x37);
	while((USART1->SR&0X40)==0);
	USART1->DR = (u8) asc1;
	#endif
}

void EC20Send_StrData_Transparent(u8 *bufferdata)
{
	printf((const char *)bufferdata);
	delay_ms(2);
}



// --------------------------------------   Pack  -----------------------------------------------------------------//

/*******************************************************************************
* Function Name  : create_start_pack
* Description    : ����start������˸�ʽ
* Input          : pack sn 
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
void create_start_pack(u8 packsn)
{
	u8 i = 0;
	u8 *pack = Pack_start;
	u16 sum = 0;
	pack[0] = 0xEB;
	pack[1] = 0x90;
	pack[2] = 0xAC;
	pack[3] = 0x0F;
	pack[4] = 26;		//С�˸�ʽ
	pack[5] = 00;
	pack[6] = packsn;
	pack[7] = 0x04;	//4g
	memcpy(&pack[8], EARTH_STATION_ID,12);
	pack[20] = (u8)( EarthStation_ID & 0x00ff);	//С�˸�ʽ
	pack[21] = (u8)((EarthStation_ID >> 8) & 0x00ff);
	pack[22] = 0x01;	//cmd : start pack
	pack[23] = (u8)EC20_MODE_TRANSPARENT;
	pack[24] = 0;
	pack[25] = 0;
	pack[26] = 0;
	pack[27] = 0;
	for(i = 4 ; i < 28 ; i++)
	{
		sum += pack[i];
	}
	
	pack[28] = (u8)(sum & 0x00ff);	//С�˸�ʽ
	pack[29] = (u8)(sum >> 8);
	pack[30] = 0xEA;
	pack[31] = 0x8F;
	pack[32] = 0xAA;
	pack[33] = 0x0D;
	

	//changeTAB_bytes_to_ascii((const u8 *) tmp, pack, 26);
}

/*******************************************************************************
* Function Name  : create_heart_pack
* Description    : ����heart������˸�ʽ
* Input          : string data buf
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
#if 0
void create_heart_pack(u8 packsn)
{
	u8 i = 0,tmp[20];
	u8 * pack = Pack_heart;
	u16 sum = 0;
	tmp[0] = 0x00;
	tmp[1] = 9;
	tmp[2] = packsn;
	tmp[3] = 0x04;	//4g
	tmp[4] = (u8)((EarthStation_ID >> 8) & 0x00ff);
	tmp[5] = (u8)( EarthStation_ID & 0x00ff);
	tmp[6] = 0x02;	//cmd : heart pack
	for(i = 0 ; i < 7 ; i++)
	{
		sum += tmp[i];
	}
	
	tmp[7] = (u8)(sum >> 8);
	tmp[8] = (u8)(sum & 0x00ff);
	
	changeTAB_bytes_to_ascii((const u8 *) tmp, pack, 9);
}
#endif
void create_heart_pack(u8 packsn)
{
	u8 i = 0;
	u8 * pack = Pack_heart;
	u16 sum = 0;
	pack[0] = 0xEB;
	pack[1] = 0x90;
	pack[2] = 0xAC;
	pack[3] = 0x0F;
	pack[4] = 9;
	pack[5] = 0x00;
	pack[6] = packsn;
	pack[7] = 0x04;	//4g
	pack[8] = (u8)( EarthStation_ID & 0x00ff);	//С�˸�ʽ
	pack[9] = (u8)((EarthStation_ID >> 8) & 0x00ff);
	pack[10] = 0x02;	//cmd : heart pack
	for(i = 4 ; i < 11 ; i++)
	{
		sum += pack[i];
	}
		
	pack[11] = (u8)(sum & 0x00ff);		//С�˸�ʽ
	pack[12] = (u8)(sum >> 8);
	pack[13] = 0xEA;
	pack[14] = 0x8F;
	pack[15] = 0xAA;
	pack[16] = 0x0D;
}
/*******************************************************************************
* Function Name  : create_tsCMD_pack
* Description    : ����͸���������ָ��up��down��ģʽ������
* Input          : string data buf
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
#if 0
void create_tsCMD_pack(u8 * pack,u8 mode,u8 direction,u16 hexDataLen)
{
	u8 i = 0,tmp[50];
	u16 sum = 0;
	tmp[0] = 0x00;
	tmp[1] = 0x07;
	tmp[2] = 0x03;
	tmp[3] = mode;
	tmp[4] = direction;
	tmp[5] = (u8)(hexDataLen >> 8);
	tmp[6] = (u8)(hexDataLen & 0x00ff);
	for(i = 0 ; i < 7 ; i++)
	{
		sum += tmp[i];
	}
	
	tmp[7] = (u8)(sum >> 8);
	tmp[8] = (u8)(sum & 0x00ff);
	
	changeTAB_bytes_to_ascii((const u8 *) tmp, pack, 9);
}
#endif

/*******************************************************************************
* Function Name  : EC20_PackSend_Data
* Description    : 
* Input          : direction: ������
					data : ͸������
					dataLen �� ͸�����ݳ���
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
#if 0
void EC20_PackSend_Data(u8 packsn,u8 direction, u8* data, u16 dataLen)
{
	u8 i = 0,tmp[20], str[40];
	tmp[0] = 0x00;
	tmp[1] = 14;
	tmp[2] = packsn;
	tmp[3] = 0x04;	//4g
	tmp[4] = (u8)((EarthStation_ID >> 8) & 0x00ff);
	tmp[5] = (u8)( EarthStation_ID & 0x00ff);
	tmp[6] = 0x03;	//���ݰ�
	tmp[7] = direction;
	tmp[8] = 0;
	tmp[9] = 0;
	tmp[10] = 0;
	tmp[11] = 0;
	tmp[12] = (u8)(dataLen >> 8);
	tmp[13] = (u8)(dataLen & 0x00ff);

	changeTAB_bytes_to_ascii_nohead(tmp, str, 14);

	printf("^&");
	printf(str);
	EC20Send_hexData_Transparent(data ,dataLen);
	printf("&^");
	printf("\r\n");
}
#endif

void EC20_PackSend_Data(u8 packsn,u8 direction, u8* data, u16 dataLen)
{
	u8 tmp[30];
	u16 i = 0,totalLen = 14+ dataLen + 2;	//ǰ��14�� + ���ݳ��� + crc����
	u16 sum = 0;
	tmp[0] = 0xEB;
	tmp[1] = 0x90;
	tmp[2] = 0xAC;
	tmp[3] = 0x0F;
	tmp[4] = (u8)( totalLen & 0x00ff);	//С�˸�ʽ
	tmp[5] = (u8)( totalLen >> 8 );	
	tmp[6] = packsn;
	tmp[7] = 0x04;	//4g
	tmp[8] = (u8)( EarthStation_ID & 0x00ff);//С�˸�ʽ
	tmp[9] = (u8)((EarthStation_ID >> 8) & 0x00ff);	
	tmp[10] = 0x03;	//���ݰ�
	tmp[11] = direction;
	tmp[12] = 0;
	tmp[13] = 0;
	tmp[14] = 0;
	tmp[15] = 0;
	tmp[16] = (u8)(dataLen & 0x00ff);//С�˸�ʽ
	tmp[17] = (u8)(dataLen >> 8);	
	
	for(i = 4 ; i < 18 ; i++)
	{
		sum += tmp[i];
	}
	
	ec20_send_hex(tmp, 18);
	
	for(i = 0 ; i < dataLen ; i++)
	{
		sum += data[i];
	}
	
	ec20_send_hex(data, dataLen);
	
	tmp[0] = (u8)(sum & 0x00ff);
	tmp[1] = (u8)(sum >> 8);	
	tmp[2] = 0xEA;
	tmp[3] = 0x8F;
	tmp[4] = 0xAA;
	tmp[5] = 0x0D;
	ec20_send_hex(tmp, 6);
}



/*******************************************************************************
* Function Name  : ec20_loop
* Description    : 4g����ѭ��
* Input          : None
* Output         : None
* Return         : None
* date			 : 2019-9-2
* author		 : WangD
*******************************************************************************/
int inValue = 0;
void ec20_loop(void)
{
	char *STR_Index;
	EC20_POWERON;
	state_4g.linked = false;
	usart1_init(115200);
	usart3_init(115200);
	delay_ms(1000);
	delay_ms(1000);
	usart3_SendStr("ec20 task run...\r\n");
	//portDISABLE_INTERRUPTS();	//ֹͣ�����л�
	usart3_SendStr("ec20 START...\r\n");
	
	printf("ITM test\r\n");
	printf("input value ...\r\n");
	//scanf("%d",&inValue);
	printf("value = %d\r\n",inValue);
	printf("ITM test END\r\n");
	
	while(1)
	{
		
	}
	/////��һ���ǿ���EC20�Ĳ����ʵ�230400���Ƶ���û�����ѡ���û��߲���,���ÿ������ε�
	#if 1
	printf("ATE0\r\n");
	delay_ms(500);	
	printf("AT+IPR?\r\n");
	delay_ms(500);
	Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
	printf("AT+IPR=460800;&W\r\n");	//230400
	delay_ms(500);	
	Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
    //printf("AT&W\r\n");
	delay_ms(500);
	EC20_USART_Config_Baud(460800);
	delay_ms(1000);
	delay_ms(1000);
	#endif
    ///////////////////����ǽ�β///////////////////////////
    usart3_SendStr("change baud finish...\r\n");
	
	Exit_ec20_Transparent();
	EC20_Init_Transparent();
	
	create_start_pack(EC20_pack_sn++);
	create_heart_pack(EC20_pack_sn++);
	//create_tsCMD_pack(Pack_cmd_up, EC20_MODE_TRANSPARENT, TRANSPORT_DIR_UP, TS_UPSTREAM_BUF_SIZE * 2);
	//create_tsCMD_pack(Pack_cmd_down, EC20_MODE_TRANSPARENT, TRANSPORT_DIR_DOWN, TS_DOWNSTREAM_BUF_SIZE * 2);
	
	delay_ms(1000);	//���������·�Ӧ������
	//EC20_Send_Str(Pack_start);
	
	state_4g.linked = true;
	led_green_on();
	heartTime.count = tim3_count + EC20_HEART_PRIOD;
	heartTime.flag = false;
	
	serverLinkTime.count = tim3_count + EC20_ServerLink_Priod;
	serverLinkTime.flag = false;
	
	Enter_ec20_Transparent();
	delay_ms(1000);
	//EC20Send_hexData_Transparent("0123456789" ,10);
	ec20_send_hex(Pack_start,34);
	while(1)
	{
		if(state_4g.linked == false)
		{
			led_red_on();
			usart3_SendStr("link error...\r\n");
			//Exit_ec20_Transparent();
			EC20_ReLink_Transparent();
			//EC20_Init_Transparent();
			//state_4g.linked = true;	
			//Enter_ec20_Transparent();
			create_start_pack(EC20_pack_sn++);
			ec20_send_hex(Pack_start,34);
			usart3_SendStr("link ok...\r\n");
			led_green_on();
		}
		
		if(is_upstream_buf1_full == true)
		{
			is_upstream_buf1_full = false;
			//EC20Send_StrData_Transparent(Pack_cmd_up);
			//EC20Send_hexData_Transparent((u8 *)usart6_dma_rx.p_buf1 ,TS_UPSTREAM_BUF_SIZE);
			EC20_PackSend_Data(EC20_packup_sn++, TRANSPORT_DIR_UP, (u8 *)usart6_dma_rx.p_buf1, TS_UPSTREAM_BUF_SIZE);
			heartTime.count = tim3_count + EC20_HEART_PRIOD;
			usart3_SendStr("up 1 ...\r\n");
		}
		if(is_upstream_buf2_full == true)
		{
			is_upstream_buf2_full = false;
			//EC20Send_StrData_Transparent(Pack_cmd_up);
			//EC20Send_hexData_Transparent((u8 *)usart6_dma_rx.p_buf2 ,TS_UPSTREAM_BUF_SIZE);
			EC20_PackSend_Data(EC20_packup_sn++, TRANSPORT_DIR_UP, (u8 *)usart6_dma_rx.p_buf2, TS_UPSTREAM_BUF_SIZE);
			heartTime.count = tim3_count + EC20_HEART_PRIOD;
			usart3_SendStr("up 2 ...\r\n");
		}
		if(is_downstream_buf1_full == true)
		{
			is_downstream_buf1_full = false;
			//EC20Send_StrData_Transparent(Pack_cmd_down);
			//EC20Send_hexData_Transparent((u8 *)uart4_dma_rx.p_buf1 ,TS_DOWNSTREAM_BUF_SIZE);
			EC20_PackSend_Data(EC20_packdown_sn++, TRANSPORT_DIR_DOWN, (u8 *)uart4_dma_rx.p_buf1, TS_DOWNSTREAM_BUF_SIZE);
			heartTime.count = tim3_count + EC20_HEART_PRIOD;
			usart3_SendStr("down 1 ...\r\n");
		}
		if(is_downstream_buf2_full == true)
		{
			is_downstream_buf2_full = false;
			//EC20Send_StrData_Transparent(Pack_cmd_down);
			//EC20Send_hexData_Transparent((u8 *)uart4_dma_rx.p_buf2 ,TS_DOWNSTREAM_BUF_SIZE);
			EC20_PackSend_Data(EC20_packdown_sn++, TRANSPORT_DIR_DOWN, (u8 *)uart4_dma_rx.p_buf2, TS_DOWNSTREAM_BUF_SIZE);
			heartTime.count = tim3_count + EC20_HEART_PRIOD;
			usart3_SendStr("down 2 ...\r\n");
		}
		if(heartTime.flag == true)
		{
			heartTime.flag = false;
			heartTime.count = tim3_count + EC20_HEART_PRIOD;
			create_heart_pack(EC20_pack_sn++);
			//EC20Send_StrData_Transparent(Pack_heart);
			ec20_send_hex(Pack_heart,17);
			usart3_SendStr("heart  ...\r\n");
		}
		if(serverLinkTime.flag == true)
		{
			serverLinkTime.flag = false;
			serverLinkTime.count = tim3_count + EC20_ServerLink_Priod;
			
			state_4g.faultCnt = 0;
			
			STR_Index = strstr((const char*)usart_4g.p_buf, (const char*)"NO CARRIER");
			if(STR_Index != NULL)	//���������ߵ�
			{
				state_4g.linked = false;
				state_4g.faultCnt++;
			}
			STR_Index = strstr((const char*)usart_4g.p_buf, (const char*)"ERROR");
			if(STR_Index != NULL)	//δ֪����
			{
				state_4g.linked = false;
				state_4g.faultCnt++;
			}
			STR_Index = strstr((const char*)usart_4g.p_buf, (const char*)"+QIURC: \042closed\042,0");
			if(STR_Index != NULL)	//�������ر�
			{
				state_4g.linked = false;
				state_4g.server = closed;
				state_4g.faultCnt++;
			}
			STR_Index = strstr((const char*)usart_4g.p_buf, (const char*)"RDY");
			if(STR_Index != NULL)	//δ֪����
			{
				//state_4g.linked = false;
				//state_4g.server = closed;
				usart3_SendStr("RDY appeared ......\r\n");
			}
			if(state_4g.faultCnt == 0)	//û�����Ϲ��ϣ����жϷ���������
			{
				STR_Index = strstr((const char*)usart_4g.p_buf, (const char*)"^techx&");
				if(STR_Index != NULL)

				{
					state_4g.SerHeartCnt = 0;
				}
				else
				{
					state_4g.SerHeartCnt ++;
				}

				if(state_4g.SerHeartCnt > 3)
				{
					state_4g.linked = false;
					state_4g.SerHeartCnt = 0;
				}
			}
				
			Clear_Buffer(usart_4g.p_buf, &usart_4g.count);
		}
		
	}
}




