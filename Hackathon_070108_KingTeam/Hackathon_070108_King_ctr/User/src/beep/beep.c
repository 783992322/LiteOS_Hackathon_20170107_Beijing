#include "beep.h"

void beep_gpio_configuration()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*��������ʼ��*/
void beep_init(void)
{
     beep_gpio_configuration();
     GPIO_ResetBits(GPIOA,  GPIO_Pin_8);
    
    //GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}
/*����������*/
void beep_on(void)
{
	
	GPIO_SetBits(GPIOA,  GPIO_Pin_8);
}
/*�������ر�*/
void beep_off(void)
{
	GPIO_ResetBits(GPIOA,  GPIO_Pin_8);
}
