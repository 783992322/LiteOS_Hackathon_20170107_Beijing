//#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "stm32f10x.h"
#include "types.h"
#include "timer.h"
/*�����ô�����������������*/
//#ifndef NDEBUG
//���ڽ��ջ����� 	
//���ڽ��ջ����� 	
//uint8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
uint8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
//uint8_t rx_buf_len = 0;
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void usart2_init(uint32 bound)
{  
        //A2 TX2 A3 RX2
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	// GPIOBʱ��
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 ,ENABLE); //����2ʱ��ʹ��
        USART_DeInit(USART2);  //��λ����2
        //USART2_TX   PA2
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
        GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
   
        //USART2_RX	  PA3
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
        GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
	
        USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
        USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Enable rx enable, 
        USART_Init(USART2, &USART_InitStructure);
        USART_Cmd(USART2, ENABLE);

        //ʹ�ܽ����ж�
        //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
        //�����ж����ȼ�
       

}

//����2,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void usart2_printf(char* fmt,...)  
{  
        u16 i,j; 
        va_list ap; 
        va_start(ap,fmt);
        vsprintf((char*)USART2_TX_BUF,fmt,ap);
        va_end(ap);
        i=strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
        for(j=0;j<i;j++)							//ѭ����������
        {
            while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
            USART_SendData(USART2,USART2_TX_BUF[j]); 
        } 
}

void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(uint8_t * str)
{
    while(*str)
    {
        USART_SendData(USART2, *str++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

void USART2_IRQHandler(void)
{
	u16 str1;   
    //uint8_t i = 0;
  	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  	{
     USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
     USART_SendData(USART2, str1);
     while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
     USART_ITConfig( USART2,USART_IT_RXNE, ENABLE);
  	}
}
//#endif
 



















