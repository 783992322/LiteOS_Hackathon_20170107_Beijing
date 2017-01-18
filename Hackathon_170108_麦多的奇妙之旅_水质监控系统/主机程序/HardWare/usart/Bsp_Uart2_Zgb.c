/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����STM32 F103-ָ���� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "common.h"

uint8_t Uart2_ReceiveDat[20] = {0};         //����2�Ļ�����

VOID user2_irqhandle(void)
{
  unsigned char ch;
  static uint8_t Uart2_ReceSta = 0; 
  static uint8_t Uart2_BufPoint = 0;
  
	if (USART_GetITStatus (USART2, USART_IT_RXNE) != RESET)
  {
    ch = USART_ReceiveData(USART2);		
    if (Uart2_ReceSta)
    {
      Uart2_ReceiveDat[Uart2_BufPoint++] = ch;      //�����ݷ��뻺����   

      if (ch == 0x0a)
      {
        Uart2_ReceSta = 0;
        if (Uart2_ReceiveDat[0] == 0x0f)            //ȷ���Ǵӻ����͵�����
        {
          ZGB_ReceMsgProcess(Uart2_ReceiveDat);
        }
      }      
    }
    else if (ch == 0x0f)
    {
      Uart2_BufPoint = 0;
      Uart2_ReceiveDat[Uart2_BufPoint++] = 0x0f;    //�����ݷ��뻺����     
      Uart2_ReceSta = 1;
    }
  }
}

void uart2_nvic(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


VOID Uart2_Interrupt(VOID)
{
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  uart2_nvic();
  
  LOS_HwiCreate(38, 2,0,user2_irqhandle,0);//�����ж�
  LOS_IntRestore(uvIntSave);
}

 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART2, ENABLE);	    
  
  Uart2_Interrupt();
}


void USART2_SendByte(u8 dat)
{
	USART_SendData(USART2, dat);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == 0);
}


void USART2_SendBuf(uint8_t *str)
{
  int i = 0;
  for (i = 0; i < 14; i++)
  {
    USART2_SendByte(str[i]);
  }
}
