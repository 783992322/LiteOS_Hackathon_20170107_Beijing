/**************************************/
/* �ļ���io_ctr.h
   ���ܣ��������������ƹ���ʹ��ͷ�ļ�
   �汾��1.0
***************************************/
#ifndef IO_CTR_H
#define IO_CTR_H
#include "stm32f10x.h"

/*���ƶ˿ڳ�ʼ����Ĭ�Ϲر������豸*/
void io_ctr_init(void);
/*������Դ*/
void open_dev(void);
void close_dev(void);
#endif

