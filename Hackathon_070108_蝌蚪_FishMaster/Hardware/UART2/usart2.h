#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"

#define USART_REC_LEN2  			5				//�����������ֽ��� 20

extern u8 USART2_RX_BUF[USART_REC_LEN2];			//���ջ���,���USART_REC_LEN���ֽ�.
extern u8 receiveOK;

void USART2_Config(u32 bound);
void UART2_Send_Str(unsigned char *s);
void UART2_Send_Array(unsigned char send_array[],unsigned char num);
void USART2_Send_Byte(unsigned char byte);
void USART2_IRQHandler(void);

#endif /* __USART2_H */
