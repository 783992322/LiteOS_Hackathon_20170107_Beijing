#include "Common.h"

//*************ȫ�ֱ���*************//
//*********************************//

/*
 * ��������Delay_us
 * ����  ��us��ʱ
 * ����  ��time����ʱ����
 * ����  ����
 */
void Delay_us (u16 time)
{
	u16 i = 0;
	while(time--)
		{
			i = 10;
			while(i--);
		}
}


/*
 * ��������Delay_ms
 * ����  ��ms��ʱ
 * ����  ��time����ʱ����
 * ����  ����
 */
void Delay_ms (u16 time)
{
	u16 i = 0;
	while(time--)
		{
			i = 12000;
			while(i--);
		}
}

extern uint8_t OSStatue;
void UserDelay_KeyScan (u16 time)
{
//  TimingDelay = time;	
//  SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
//  
//  while(TimingDelay != 0)
//  {
//    Key_Scan();
//  }
//  
//  SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
  
  
//  if (OSStatue == 0)
    Delay_ms(time * 0.6);
//  else
//  {
//    LOS_TaskDelay(time);//ϵͳ��ʱ1S
//  }
}


