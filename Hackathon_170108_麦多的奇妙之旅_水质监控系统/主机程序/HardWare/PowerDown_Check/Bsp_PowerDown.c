#include "common.h"

/*
 * ��������PowDon_GPIO_Config
 * ����  ��IO��ʼ��
 * ����  ����
 * ����  ����
 */
void PowDon_GPIO_Config (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_PowDon,ENABLE);	//ʹ�ܸ��ù���ʱ��
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PowDon;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   
  GPIO_Init(GPIOx_Powdon, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOx_Powdon,GPIO_Pin_PowDon);
  
}


/*
 * ��������PowDon_Init
 * ����  ���������ʼ��
 * ����  ����
 * ����  ����
 */
void PowDon_Init (void)
{
  PowDon_GPIO_Config();
}

/*
 * ��������CheckPowerDown
 * ����  ������Ƿ����
 * ����  ����
 * ����  ����
 */
void CheckPowerDown (void)
{
  if (GPIO_ReadInputDataBit (GPIOx_Powdon, GPIO_Pin_PowDon))    //û��
  {
    Flash_FifoBuf[5] |= (1 << 20);
  }
  else    //�е�
  {
    if (((Flash_FifoBuf[5] >> 20) & 0x01) == 1)
    {
      WarmClearStatue++;
      Flash_FifoBuf[5] &= ~(0x3 << 20);
      WarmUpCount[0][5] = 0;
    }
  }
}
