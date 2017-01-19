#ifndef __Screen_H_
#define __Screen_H_
#include "common.h"

/* ��¼��ǰ��ʾ��ҳ�� */
#define HomePage            0     
#define DisDatPage          1
#define LimSetPage          2
#define TimeSetPage         3
#define TmpOfficeSetPage    4
#define PHOfficeSetPage     5

#define DisMastDat          0   //�����ʹӻ��Ĵ��������ݴ�ŷ�ʽ��һ�������ˢ����ʽҲҪ���⿼��
                                //���ǵ�����ƴ��λ��ʱ��������bug���Ժ��л������޸�һ��
#define DisMastTmpOff       1
#define DisMastPHOff        1

#define Acidity             1   //����
#define Neutral             2   //����
#define Basicity            3   //����

/* ���������� */
#define SensorEnvirTemp     0     
#define SensorWaterTemp     1
#define SensorWaterSpeed    2
#define SensorWaterLevel    3
#define SensorPH            4
#define SensorPower         5
#define SerialSerial        6
#define Time_UpDat          7

#define Color_Red           0
#define Color_Green         1
#define Color_Yellow        2
#define Color_Black         3
#define Color_Popul         4

#define PH_AN_Offset        1
#define PH_BN_Offset        2
#define Tmp_Offset          3

void Scr_SendCmd (uint8_t* str);
void ProBarGo (int val);
void GotoDisDatPage (void);
void FlashDatPage (void);
void FlashPage (void);
void ScrRest (void);
void GoLimPage (void);
void InitDis (char* str);
void FlashLimSetPage (uint8_t Key_Statue);
void FlashTimeSetPage (uint8_t Key_Statue);
void FlashTmpOffPage (uint8_t Key_Statue);
void FlashPHOffPage (uint8_t Key_Statue);
#endif
