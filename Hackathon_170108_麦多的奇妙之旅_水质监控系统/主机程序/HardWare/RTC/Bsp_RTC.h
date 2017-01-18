#ifndef __Bsp_RTC_H
#define __Bsp_RTC_H

#include "common.h"

//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 

extern long int NowTime;
extern _calendar_obj calendar;//ʱ�ӽṹ�� 
extern int User_year, User_month, User_day, User_hour, User_min, User_sec;

u8 RTC_Init(void);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��		

#endif
