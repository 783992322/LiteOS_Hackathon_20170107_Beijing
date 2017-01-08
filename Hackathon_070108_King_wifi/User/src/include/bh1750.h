#ifndef	__BH1750_H_
#define __BH1750_H_

#include "stm32f10x.h"
#define BH1750_WRITE_OK     0    //д����
#define BH1750_READ_OK      0    //������
#define BH1750_WRITE_ERROR  -1   // -1
#define BH1750_READ_ERROR   -1   // -1
int8_t bh1750_get_lux_val(float *lux_val);  /*��bh1750�ж�ȡ����ֵ*/
void bh1750_init(void);   /*BH1750��ʼ��*/
#endif
//*********************			END OF FILE			*********************
