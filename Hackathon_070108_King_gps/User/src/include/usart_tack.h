#ifndef __USART_TACK_H
#define __USART_TACK_H
#include "stdint.h"
#define HEADER_0X55 0x55
#define HEADER_0XAA 0xaa
#define TACK_LENGTH 0x00
#define TACK_DATA_RX     0x01
#define MAX_RX_LENGTH   64

#define COLLECT_1       0x01
#define COLLECT_2       0x02
#define CONTROL_0       0x03
typedef uint8_t rx_length_t;  /*���¶�����ܳ�������*/
typedef struct rx_data_strcut_{
    uint8_t rxbuf[MAX_RX_LENGTH];  /*���ջ�����*/
    uint8_t rx_complete_flag; /* ������ɱ�־λ*/
    rx_length_t rx_length;   /*�������ݵĳ���,�Ǵ��뻺�����ĳ��ȣ�������֡ͷ������֡λУ��λ*/  
}rx_data_t;
void usart_tack_init(void);
void usart_tack_poll(void);
#endif
