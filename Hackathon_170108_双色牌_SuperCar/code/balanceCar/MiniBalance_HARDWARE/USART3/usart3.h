#ifndef __USRAT3_H
#define __USRAT3_H 
#include "sys.h"	  	
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
extern u8 Usart3_Receive;
extern int read_UART3;
extern u8 temp_read ;

void uart3_init(u32 bound);
void USART3_IRQHandler(void);
#endif

