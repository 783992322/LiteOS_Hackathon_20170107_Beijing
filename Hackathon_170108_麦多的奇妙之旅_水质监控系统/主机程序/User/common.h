#ifndef __COMMON_H
#define __COMMON_H

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

#include "Delay.h"

#include "Bsp_GPRS.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "Bsp_Key.h"
#include "Bsp_Beep.h"
#include "Screen.h"
#include "Bsp_ChipID.h"
#include "Bsp_DS18B20.h"
#include "Bsp_PowerDown.h"
#include "Bsp_WaterPwmin_Tim3.h"
#include "Bsp_Water_Level.h"
#include "Bsp_PH.h"
#include "Bsp_Flash.h"
#include "Bsp_RTC.h"
#include "Slave_Control.h"
#include "Bsp_Uart2_Zgb.h"
#include "Bsp_Can.h"
#include "Bsp_Uart3_A6.h"
#include "SelfDatGet.h"
#include "Up2Server.h"

#define MAX_SlaveNum      (20)            //֧�����Ĵӻ�����
#define GetSensorTime     (2)             //�ɼ���������ʱ�������룩
#define TouchSlaveTime    (10)            //������ѯ�ӻ�����(��)
#define SlaveLostTime     (4)             //�������ӻ���ʧʱ�䣨�룩
#define AlarmTime         (1)            //RTC��С�ж�ʱ�� (����)
#define UpLoadingTime     (15)            //������������ϴ�����ʱ����  ��UpLoadingTime x AlarmTime �룩
#define UpWarmingTime     (1)             //�ϴ�������  ��UpLoadingTime x AlarmTime �룩
#define UpWarmingAgainCountTime (3)           //�ظ��ϴ�������
#define ClearWarmingUpTime (1)
#define SlaveLostCount     6

/* GPRS�� */
#define DefaultValue        0
#define NoResponse          11
#define NoSIMCard           12
#define GRPSGetFailue       13
#define PDPGetFailue        14
#define GetTCPFailue        15
#define ServeNoResponse     16
#define ErrorFromServe      17
#define ServerNoUser        18
#define GetTimeError        19
#define LocalFault          20

/* ϵͳ����ʱ������ */
#define Yaer    2016
#define Moon    10
#define Day     19
#define Hour    20
#define Min     25
#define Sec     0

#define PH_Max              14
#define PH_Min              0
#define WaterTmp_Max        100
#define WaterTmp_Min        0
#define EnvirTemp_Max       100
#define EnvirTemp_Min       0
#define WaterSpeed_Max      14
#define WaterSpeed_Min      0

#define SUCCESS     1
#define FAILUED     0

#define USE_ZGB     1
#define USE_CAN     0

#define ON          1
#define OFF         0

extern uint8_t Connect_Way;
//extern uint8_t TCPIsUsed;

#endif
