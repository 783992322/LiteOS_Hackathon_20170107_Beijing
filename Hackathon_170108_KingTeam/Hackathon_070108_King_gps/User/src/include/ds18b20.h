#ifndef __DS18B20_H__
#define __DS18B20_H__

#include"stm32f10x.h"
#include "stdint.h"

#define  SkipROM    0xCC     //??ROM
#define  SearchROM  0xF0  //??ROM
#define  ReadROM    0x33  //?ROM
#define  MatchROM   0x55  //??ROM
#define  AlarmROM   0xEC  //??ROM

#define  StartConvert    0x44  //??????,????????????0,???????1
#define  ReadScratchpad  0xBE  //?????9???
#define  WriteScratchpad 0x4E  //?????????TH?TL
#define  CopyScratchpad  0x48  //????????????EEPROM,??????????0,??????1
#define  RecallEEPROM    0xB8    //?EEPROM????????????,??????0,???????1
#define  ReadPower       0xB4    //????????:0???????;1???????

/*�˿ڶ���*/
#define DS_PORT   GPIOC
#define DS_DQIO   GPIO_Pin_6
#define DS_RCC_PORT  RCC_APB2Periph_GPIOC
/*���ȶ��壬 0x1f 9bit, 0x3f 10bit, 0x5f 11bit, 0x7f 12bit */
#define DS_PRECISION 0x7f  
/*�߾�����ַ*/
#define DS_AlarmTH  0x64
/*�;�����ַ*/
#define DS_AlarmTL  0x8a
#define DS_CONVERT_TICK 1000

/*IO��������*/
#define ResetDQ() GPIO_ResetBits(DS_PORT,DS_DQIO)
#define SetDQ()  GPIO_SetBits(DS_PORT,DS_DQIO)
#define GetDQ()  GPIO_ReadInputDataBit(DS_PORT,DS_DQIO)

#define ERROR_NO_DS18B20    0x55 //û�м�⵽�豸,����85��
#define ERROR_CRC_DS18B20   0x64 //������ζ�ȡ��CRC��CRCУ��ֵ���������˵����ȡ��ֵ�����⣬����100�棬����Ӧ����16���Ƶ���
#define OK                  0   //��������0
#define VAL_NEG             -1  //��ֵ�� -1
#define VAL_POS             1   //��ֵ�� 1
void ds18b20_init(void);
signed short ds18b20_read(void);
int16_t get_temperature(int16_t hex_val);
int16_t ResetDS18B20(void);
/*��ȡ�˲�����¶�ֵ*/
int16_t get_temperature_after_filter(void);
#endif
