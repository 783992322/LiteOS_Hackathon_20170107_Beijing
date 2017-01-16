/******************** (C) COPYRIGHT 2012 WildFire Team **************************

**********************************************************************************/
#include "usart2.h"

u8 USART2_RX_BUF[USART_REC_LEN2];			//���ջ���,���USART_REC_LEN���ֽ�.
u8 Usart2Count;								//���ռ���
u8 receiveOK = 0;

/*******************************************************************************
* ������	: USART2_Config()
* ����	    : USART2����
* �������  : u32 bound
* ���ز���  : ��
********************************************************************************/
void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_DeInit(USART2);   										//��λ����2
    USART2->SR&=~(1<<7);    										//�巢�ͳɹ���־

	/* USART2 GPIO config */
	/* Configure USART2 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART2, &USART_InitStructure); 
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/*******************************************************************************
* ������	: USART2_Send_Byte()
* ����	    : USART2����һ���ֽ�
* �������  : u8 byte
* ���ز���  : ��
********************************************************************************/
void USART2_Send_Byte(unsigned char byte)
{
        USART_SendData(USART1, byte);
        while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
}

/*******************************************************************************
* ������	: UART2_Send_Str()
* ����	    : UART2�����ַ���
* �������  : unsigned char *s
* ���ز���  : ��
********************************************************************************/
void UART2_Send_Str(unsigned char *s)
{
	unsigned char i=0;

	while(s[i]!='\0')
	{
		USART_SendData(USART2,s[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
		i++;
	}
}

/*******************************************************************************
* ������	: UART2_Send_Array()
* ����	    : UART2��������
* �������  : unsigned char send_array[],unsigned char num
* ���ز���  : ��
********************************************************************************/
void UART2_Send_Array(unsigned char send_array[],unsigned char num) 
{
	unsigned char i=0;

	while(i<num)
	{
		USART_SendData(USART2,send_array[i]);
		while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);
		i++;
	}
}

/*******************************************************************************
* ������	: USART2_IRQHandler()
* ����	    : USART2�ж�
* �������  : ��
* ���ز���  : ��
********************************************************************************/
void USART2_IRQHandler(void)
{   
	static char rCnt = 0;  
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{  
		USART2_RX_BUF[rCnt] = USART_ReceiveData(USART2);
		rCnt++;
		if(rCnt>=5)
		{
			rCnt = 0;
			receiveOK = 1;
		}
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
