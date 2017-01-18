#ifndef __Bsp_Flash_H
#define __Bsp_Flash_H
#include "common.h"

#define FLASH_SAVE_ADDR  0X08000000+0x000f000 	//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
#define FLASH_OffSAVE_ADDR  0X08000000+0x000f000

#define FLASH_ID_EXIST      0x21

extern int32_t OfficDatBuf[6];
extern uint32_t Flash_FifoBuf[256];

void Flash_WriteBufToPage (void);
void Flash_ReadPageToBUF (void);
uint8_t Search_SlaveID (uint32_t SlaveID);
uint8_t Add_SlaveID (uint32_t SlaveID);
void Flash_ReadOfficDat (void);
void Flash_WriteOfficDat (void);
void FlashInit (void);
  

#endif
