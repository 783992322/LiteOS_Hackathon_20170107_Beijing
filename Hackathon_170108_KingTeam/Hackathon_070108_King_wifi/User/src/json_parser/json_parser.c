/*------------------------------------------------------------------*/
/* ���õ���json�����н������������ݰ����������Լ�����������             */
/*------------------------------------------------------------------*/
#include "json_parser.h"
#include "jsmn.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "comm_struct.h"
#include "sbus.h"
#include "types.h"
#include "usart3.h"
#include "io_ctr.h"
uint8_t rcv_jsonpack_flag;   //���յ�һ֡json���ݰ�
extern uint8 rx_buf_len;
char *json_pack;
enum type_t{sensor, cmd, log}; //ö������
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
uint8_t json_parse(void)
{
        int i;
        int r;
        static uint8_t id;
	    uint8_t pw;
	    uint8_t sta;
	    enum type_t type;
	    char* message_type;
        jsmn_parser p;
        jsmntok_t t[16]; /* We expect no more than 128 tokens */
        char *str[2];  /*���ﻹû�����ף�Ϊʲô��2*/
	    
        jsmn_init(&p);
	    //usart3_printf("1\r\n");
	    if(rcv_jsonpack_flag == 1){
					rcv_jsonpack_flag = 0;
					/*ÿ�ν��������ݳ���Ӧ��Ϊ���յĳ��ȣ������ǻ��������ܳ���*/
					r = jsmn_parse(&p, (const char *)USART3_RX_BUF, rx_buf_len, t, sizeof(t)/sizeof(t[0]));
					//�ѳ������㣬���¿�ʼ����һ֡����
					rx_buf_len = 0;
					//���룬���յ�һ֡���ݺ��Ƚ��д����Ƿ���Ҫ�رմ��ڽ����ж�
					/* Assume the top-level element is an object */
					if (r < 1 || t[0].type != JSMN_OBJECT) {
							return 1;
					}
					/* Loop over all keys of the root object */
					for (i = 1; i < r; i++) {
							/*������*/
							 /*ID�ж�*/
							 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "id") == 0) {
									/* We may want to do strtol() here to get numeric value */
									id = strtol((const char *)USART3_RX_BUF + t[i+1].start, str, 10);
									
									i++;
							}
							 /*��Ϣ����*/             
							 else if (jsoneq((const char *)USART3_RX_BUF, &t[i], "message_type") == 0) {
								     //�����ַ���
									 message_type = (char*)malloc( t[i+1].end-t[i+1].start + 1);
									 message_type = strncpy(message_type,(const char *)USART3_RX_BUF + t[i+1].start,  t[i+1].end-t[i+1].start + 1);
								     message_type[t[i+1].end-t[i+1].start] = '\0';
									 usart3_printf("mess = %s", message_type);
								     if(strcmp((const char*)message_type, "sensor") == 0)
										 type = sensor;
									 if(strcmp((const char*)message_type, "cmd") == 0)
										 type = cmd;
									 if(strcmp((const char*)message_type, "log") == 0)
										 type = log;
									 free(message_type);
									 usart3_printf("type = %d\r\n", type);
									 i++;
							}
							//������յ��ǿ���ָ���ô��Ҫ�ٽ�������tokens
							if(type == cmd){
								/*��Դ����*/
								 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "pw") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 pw = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
										 usart3_printf("pw = %d\r\n", pw);
										 i++;
								}
								 /*״̬��ʶ*/
								 else if (jsoneq((const char *)USART3_RX_BUF, &t[i], "sta") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 sta = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
										 usart3_printf("sta = %d\r\n", sta);
										 i++;
								}
								
								
							}
							//�������־���ߴ�������ֻ�����sta
							else if(type == sensor || type == log){
								/*״̬��ʶ*/
								 if (jsoneq((const char *)USART3_RX_BUF, &t[i], "sta") == 0) {
										/* We may want to do strtol() here to get numeric value */
										 sta = strtol((const char *)USART3_RX_BUF + t[i+1].start,str, 10);
									     usart3_printf("sta = %d\r\n", sta);
										 i++;
								}
							}
							 
					}
				 
				//��ˮ
				if(id == 1 )
			  {
					//�򿪽�ˮ��
					 if(pw == 1){
						 //usart3_printf("pw1 = %d", pw);
						
						 //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
						 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
						 //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
						 //GPIO_Init(GPIOC, &GPIO_InitStructure);
						 GPIO_SetBits(GPIOC,  GPIO_Pin_5);
						   //ivalve_state = 1;
						}
						else{
							//usart3_printf("pw0 = %d", pw);
							
							GPIO_ResetBits(GPIOC,  GPIO_Pin_5);
						} 
						
							
				//����ָ�״̬		
			 }
		 }
        //memset(sensor_limit_times, 6, 0);   /*ÿ��������*/
        return 0;
}
