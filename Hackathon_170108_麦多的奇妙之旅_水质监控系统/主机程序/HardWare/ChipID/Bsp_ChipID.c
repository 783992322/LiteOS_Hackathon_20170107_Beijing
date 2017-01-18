#include "common.h"

long ChipID = 0;          //оƬID��long�ͣ���һ�е����ַ���
char ChipID_String[25];   //24���ֽ�ID�����һ���ֽ��ǽ�����


/*
 * ��������GetChipID
 * ����  ����ȡоƬID
 * ����  ����
 * ����  ����
 */
void GetChipID (void)
{
  uint8_t i = 0, k = 0;
  u32 ChipUniqueID[3] = {0};
  
  ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);       //��ȡID
  ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC);
  ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0);
  ChipID = ChipUniqueID[0];

  /* ��long��ת��Ϊ�ַ��� */
  for (i = 0; i < 3; i++)
  {
    for (k = 0; k < 8; k++)
    {
      ChipID_String[i * 8 + k] = ((ChipUniqueID[i] << k * 4) >> 28);
      
      if (ChipID_String[i * 8 + k] <= 9)
        ChipID_String[i * 8 + k] += 0x30;
      else
      {
        switch(ChipID_String[i * 8 + k] - 9)
        {
          case 1:
            ChipID_String[i * 8 + k] = 'A';
            break;
          case 2:
            ChipID_String[i * 8 + k] = 'B';
            break;
          case 3:
            ChipID_String[i * 8 + k] = 'C';
            break;
          case 4:
            ChipID_String[i * 8 + k] = 'D';
            break;
          case 5:
            ChipID_String[i * 8 + k] = 'E';
            break;
          case 6:
            ChipID_String[i * 8 + k] = 'F';
            break;
          default:
            break;         
        }
      }
    }
  }
  
  ChipID_String[24] = '\0';   //�������ֶ���ӽ�����
}

/*
 * ��������Chip2String
 * ����  �����ӻ���IDת��Ϊ�ַ������
 * ����  ��Num��long��ID
 *         String���ַ���ID
 * ����  ����
 */
void Chip2String (uint32_t Num, char* String)
{
  uint8_t k;
  /* convert the num to string */

  for (k = 0; k < 8; k++)
  {
    *String = ((Num << k * 4) >> 28);
    
    if (*String <= 9)
      *String += 0x30;
    else
    {
      switch(*String - 9)
      {
        case 1:
          *String = 'A';
          break;
        case 2:
          *String = 'B';
          break;
        case 3:
          *String = 'C';
          break;
        case 4:
          *String = 'D';
          break;
        case 5:
          *String = 'E';
          break;
        case 6:
          *String = 'F';
          break;
        default:
          break;         
      }
    }
    String++;
  }
  *String = '\0';
}
