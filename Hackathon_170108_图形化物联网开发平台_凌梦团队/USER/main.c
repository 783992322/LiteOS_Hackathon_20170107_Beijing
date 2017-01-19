
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "proto.h"
#include "IO.h"

#include "los_task.h"

//�����Ҵ�����һ������, �����з�����ȡ�����ָ�ִ��

//��ֲLiteOSʱ������ʱ����ͻ, ����OS���Ȳ�����, ʱ������������ʱ������ϵͳ����, ֻ����һ��������ִ��,
//�����еȴ�����ͼ�λ���λ�����ʹ����ֽ�������, �յ�֮��ʹ洢��RAM�в���ʼ�������������ִ��
//�յ����ֽ�������.

UINT32 myTask(void)
{
	Timer_Init();
	IO_Init();
	Serial_Init(115200);
	//Serial_Init(9600);
	
	Timer_delay_ms(500);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	Timer_delay_ms(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	
	setup();
	while(1) {
		
		//������ȡ�����ָ�ִ��
		loop();
		
		
		//���ﲻ��Ҫ��ʱ, ���������˳�
		/*
		uwRet = LOS_TaskDelay(50);
		if (uwRet != LOS_OK)
		{
				printf("Delay balancecar Failed.\r\n");
				return LOS_NOK;
		}
		*/
	}
}




