/**
 * ����ͷ�ļ�
 */
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"

/**
 * �û�ѡ��ʱ����ʼ����Ժ󣬵�·ͨ��
 * ����ѡ����ʱ���Ժ�ֹͣ��磬��·�Զ��ϵ�
 */
int main(void)
{	
	// ��ʼ��LED�˿�
	LED_GPIO_Config();
	SysTick_Init();
	// �û�ѡ����ɣ���·��ʼͨ��
	LED1( ON );
	// ����У���ʱ�ض�ʱ�䣬������1sΪ��
	// ѡ����ʱ���Է���Ϊ��λ
		Delay_us(100000);
	// ���ʱ�䵽����·�Զ��ϵ�
	LED1( OFF );
	// �����ش���������
}
