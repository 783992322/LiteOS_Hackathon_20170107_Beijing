#include "common.h"

int32_t OfficDatBuf[5] = {0};
uint32_t Flash_FifoBuf[256] = {0};                  //Flash������ϵ�BUF

void FlashInit (void)
{
  Flash_ReadOfficDat();
  if(OfficDatBuf[4] != 0xaaaaaaaa)
  {
    OfficDatBuf[0] = 0;
    OfficDatBuf[1] = 0;
    OfficDatBuf[2] = 0;
    OfficDatBuf[3] = 0;
    OfficDatBuf[4] = 0xaaaaaaaa;
    Flash_WriteOfficDat();
  }
}
  


void Flash_ReadOfficDat (void)
{
  int i =0;
  u32 faddr = FLASH_OffSAVE_ADDR;
  
  for (i = 0; i < 5; i++)
  {
    OfficDatBuf[i] = *(vu32*)faddr;
    faddr += 4;
  }
}

void Flash_WriteOfficDat (void)
{  
  int i = 0;
  uint32_t faddr = FLASH_OffSAVE_ADDR;
  
  FLASH_Unlock();				//����			
  FLASH_ErasePage(FLASH_OffSAVE_ADDR);
  for (i = 0; i < 256; i++)
  {
    FLASH_ProgramWord(faddr, OfficDatBuf[i]);	
    faddr += 4;
//    IWDG_Feed();
  }
  		
  FLASH_Lock();								//����
}

/*
 * ��������Flash_Clear
 * ����  ����Ҫ��Ҫ������Ŵӻ����������ݵ�ַ 
 * ����  ����
 * ����  ����
 */
//void Flash_Clear (void)
//{
//  FLASH_ErasePage(FLASH_SAVE_ADDR);
//}

/*
 * ��������Flash_Clear
 * ����  ��һ�ζ�64λ���� 
 * ����  ����
 * ����  ����
 */
//void Flash_ReadPageToBUF (void)
//{
//  int i =0;
//  u32 faddr = FLASH_SAVE_ADDR;
//  
//  for (i = 0; i < 256; i++)
//  {
//    Flash_FifoBuf[i] = *(vu32*)faddr;
//    faddr += 4;
//  }
//}


/*
 * ��������Flash_Clear
 * ����  ��������BUFд��Flash 
 * ����  ����
 * ����  ����
 */
//void Flash_WriteBufToPage (void)
//{
//  int i = 0;
//  uint32_t faddr = FLASH_SAVE_ADDR;
//  
//  FLASH_Unlock();				//����			
//  FLASH_ErasePage(FLASH_SAVE_ADDR);
//  for (i = 0; i < 256; i++)
//  {
//    FLASH_ProgramWord(faddr, Flash_FifoBuf[i]);	
//    faddr += 4;
//    IWDG_Feed();
//  }
//  		
//  FLASH_Lock();								//����
//}

/*
 * ��������Flash_Clear
 * ����  �������в������޴��û����� �����򷵻�����λ��
 * ����  ��SlaveID��Ҫ��ѯ��ID
 * ����  ������λ��
 */
uint8_t Search_SlaveID (uint32_t SlaveID)
{
  int SlaveNum = 0;
  int i = 0;
  
  SlaveNum = Flash_FifoBuf[0];    //��һ��32λ����ǵ�ǰ�ӻ�����
  for (i = 0; i < SlaveNum; i++)
  {
    if (SlaveID == (Flash_FifoBuf[7 + i * 3]))
    {
      return (7 + i * 3);
    }
  }
  return 0;
}

/*
 * ��������Flash_Clear
 * ����  �������в������޴��û����� �����򷵻�����λ��
 * ����  ��SlaveID��Ҫ��ӵ�ID
 * ����  ��SUCCESS���ɹ�
 *         FLASH_ID_EXIST������ӹ�
 */
uint8_t Add_SlaveID (uint32_t SlaveID)
{
  int statue = 0;
  int SlaveNum = 0;
  statue = Search_SlaveID(SlaveID);
  
  if (statue != 0)
  {
    return FLASH_ID_EXIST;
  }
  else
  {
    SlaveNum = Flash_FifoBuf[0];
    Flash_FifoBuf[7 + SlaveNum * 3] = SlaveID;
    Flash_FifoBuf[7 + SlaveNum * 3 + 1] = 0x0000;
    Flash_FifoBuf[7 + SlaveNum * 3 + 2] = 0x0000;
    Flash_FifoBuf[0] = ++SlaveNum;

    return SUCCESS;
  }
}
