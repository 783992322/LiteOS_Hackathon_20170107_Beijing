#include "test.h"
void test_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // PB12 ���� 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void test_delay(void)
{
	  GPIO_ResetBits(GPIOB,  GPIO_Pin_12);
	  delay_ms(100);
	  GPIO_SetBits(GPIOB,  GPIO_Pin_12);
	  delay_ms(100);
}
