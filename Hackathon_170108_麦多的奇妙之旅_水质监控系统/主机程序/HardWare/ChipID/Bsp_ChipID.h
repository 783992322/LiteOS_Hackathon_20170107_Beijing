#ifndef __Bsp_ChipID_H
#define __Bsp_ChipID_H
#include "common.h"

extern long ChipID;             //CPU ID
extern char ChipID_String[25];  //�ַ�����ʽ��ChipID  ���һ���ֽڴ�Ľ�����

void GetChipID (void);
//void Chip2String (uint32_t Num, char* String);

#endif
