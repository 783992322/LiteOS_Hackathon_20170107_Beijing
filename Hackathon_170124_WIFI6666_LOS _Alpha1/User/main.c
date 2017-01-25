/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-01-xx
  * @brief   WF-ESP8266 WiFiģ�����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "los_base.h"
#include "los_config.h"
#include "los_typedef.h"
#include "los_hwi.h"
#include "los_task.ph"
#include "los_sem.h"
#include "los_event.h"
#include "los_memory.h"
#include "los_queue.ph"
// STM32??????????
#include "stm32f10x.h"
#include "bsp_led.h"

 
 
#include "bsp_usart1.h"
//#include "bsp_SysTick.h"
#include "bsp_esp8266.h"
#include "test.h"
#include "bsp_dht11.h"
#include "bsp_key.h" 
#include "bsp_hall.h" 

UINT32 g_TestTskHandle, uwMs, uwTick;


void hardware_init(void)
{
/* ��ʼ�� */
	LED_GPIO_Config(); 
	HALL_GPIO_Config();
  USARTx_Config ();                                                              //��ʼ������1
//	SysTick_Init ();                                                               //���� SysTick Ϊ 1ms �ж�һ�� 
	ESP8266_Init ();                                                               //��ʼ��WiFiģ��ʹ�õĽӿں�����
	DHT11_Init ();

	printf ( "\r\nҰ�� WF-ESP8266 WiFiģ���������\r\n" );                          //��ӡ����������ʾ��Ϣ

//  ESP8266_StaTcpClient_UnvarnishTest ();	
//  while ( 1 );
}

void task1(void)
{
	hardware_init();
	LED1_ON;
//	uwTick = LOS_MS2Tick(1000);//1000 msת��ΪTick

//	LOS_TaskDelay(200000);
//	LED1_OFF;
	ESP8266_StaTcpClient_UnvarnishTest();
//	LED1_TOGGLE;
}

UINT32 creat_task1(void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;//?????
	task_init_param.pcName = "task1";//???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task1;//??????
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//??????
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_TestTskHandle,&task_init_param);//????????
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	uwRet = creat_task1();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}





/*********************************************END OF FILE**********************/
