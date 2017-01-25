/**
  ******************************************************************************
  * @file    bsp_HALL.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����Ӧ��bsp��ɨ��ģʽ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./Hall/bsp_hall.h"  

/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void HALL_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(HALL1_GPIO_CLK,ENABLE);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = HALL1_GPIO_PIN; 
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(HALL1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
//	GPIO_InitStructure.GPIO_Pin = HALL2_GPIO_PIN; 
//	//���ð���������Ϊ��������
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//	//ʹ�ýṹ���ʼ������
//	GPIO_Init(HALL2_GPIO_PORT, &GPIO_InitStructure);	
}

 /*
 * ��������HALL_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��HALL_OFF(û���°���)��HALL_ON�����°�����
 */
uint8_t HALL_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == HALL_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == HALL_ON);   
		return 	HALL_ON;
	}
	else
		return HALL_OFF;
}
/*********************************************END OF FILE**********************/
