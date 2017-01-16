/* Includes ------------------------------------------------------------------*/
// Huawei LiteOSͷ�ļ�
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
// STM32Ӳ�����ͷ�ļ�
#include "stm32f10x.h"
#include "usart2.h"
#include "mcu_init.h"
#include "ult.h"
#include "sht10.h"
#include "xassert.h"
#include "bh1750_s.h"
#include "usart.h"
#include "task.h"
void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);
UINT32 g_TestTskHandle;
void Delay(__IO uint32_t nCount) //����ʱ
{
	for(; nCount != 0; nCount--);
}

void hardware_init(void)
{
	//LED_GPIO_Config();
	//USART_Config();
	RCC_Configuration(); /* ���õ�Ƭ��ϵͳʱ��*/
	NVIC_Configuration();/* ����Ƕ���ж�����*/
	Systick_Init(72);    /*��ʼ��Systick����ʱ��*/
	usart2_init(9600);
	usart1_init(9600);
	sht10_init();
    
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();
    usr_umqtt_init();
	SOFT_DELAY;
	uwRet = creat_task_mqtt();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	uwRet = creat_task_get_air_temp_humi();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	
	uwRet = creat_task_get_lux();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	
	uwRet = creat_task_test();
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
