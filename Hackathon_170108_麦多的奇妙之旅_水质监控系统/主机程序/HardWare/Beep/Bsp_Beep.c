#include "common.h"

void Beep (uint8_t Statue)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  if (Statue == ON)
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
    GPIO_ResetBits(GPIOx_Beep,GPIO_Pin_Beep);
  }
  else
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
    GPIO_SetBits(GPIOx_Beep,GPIO_Pin_Beep);
  }
}

/*
 * ��������Beep_ON_Nop
 * ����  ����������ʾ
 * ����  ��Time������ʱ��
 * ����  ����
 */
void Beep_ON_Nop (int Time)
{
  Beep(ON);
  UserDelay_KeyScan(Time);
  Beep(OFF);
  UserDelay_KeyScan(Time);
}

/*
 * ��������Beep_Init
 * ����  �����������ų�ʼ��
 * ����  ����
 * ����  ����
 */
void Beep_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_Beep, ENABLE); 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Beep;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOx_Beep, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Beep,GPIO_Pin_Beep);
  
  Beep(ON);
  Delay_ms(50);
  Beep(OFF);
}
