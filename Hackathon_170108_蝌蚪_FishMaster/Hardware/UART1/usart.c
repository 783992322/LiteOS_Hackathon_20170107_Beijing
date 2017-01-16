#include "stm32f10x.h"
#include "usart.h"
#include "stdio.h"

u8 USART_RX_BUF[USART_REC_LEN];     		//���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       					//����״̬���
u8 UsartCount;
u8 UART1_CMD; 								//��������
u8 num = 0;

/* �������´���,֧��printf����,������Ҫѡ��use MicroLIB */
#if 0
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle;
}; 

FILE __stdout;       

/*******************************************************************************
* ������	: fputc()
* ����	    : USART1����ַ�
* �������  : int ch, FILE *f
* ���ز���  : ��
********************************************************************************/
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	

    return ch;
}

/*******************************************************************************
* ������	: USART1_Config()
* ����	    : USART1����
* �������  : u32 bound
* ���ز���  : ��
********************************************************************************/
void USART1_Config(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	USART_DeInit(USART1);   										//��λ����1
    USART1->SR&=~(1<<7);    										//�巢�ͳɹ���־

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;						//PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;					//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);							//��ʼ��PA9

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);							//��ʼ��PA10
	
	USART_InitStructure.USART_BaudRate = bound;						//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;			//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;				//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);						//��ʼ������
	
#if EN_USART1_RX
    //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;		//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);					//�����ж�
#endif
    USART_Cmd(USART1, ENABLE);                    					//ʹ�ܴ���
}

/*******************************************************************************
* ������	: USART1_Send_Byte()
* ����	    : USART1����һ���ֽ�
* �������  : u8 byte
* ���ز���  : ��
********************************************************************************/
void USART1_Send_Byte(u8 byte)
{
	USART_SendData(USART1, byte);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
}

/*******************************************************************************
* ������	: UART1_Send_Str()
* ����	    : UART1�����ַ���
* �������  : unsigned char *s
* ���ز���  : ��
********************************************************************************/
void UART1_Send_Str(unsigned char *s)
{
	unsigned char i=0;

	while(s[i]!='\0')
	{
		USART_SendData(USART1,s[i]);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
		i++;
	}
}

/*******************************************************************************
* ������	: UART1_Send_Array()
* ����	    : UART1��������
* �������  : unsigned char send_array[],unsigned char num
* ���ز���  : ��
********************************************************************************/
void UART1_Send_Array(unsigned char send_array[],unsigned char num) 
{
	unsigned char i=0;

	while(i<num)
	{
		USART_SendData(USART1,send_array[i]);
		while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
		i++;
	}    
}

/*******************************************************************************
* ������	: USART1_IRQHandler()
* ����	    : USART1�ж�
* �������  : ��
* ���ز���  : ��
********************************************************************************/
void USART1_IRQHandler(void)
{ 
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{	
		UART1_CMD = USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������			
	}
}
