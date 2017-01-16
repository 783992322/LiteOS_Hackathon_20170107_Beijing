
#ifndef __SHT10_H__
#define __SHT10_H__
#include "stm32f10x.h"

enum {TEMP, HUMI};

/* GPIO��غ궨�� */
#define SHT10_AHB2_CLK	RCC_APB2Periph_GPIOC
#define SHT10_DATA_PIN	GPIO_Pin_10
#define SHT10_SCK_PIN	GPIO_Pin_12
#define SHT10_DATA_PORT	GPIOC
#define SHT10_SCK_PORT	GPIOC

#define SHT10_DATA_H()	GPIO_SetBits(SHT10_DATA_PORT, SHT10_DATA_PIN)			 //����DATA������
#define SHT10_DATA_L()	GPIO_ResetBits(SHT10_DATA_PORT, SHT10_DATA_PIN)			 //����DATA������
#define SHT10_DATA_R()	GPIO_ReadInputDataBit(SHT10_DATA_PORT, SHT10_DATA_PIN)	 //��DATA������

#define SHT10_SCK_H()	GPIO_SetBits(SHT10_SCK_PORT, SHT10_SCK_PIN)				 //����SCKʱ����
#define SHT10_SCK_L()	GPIO_ResetBits(SHT10_SCK_PORT, SHT10_SCK_PIN)			 //����SCKʱ����

/* ��������غ궨�� */
#define	noACK	0
#define ACK		1
#define ERROR_CRC   -1								
                       //addr  command	 r/w
#define STATUS_REG_W	0x06	//000	 0011	  0	  д״̬�Ĵ���
#define STATUS_REG_R	0x07	//000	 0011	  1	  ��״̬�Ĵ���
#define MEASURE_TEMP 	0x03	//000	 0001	  1	  �����¶�
#define MEASURE_HUMI 	0x05	//000	 0010	  1	  ����ʪ��
#define SOFTRESET       0x1E	//000	 1111	  0	  ��λ


void SHT10_Config(void);
void sht10_conreset(void);
uint8_t SHT10_SoftReset(void);
int8_t sht10_measure(uint16_t *p_value, uint8_t *p_checksum, uint8_t mode);
void sht10_calculate(uint16_t t, uint16_t rh,float *p_temperature, float *p_humidity);
float sht10_calcu_dewpoint(float t, float h);
/*sht10 ��ʼ��*/
void sht10_init(void);
#endif

