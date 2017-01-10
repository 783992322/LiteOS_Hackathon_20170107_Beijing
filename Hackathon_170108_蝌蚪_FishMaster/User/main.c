/*----------------------------------------------------------------------------------------------------
	                  ����ר��  FishMaster
------------------------------------------------------------------------------------------------------
  ����: ����ר��
  �Ŷ�: ���
  ��Ա: ��ʯ��	email: 15512180705@163.com			WeChat: 15512180705
		������	email: 1693779179@qq.com			WeChat: 15712858060
		�����	email: gu332523602@gmail.com		WeChat: 18330115758
		��ͨ	email: zhangtong@southjun.com		WeChat: 15830192870
  ����: 2017-01-07
  �汾: v1.0
------------------------------------------------------------------------------------------------------*/

#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
#include "los_hwi.h"

#include "stm32f10x.h"
#include "bsp_led.h"
#include "usart.h"	
#include "usart2.h"
#include "OLED_I2C.h"  
#include "DS18B20.h"
#include "Do.h"
#include "stdio.h"

void Delay(__IO u32 nCount);						//����ʱ����

UINT8 CMD[3] = {0xFD, 0xFD, 0xFD};					//TDS��������������
UINT8 Tx_Data[3] = {0};
u8 TEMPL[2] = {0};
u8 TEMPH[2] = {0};
u8 TDS1[2] = {0};									//��λTDS
u8 TDS2[2] = {0};									//ʮλTDS
u8 TDS3[2] = {0};									//��λTDS
u8 TDS4[2] = {0};									//ǧλTDS
u32 TDSNUM; //TDS����
UINT32 g_TestTskHandle;

/*******************************************************************************
* ������	: Delay()
* ����	    : ����ʱ
* �������  : __IO uint32_t nCount
* ���ز���  : ��
********************************************************************************/
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

/*******************************************************************************
* ������	: hardware_init()
* ����	    : ��ΧӲ����ʼ��
* �������  : ��
* ���ز���  : ��
********************************************************************************/
void hardware_init(void)
{
	LED_GPIO_Config();
	USART1_Config(57600);									//USART1���ڳ�ʼ�� 
	USART2_Config(9600);									//USART2���ڳ�ʼ�� 
	LOS_HwiCreate(37, 0, 0, USART1_IRQHandler, NULL);		//��������1�����ж�
	LOS_HwiCreate(38, 0, 0, USART2_IRQHandler, NULL);		//��������2�����ж�
	DS18B20_IN1_GPIO_Config(); 								//��ʼ��18B20
	DO_Config();											//�̵�����ʼ��
    I2C_Configuration();									//��ʼ��IIC
	OLED_Init();											//��ʼ��OLED
	OLED_Start();											//OLED��������
	Delay(9999999); 										//��ʱһ��ʱ��Ϊ����ʾ��������
}

/*******************************************************************************
* ������	: task1()
* ����	    : ����1��ȡ�¶��ϱ�
* �������  : ��
* ���ز���  : ��
********************************************************************************/
VOID task1(void)
{
	UINT32 uwRet = LOS_OK;
	UINT32 TX_NUM;
	
	while(1)
	{
		Tx_Data[0] = DS18B20_IN1_Get_Temp();						//��ȡ�¶�
		if(receiveOK == 1)											//���ڽ������
		{
			Tx_Data[1] = USART2_RX_BUF[1];      					//�ж��л�ȡTDSֵ��λ
			Tx_Data[2] = USART2_RX_BUF[2];
			receiveOK = 0;
		}
		TX_NUM = Tx_Data[2] + (Tx_Data[1] << 8) + (Tx_Data[0] << 16); 
		printf("%X", TX_NUM); 										//�������ݸ�WIFIģ������һ��Ҫ���ַ�����������ƽ̨����ʶ
        OLED_CLS();
		
		OLED_ShowStr(2, 0, (unsigned char*)"TEMP:", 2);
		OLED_ShowStr(75, 0, (unsigned char*)"C", 2);				//�¶Ƚ���
	    OLED_ShowStr(2, 4, (unsigned char*)"TDS:", 2);
		OLED_ShowStr(75, 4, (unsigned char*)"PPM", 2);				//�¶Ƚ���
		TEMPL[0] = (Tx_Data[0] / 10) + 0x30;
		TEMPH[0] = (Tx_Data[0] % 10) + 0x30;
	    TDS4[0] = (Tx_Data[2] / 10) + 0x30;							//TDSǧλ
		TDS3[0] = (Tx_Data[1] / 100) + 0x30;						//TDS��λ
		TDS2[0] = ((Tx_Data[1] % 100) / 10) + 0x30;					//TDSʮλ
		TDS1[0] = ((Tx_Data[1] % 100) % 10) + 0x30;					//TDS��λ
		
		OLED_ShowStr(50, 0, (unsigned char*)TEMPL, 2);
		OLED_ShowStr(58, 0, (unsigned char*)TEMPH, 2);
		
		OLED_ShowStr(42, 4, (unsigned char*)TDS3, 2);				//��ʾ��λ
		OLED_ShowStr(50, 4, (unsigned char*)TDS2, 2);				//��ʾʮλ
		OLED_ShowStr(58, 4, (unsigned char*)TDS1, 2);				//��ʾ��λ
		
		if(UART1_CMD == '5')
		{
			Relay1ON;
		}
		else
		{
			Relay1OFF;
		}
		
		LED1_TOGGLE;
		
		uwRet = LOS_TaskDelay(1000);								//����ϵͳ��ʱ
		if(uwRet !=LOS_OK)
			return;
	}
}

/*******************************************************************************
* ������	: creat_task1()
* ����	    : ��������1
* �������  : ��
* ���ز���  : ��
********************************************************************************/
UINT32 creat_task1(void) 
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task1";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle, &task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

/*******************************************************************************
* ������	: task2()
* ����	    : ����2 TDSˮ�ʼ��
* �������  : ��
* ���ز���  : ��
********************************************************************************/
VOID task2(void)
{
	UINT32 uwRet = LOS_OK;
	while(1)
	{
		UART2_Send_Array(CMD, 3); 										//����FD FD FD ��ȡTDS
		LED2_TOGGLE;
		uwRet = LOS_TaskDelay(3000);
		if(uwRet !=LOS_OK)
			return;
	}
}

/*******************************************************************************
* ������	: creat_task2()
* ����	    : ��������2
* �������  : ��
* ���ز���  : ��
********************************************************************************/
UINT32 creat_task2(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;
	task_init_param.pcName = "task2";
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task2;
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle, &task_init_param);
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

/*******************************************************************************
* ������	: osAppInit()
* ����	    : Ӧ�ó�ʼ��
* �������  : ��
* ���ز���  : ��
********************************************************************************/
UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();
	LED2_ON;
	Delay(0x0FFFFF);
	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	uwRet = creat_task2();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
