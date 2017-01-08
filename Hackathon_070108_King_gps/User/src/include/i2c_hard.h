
#ifndef __HARD_I2C_DRIVER_H__
#define __HARD_I2C_DRIVER_H__
#include "stm32f10x.h"
//I2C��ʼ����Ϣ�ṹ��
typedef struct
{
    unsigned char controlMode;  ///<I2C���Ʒ�ʽ
    unsigned int  speed;        ///<I2C��д�ٶ�
    unsigned char subAddrMode;  ///<�Ƿ����ӵ�ַ
    unsigned char subAddrWidth; ///<�ӵ�ַ�ֽ���
    unsigned short slaveAddr;   ///<�豸��ַ
	unsigned char channel;		///<��Ҫ��ʼ����ͨ��
}I2C_INIT_INFO;

#define MASTER_MODE_SELECT_FAILED                          (1)
#define MASTER_TRANSMITTER_MODE_SELECTED_FAILED            (2)    
#define MASTER_RECEIVER_MODE_SELECTED_FAILED               (3)
#define MASTER_BYTE_RECEIVED_FAILED                        (4)
#define MASTER_BYTE_TRANSMITTING_FAILED                    (5)
#define MASTER_BYTE_TRANSMITTED_FAILED                     (6)
#define MASTER_MODE_ADDRESS10_FAILED                       (7)
#define MASTER_GET_I2C_FLAG_BUSY_FAILED					   (8)

#define I2C1_CHANNEL    0   //ʹ��I2Cͨ��1 channel
#define I2C2_CHANNEL    1   //ʹ��I2Cͨ��2
extern uint8_t VT_I2C_HardInit(I2C_INIT_INFO *I2C_Info);
extern uint8_t VT_I2C_HardWriteNByte(I2C_TypeDef *I2Cx, uint32_t subaddr, uint8_t *s, uint32_t num);
extern uint8_t VT_I2C_HardReadNByte(I2C_TypeDef *I2Cx, uint32_t suba,uint8_t *s,uint32_t num);
#endif

