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

char Uart3_ReceiveDat[500] = {0};         //����3�Ļ�����
int Uart3_BufPoint = 0;                   //�������������ʱ��ָ�룬ÿ��һ������һ��
uint8_t Uart3_ReceStatue = UnFinished;    //һ֡�����Ƿ������ɱ�־λ

char DatFromServer[300];                  //�ӷ��������������ݻ�����
int DatFromServerPoint = 0;


VOID user3_irqhandle(void)
{
  unsigned char ch;
  
	if (USART_GetITStatus (USART3, USART_IT_RXNE) != RESET)
  {
    ch = USART_ReceiveData(USART3);		
    if (ch != '\0')
    {  					
      Uart3_ReceiveDat[Uart3_BufPoint++] = ch;    //�����ݷ��뻺����
      DatFromServer[DatFromServerPoint++] = ch;
        if (Uart3_BufPoint >= 499)                  //��ֹ���鳬��
          Uart3_BufPoint = 0;
        if (DatFromServerPoint >= 299)
          DatFromServerPoint = 0;
      }
      else
      {
        Uart3_ReceiveDat[Uart3_BufPoint++] = 0xaa;
        Uart3_ReceiveDat[Uart3_BufPoint] = '\0';
        DatFromServerPoint = 0;
        GPRS_ReceMsgProcess();                      //���մ�����
      }
    }
}

void uart3_nvic(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


VOID Uart3_Interrupt(VOID)
{
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  uart3_nvic();
  
  LOS_HwiCreate(39, 0,0,user3_irqhandle,0);//�����ж�
  LOS_IntRestore(uvIntSave);
}

 /**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(USART3, ENABLE);	    
  
  Uart3_Interrupt();
}


static void USART3_SendByte(u8 dat)
{
	USART_SendData(USART3, dat);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == 0);
}

void USART3_SendBuf(uint8_t *str)
{
  int i = 0;
  while(str[i] != '\0')
  {
    USART3_SendByte(str[i]);
    i++;
  }
}


/*
 * ��������Uart3_BufClear
 * ����  ������buf���
 * ����  ����
 * ����  ����
 */
void Uart3_BufClear (void)
{
  int i = 0;
  for (i = 0; i < 300; i++)
  {
    Uart3_ReceiveDat[i] = 0x00;
  }
  Uart3_ReceiveDat[200] = '\0';
  Uart3_BufPoint = 0;
}



