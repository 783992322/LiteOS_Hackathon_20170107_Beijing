#ifndef __SBUS_H_
#define __SBUS_H_
#include "stdint.h"
/* �ӻ������ж�ȡ���� */
void sbus_get(uint16_t addr, uint16_t len, uint16_t *buf);
/* �����ݼ��ص��ڴ���� */
void sbus_set(uint16_t addr, uint16_t len, const uint16_t *buf);


#endif // __SBUS_H_
