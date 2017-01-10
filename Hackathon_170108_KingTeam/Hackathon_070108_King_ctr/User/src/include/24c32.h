#ifndef __24c32_H
#define __24c32_H

#include "stm32f10x.h"
#include "types.h"

#define I2C_SCK	        GPIO_Pin_6  //out
#define I2C_SDA	        GPIO_Pin_7
#define EEPROM_PORT     GPIOB
/* ��ʼ�� */
void at24c32_init(void);
/* д��һ���ֽ����� */
void at24c32_write(uint16 addr, unsigned char val);
/* ����һ���ֽ����� */
unsigned char at24c32_read(uint16 addr);
/* д��һ������ */
void eeprom_block_write(uint16_t addr, uint16_t len, const uint8 *buf);
/* ����һ������ */
void eeprom_block_read(uint16_t addr, uint16_t len, uint8_t *buf);
/* �������� */
void erase_eeprom(uint16 startAddr, uint16 len);

#endif /* __MAIN_H */




