#include "comm_struct.h"
#include "stdint.h"
#include "sbus.h"
#define REG_HOLDING_START (0)
#define REG_HOLDING_NREGS (64)

/* ���߼Ĵ��������� */
static uint16_t sbus_register_buf[REG_HOLDING_NREGS] = {0};
/* �����ݼ��ص��ڴ���� */
void sbus_set(uint16_t addr, uint16_t len, const uint16_t *buf)
{
        uint16_t *p = &sbus_register_buf[addr];

        /* ��ַ�����Ⱦ�����Ϊ0 */
        //xassert(addr != 0);
        //xassert(len  != 0);

        do
                *p++ = *buf++;
        while(--len);
}
/* �ӻ������ж�ȡ���� */
void sbus_get(uint16_t addr, uint16_t len, uint16_t *buf)
{
        uint16_t *p = &sbus_register_buf[addr];

        /* ��ַ�����Ⱦ�����Ϊ0 */
        //xassert(addr != 0);
        //xassert(len  != 0);

        do
                *buf++ = *p++;
        while(--len);
}
