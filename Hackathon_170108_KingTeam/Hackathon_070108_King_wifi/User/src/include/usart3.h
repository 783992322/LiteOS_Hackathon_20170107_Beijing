#ifndef __USART3_H
#define __USART3_H	 
#include "types.h"  
#include "stm32f10x.h"
//#include "config.h"
//#ifndef NDEBUG
#define USART3_MAX_RECV_LEN		600					//�����ջ����ֽ���
#define USART3_MAX_SEND_LEN		600					//����ͻ����ֽ���
#define USART3_RX_EN 			1					//0,������;1,����.

extern uint8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern uint8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�


void usart3_init(uint32 bound);				//����2��ʼ�� 
void usart3_printf(char* fmt,...);
void USART3_Puts(uint8_t * str);
void USART3_Putc(unsigned char c);
//#endif
#endif













