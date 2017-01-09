#ifndef __ADC_H_
#define __ADC_H_
#include "stm32f10x.h"
#define N   50
#define M   4
/*adc��ʼ��*/
void adc_init(void);
/*��ƽ��ֵ����*/
void filter(void);
/*��ȡ ADC ��ֵ���������ƻ���Ϊʮ����*/
u16 GetVolt(u16 advalue);
/*��ȡadcֵ*/
uint16_t adc_val(uint8_t ch);

#endif
