#ifndef __Bsp_PH_H__
#define __Bsp_PH_H__
#include "stm32f10x.h"
#include "bitbound.h"

#define AD_Cs    	paout(4) 		
#define AD_Sclk		paout(5)
#define	AD_Dout   pain(6)
#define AD_Din		paout(7)

#define con_size		15

//�����ǼĴ���
#define AD_IdReg 		0x60				//Id�Ĵ���
#define	AD_ModeReg  0x80				//ģʽ�Ĵ���
#define AD_DataReg 	0x58				//������
#define AD_SrReg    0x40				//״̬�Ĵ���
#define AD_ConfReg	0x10				//���üĴ���
#define AD_IoReg		0x28        //IO�Ĵ���

void AD_delay(void);
void AD_Init(void);
void AD_IoInit(void);
void AD_Read(unsigned char nByte);
void AD_Write(u8 ByteData);
uint16_t GetPH (void);
extern u8 DataRead[con_size];




#endif
