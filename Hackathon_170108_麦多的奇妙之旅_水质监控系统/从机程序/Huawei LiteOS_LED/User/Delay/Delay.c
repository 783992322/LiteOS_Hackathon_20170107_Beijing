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


