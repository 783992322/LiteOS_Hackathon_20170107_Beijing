#include "common.h"

/*
 * ��������WatLevl_GPIO_Config
 * ����  ��IO��ʼ��
 * ����  ����
 * ����  ����
 */
void WatLevl_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_WatLevl,ENABLE);	//ʹ�ܸ��ù���ʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_WatLevl;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
  GPIO_Init(GPIOx_WatLevl, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Powdon,GPIO_Pin_PowDon);
}

/*
 * ��������Water_Level_Init
 * ����  ��ˮλ����ʼ��
 * ����  ����
 * ����  ����
 */
void Water_Level_Init (void)
{
  WatLevl_GPIO_Config();
}

/*
 * ��������CheckWaterLevel
 * ����  �����ˮλ
 * ����  ����
 * ����  ����
 */
void CheckWaterLevel (void)
{
  if (GPIO_ReadInputDataBit (GPIOx_WatLevl, GPIO_Pin_WatLevl))    //����
  {
    if (((Flash_FifoBuf[5] >> 24) & 0x01) == 1)
    {
      WarmClearStatue++;
      Flash_FifoBuf[5] &= ~(0x3 << 24);
    }
  }
  else      //�쳣
  {
    Flash_FifoBuf[5] |= (1 << 24);   
    WarmUpCount[0][3] = 0;    
  }
}
