#include "common.h"

//extern uint8_t Connect_Way;                      //ͨѶ��ʽ��־λ USE_CAN   USE_ZGB
long SlaveNeedAddBuf[MAX_SlaveNum] = {0};           //��Ҫ�ϴ����������Ĵӻ��б�

uint8_t TmpOffGetRec = FAILUED;                     //�ӻ��¶Ȳ������ûذ�
uint8_t PHOffGetRec_Neutral = FAILUED;              //�ӻ�PH���Ե����ûذ�
uint8_t PHOffGetRec_Acidity = FAILUED;              //���Իذ�
uint8_t PHOffGetRec_Basicity = FAILUED;             //���Իذ�

uint16_t WarmClearStatue = 0;     //�Ƿ����о������ѱ���
uint8_t WarmUpCount[MAX_SlaveNum + 1][6];

/*
 * ��������Can_ReceMsgProcess
 * ����  ��CAN�жϴ�����
 * ����  ��RxMessage�����ܵ�����Ϣ
 * ����  ����
 */
void Can_ReceMsgProcess (CanRxMsg* RxMessage)
{
//  uint8_t i = 0;
//  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
//  uint8_t WaterLevel = 0;         //ˮλ
//  float WaterSpeed = 0.0;         //����
//  float EnvirTemp = 0.0;          //����
//  float ph = 0.0;                 //ph
//  float WaterTemp = 0.0;          //ˮ��
//  
//  uint8_t statue = 0;             //��ʱ״̬��־λ
//  uint8_t SlaveSerial = 0;        //��Falsh�в����豸ʱ������кŵı���
//  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN�����õ�����
//  long ChipID = 0;
//  
//  uint8_t statueFromServer = 0;
//  uint8_t ErrorCount = 0;
//  uint8_t SlaveSearchCount = 0;

//  
//  /* ��ȡ���ڵĴӻ�ID */
//  ChipID = ((RxMessage->Data[1] << 24) + (RxMessage->Data[2] << 16) \
//                    + (RxMessage->Data[3] << 8) + (RxMessage->Data[4]));
//  
//  /* ���յ������������� */
//  if ((RxMessage->Data[6] == 0xaa) && (RxMessage->Data[7] == 0xaa)) 
//  {    
//    Can_DatBuf[1] = RxMessage->Data[1];           //��IDд��ط�����
//    Can_DatBuf[2] = RxMessage->Data[2];
//    Can_DatBuf[3] = RxMessage->Data[3];
//    Can_DatBuf[4] = RxMessage->Data[4];
//    Can_DatBuf[6] = 0xaa;                         //��ʾ���������йص����ݰ�
//    Can_DatBuf[7] = 0xaa;
//    
//    switch(RxMessage->Data[5])                    //�ж�����������������ְ�
//    {
//      case 0xaa:                                  //��������
//        {
//          statue = Add_SlaveID(ChipID);           //��ID�����Flash�У�����Ѵ��� �����ڲ������
//          SlaveSerial = Search_SlaveID(ChipID);   //��ȡ�����豸��Flash�д�ŵ�λ�����
//          
//          switch(statue)              
//          {
//            case SUCCESS:                         //��Ӵӻ��ɹ����ȴ���������һ��ȷ��
//              {
//                /* 
//                 *  ����Ӧ�ûط�0x01��ʾ���������룬�ȴ�������ȷ��
//                 *  ������Э������Щ�޸ģ���Щ��֤����޹ؽ�Ҫ�����
//                 *  ֱ�ӻط�0x03��ʾ�����ɹ�
//                */
//                Can_DatBuf[5] = 0x03;                             //�����ɹ�
//                Can_Send_Msg(Can_DatBuf, 8);                      //��ӻ��ظ�����
////                Flash_FifoBuf[SlaveSerial + 1] |= (1 << 31);    //��λ�ѽ��������־λ���������ȡ����
////                Flash_WriteBufToPage();         //��BUFд��Flash�и��£�����ȡ���ˣ��������Flash�ᵼ��һЩ�����ڵĽ������
//                if((((Flash_FifoBuf[SlaveSerial + 1] >> 31) & 0x01) == 0) &&           //�豸δ�����δ���������������
//                    (((Flash_FifoBuf[SlaveSerial + 1] >> 30) & 0x01) == 0))
//                {
//                  SlaveNeedAddBuf[0] += 1;    //�����һλ��ʾ��ǰ����ӵĴӻ�����
//                  SlaveNeedAddBuf[SlaveNeedAddBuf[0]] = ChipID;
//                }
//                
//                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//                if (SlaveSearchCount == (SlaveLostCount + 1))
//                  Beep_ON_Nop(100);
//                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //����ʧ��־λ���
//                
//                Beep_ON_Nop(100);                  //��ʾһ����ʾ�豸��ӳɹ�
//                FlashPage();                       //ˢ����ʾҳ��
//              }
//              break;
//            case FLASH_ID_EXIST:                  //�ôӻ������Ѿ��ύ��
//              {
//                Can_DatBuf[5] = 0x02;             //�������ύ�������Ѵӻ���Ҫ�ٷ���������Ϣ
//                Can_Send_Msg(Can_DatBuf, 8);      //��ӻ��ظ�����
//                
//                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//                if (SlaveSearchCount == (SlaveLostCount + 1))
//                  Beep_ON_Nop(100);
//                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //����ʧ��־λ���
//              }
//              break;
//            default:
//              break;      
//          }    
//        }
//        break;
//      case 0x01:                                  //����Ƕ�������ɵĻظ�
//        {
//          /*
//           *  ������Э�������֮������ϲ�ʵ���������Щ�޸�
//           *  ����ظ���ò���Ҫ�����ط���Ӱ��ӻ���������ֻ��
//           *  ���ǵ��Ժ�Թ��ܵ�������鲢û��ɾ����ֻ�ǲ���
//           *  ����
//           */
////          Flash_ReadPageToBUF();                      //����һ��BUF��
////          statue = Search_SlaveID(ChipID);            //�ҵ�����ӻ����ݴ������λ��
////          Flash_FifoBuf[statue + 1] |= 0x80000000;    //������λ�����λ��1����ʾ����ӻ��������Ѿ���ͨ��
////          Flash_WriteBufToPage();                     //�����º������д��Flash
////          Flash_FifoBuf[SlaveSerial] &= ~(1 << 31);
//        }
//        break;
//      default:
//        break;
//    }
//  }
//  /* ���յ��ɼ����ݷ��� */
//  if ((RxMessage->Data[7] == 0x01) || (RxMessage->Data[7] == 0x02) || (RxMessage->Data[7] == 0x03))
//  {
//    RxMessage->Data[5]--;   //�Լ���Ϊ��CAN�����ZGB��һ�£������ڷ���ǰ������һ�Σ�ԭ���ڴӻ���ZGB��������д
//    RxMessage->Data[6]--;

//    /* ���ҵ���ֵ */
//    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
//    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
//    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
//    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
//    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
//    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
//    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);

//    
//    SlaveSerial = Search_SlaveID(ChipID);   //��ȡ�����豸��Flash�д�ŵ�λ�����
//    switch (RxMessage->Data[7])             //�ж������ݵĵڼ�������CANһ�����ݷ�Ϊ������
//    {
//      case 0x01:
//            /* PHֵ��� */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 12);                 //�����λ��
//            Flash_FifoBuf[SlaveSerial + 1] |= (RxMessage->Data[5] << 12);  
//      
//            /* ˮ�´�� */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 4); 
//            Flash_FifoBuf[SlaveSerial + 1] |= (RxMessage->Data[6] << 4);     //��������
//        break;
//      case 0x02:
//            /* ���´�� */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 24);
//            Flash_FifoBuf[SlaveSerial + 2] |= (RxMessage->Data[5] << 24);                 //��������
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 20);
//            Flash_FifoBuf[SlaveSerial + 2] |= ((RxMessage->Data[6] & 0xf) << 20);         //С������
//            /* ˮ�´�� */
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 0); 
//            Flash_FifoBuf[SlaveSerial + 1] |= (((RxMessage->Data[6] >> 5) & 0xf) << 0);   //С������
//        break;
//      case 0x03:
//            /* ���ٴ�� */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 12);
//            Flash_FifoBuf[SlaveSerial + 2] |= (RxMessage->Data[5] << 12);                 //��������
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 8);
//            Flash_FifoBuf[SlaveSerial + 2] |= (((RxMessage->Data[6] >> 5) & 0xf) << 8);   //С������
//            /* ˮλ��� */
//            Flash_FifoBuf[SlaveSerial + 2] &= ~(0x1 << 7);
//            Flash_FifoBuf[SlaveSerial + 2] |= (((RxMessage->Data[6] >> 3) & 0x01) << 7);  //ˮλֻ��һλ    
//      
//      
//            /* ��ȡ���յ��Ĳɼ����� */
//            WaterLevel = ((Flash_FifoBuf[SlaveSerial + 2] >> 7) & 0x01);
//            WaterSpeed = ((Flash_FifoBuf[SlaveSerial + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 8) & 0xf) / 10.0);
//            EnvirTemp = ((Flash_FifoBuf[SlaveSerial + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 20) & 0xf) / 10.0);
//            ph = (((Flash_FifoBuf[SlaveSerial + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveSerial + 1] >> 12) & 0xf) / 10.0);
//            WaterTemp = (((Flash_FifoBuf[SlaveSerial + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveSerial + 1] >> 0) & 0xf) / 10.0);
//            
//            for (i = 1; i <= Flash_FifoBuf[0]; i++)
//              if (ChipID == (Flash_FifoBuf[7 + i * 3]))
//                break;
//              
//            /* �ж�ˮλ�Ƿ���Ҫ���� */
//            if (!WaterLevel)                                                    //ˮλ����
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 24);                      //��λ������־λ
//            else                                                                //ˮλ�ָ�����
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 24) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 24);                   //����־λ���
//              WarmUpCount[i][3] = 0;
//            }
//              
//            /* �ж������Ƿ���Ҫ���� */
//            if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))   //���ٱ���
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 22);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 22) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 22);                   //��������
//              WarmUpCount[i][2] = 0;
//            }
//              
//            
//            /* �ж������Ƿ���Ҫ���� */
//            if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))       //���±���
//              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 20);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 1] >> 20) & 0x01)
//                WarmClearStatue++;
//              
//              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 20);                   //��������
//              WarmUpCount[i][0] = 0;
//            }
//              
//            /* �ж�PH�Ƿ���Ҫ���� */
//            if ((ph > PHMax) || (ph < PHMin))                                   //PH����
//              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 5);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 2] >> 5) & 0x01)
//                WarmClearStatue++;
//              
//              WarmUpCount[i][4] = 0;
//              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 5);                    //PH����
//            }
//              
//            /* �ж�ˮ���Ƿ���Ҫ���� */
//            if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))       //ˮ�±���
//              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 3);
//            else
//            {
//              if ((Flash_FifoBuf[SlaveSerial + 2] >> 3) & 0x01)
//                WarmClearStatue++;
//              
//              WarmUpCount[i][1] = 0;
//              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 3);                    //ˮ������
//            }
//              
//      
//            SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
//            if (SlaveSearchCount == (SlaveLostCount + 1))
//              Beep_ON_Nop(100);
//            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //����ʧ��־λ���
//            
//            FlashPage();        //����һ�����ݺ�ˢ��ҳ��
//    
//        break;
//      default:
//        break;
//    }
//  }
//  if (RxMessage->Data[7] == 0xcc)   //������¶Ȳ����Ļذ�
//  {
//    TmpOffGetRec = SUCCESS;
//  }
//  switch (RxMessage->Data[7])
//  {
//    case 0xd0:
//      PHOffGetRec_Neutral = SUCCESS;
//      break;
//    case 0xd1:
//      PHOffGetRec_Acidity = SUCCESS;
//      break;
//    case 0xd2:
//      PHOffGetRec_Basicity = SUCCESS;
//      break;
//    default:
//      break;
//  }
}


/*
 * ��������ZGB_ReceMsgProcess
 * ����  ��ZGB�жϴ�����
 * ����  ��Uart2_ReceiveDat�����ܵ�����Ϣ
 * ����  ����
 */
void ZGB_ReceMsgProcess (uint8_t Uart2_ReceiveDat[])
{
  uint8_t i = 0;
  uint8_t SlaveSerial = 0;        //��Falsh�в����豸ʱ������кŵı���
  uint8_t WaterLevel = 0;         //ˮλ
  float WaterSpeed = 0.0;         //����
  float EnvirTemp = 0.0;          //����
  float ph = 0.0;                 //ph
  float WaterTemp = 0.0;          //ˮ��
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //�ط��İ�
  long ChipID = 0;
  uint8_t statue = 0;   //, statueFromServer = 0
//  uint8_t ErrorCount = 0;
  uint8_t SlaveSearchCount = 0;
  
  ChipID = ((Uart2_ReceiveDat[1] << 24) + (Uart2_ReceiveDat[2] << 16) \
                    + (Uart2_ReceiveDat[3] << 8) + (Uart2_ReceiveDat[4]));
  
  /* �������� */
  if ((Uart2_ReceiveDat[12] == 0xaa) && (Uart2_ReceiveDat[13] == 0xaa)) 
  {
    ZGB_DatBuf[1] = Uart2_ReceiveDat[1];
    ZGB_DatBuf[2] = Uart2_ReceiveDat[2];
    ZGB_DatBuf[3] = Uart2_ReceiveDat[3];
    ZGB_DatBuf[4] = Uart2_ReceiveDat[4];
    ZGB_DatBuf[12] = 0xaa;                        //��ʾ���������йص����ݰ�
    ZGB_DatBuf[13] = 0xaa;
    
    switch(Uart2_ReceiveDat[11])                  //�鿴�������������������ְ�
    {
      case 0xaa:                                  //��������
        {
          statue = Add_SlaveID(ChipID);
          SlaveSerial = Search_SlaveID(ChipID);   //��ȡ�����豸��Flash�д�ŵ�λ�����
          switch(statue)
          {
            case SUCCESS:                         //��Ӵӻ��ɹ����ȴ���������һ��ȷ��
              {
                /* 
                 *  ���������Э�飬����Ӧ�ûط�0x01��ʾ�ѽ������뵫����Ҫ������ͨ��
                 *  �ط�0x03��ʾ��������ͨ�����룬���Ǹ���ʵ����������޸�
                */
                ZGB_DatBuf[11] = 0x03;             //���������룬�ȴ�������ȷ��
                
                UserDelay_KeyScan(2000);                    //ZGB�е����⣬�ս�������Ϣ���̻ط�������ȥ����˵ȴ�һ��
                USART2_SendBuf(ZGB_DatBuf);        //��ӻ��ظ�����
                USART2_SendByte('\n');             //ZGB���ݻس����ж�һ֡�����Ƿ����
                
                if((((Flash_FifoBuf[SlaveSerial + 1] >> 31) & 0x01) == 0) &&           //�豸δ�����δ���������������
                    (((Flash_FifoBuf[SlaveSerial + 1] >> 30) & 0x01) == 0))
                {
                  SlaveNeedAddBuf[0] += 1;    //�����һλ��ʾ��ǰ����ӵĴӻ�����
                  SlaveNeedAddBuf[SlaveNeedAddBuf[0]] = ChipID;
                }
                
                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
                if (SlaveSearchCount == (SlaveLostCount + 1))
                  Beep_ON_Nop(100);
                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //����ʧ��־λ���
                
                Beep_ON_Nop(100);                  //��ʾһ����ʾ�豸��ӳɹ�
                FlashPage();                       //ˢ����ʾҳ��
              }
              break;
            case FLASH_ID_EXIST:                  //�ôӻ������Ѿ��ύ��
              {
                UserDelay_KeyScan(2000);
                ZGB_DatBuf[11] = 0x02;            //�������ύ�������Ѵӻ���Ҫ�ٷ���������Ϣ
                USART2_SendBuf(ZGB_DatBuf);       //��ӻ��ظ�����
                USART2_SendByte('\n');
                
                SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
                if (SlaveSearchCount == (SlaveLostCount + 1))
                  Beep_ON_Nop(100);
                Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);   //����ʧ��־λ���
              }
              break;
            default:
              break;      
          }    
        }
        break;
     case 0x01:                                   //����Ƕ�������ɵĻظ�
        {
        }
       break;
     default:
       break;
    }
  }
  
  /* �ɼ������ݷ��� */
  if ((Uart2_ReceiveDat[12] == 0xbb) && (Uart2_ReceiveDat[13] == 0xbb)) 
  {    
    /* ���ҵ���ֵ */
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    
    switch(Uart2_ReceiveDat[11])    //�鿴�����ǲɼ�����������ְ���Ŀǰֻд��һ�֣�����д������ڼӹ���
    {
      case 0x01:
        {
          SlaveSerial = Search_SlaveID(ChipID);   //��ȡ�����豸��Flash�д�ŵ�λ�����
          if (SlaveSerial != 0)                   //���Flash�д��ڴ�ID
          {                                       
            Uart2_ReceiveDat[5]--;
            Uart2_ReceiveDat[6]--;
            Uart2_ReceiveDat[7]--;
            Uart2_ReceiveDat[8]--;
            Uart2_ReceiveDat[9]--;
            Uart2_ReceiveDat[10]--;
            
            /* PHֵ��� */
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 12);                 
            Flash_FifoBuf[SlaveSerial + 1] |= (Uart2_ReceiveDat[5] << 12);  
            
            /* ˮ�´�� */
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xff << 4); 
            Flash_FifoBuf[SlaveSerial + 1] |= (Uart2_ReceiveDat[6] << 4);    
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 0); 
            Flash_FifoBuf[SlaveSerial + 1] |= (((Uart2_ReceiveDat[8] >> 4) & 0xf) << 0);  
            
            /* ���´�� */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 24);
            Flash_FifoBuf[SlaveSerial + 2] |= (Uart2_ReceiveDat[7] << 24);   
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 20);
            Flash_FifoBuf[SlaveSerial + 2] |= ((Uart2_ReceiveDat[8] & 0xf) << 20);  
            
            /* ���ٴ�� */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xff << 12);
            Flash_FifoBuf[SlaveSerial + 2] |= (Uart2_ReceiveDat[9] << 12);  
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0xf << 8);
            Flash_FifoBuf[SlaveSerial + 2] |= (((Uart2_ReceiveDat[10] >> 4) & 0xf) << 8);  
            
            /* ˮλ��� */
            Flash_FifoBuf[SlaveSerial + 2] &= ~(0x1 << 7);
            Flash_FifoBuf[SlaveSerial + 2] |= (((Uart2_ReceiveDat[10] >> 3) & 0x01) << 7);   
            
            UserDelay_KeyScan(500);
            ZGB_DatBuf[1] = Uart2_ReceiveDat[1];
            ZGB_DatBuf[2] = Uart2_ReceiveDat[2];
            ZGB_DatBuf[3] = Uart2_ReceiveDat[3];
            ZGB_DatBuf[4] = Uart2_ReceiveDat[4];
            ZGB_DatBuf[11] = 0x02;
            ZGB_DatBuf[12] = 0xbb;              //��ʾ���������йص����ݰ�
            ZGB_DatBuf[13] = 0xbb;
            USART2_SendBuf(ZGB_DatBuf);         //��ӻ��ظ��Ѿ��յ�����
            USART2_SendByte('\n');
            
            WaterLevel = ((Flash_FifoBuf[SlaveSerial + 2] >> 7) & 0x01);
            WaterSpeed = ((Flash_FifoBuf[SlaveSerial + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 8) & 0xf) / 10.0);
            EnvirTemp = ((Flash_FifoBuf[SlaveSerial + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveSerial + 2] >> 20) & 0xf) / 10.0);
            ph = (((Flash_FifoBuf[SlaveSerial + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveSerial + 1] >> 12) & 0xf) / 10.0);
            WaterTemp = (((Flash_FifoBuf[SlaveSerial + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveSerial + 1] >> 0) & 0xf) / 10.0);
            
            for (i = 1; i <= Flash_FifoBuf[0]; i++)
              if (ChipID == (Flash_FifoBuf[7 + i * 3]))
                break;
            
            
            /* ˮλ */
            if (WaterLevel)                                                    //ˮλ����
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 24);
            else                                                                //ˮλ�ָ�����
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 24) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 24);
              
              WarmUpCount[i][3] = 0;
            }
              
            /* ���� */
            if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))   //���ٱ���
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 22);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 22) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 22);                   //��������
              
              WarmUpCount[i][2] = 0;
            }
              
            /* ���� */
            if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))       //���±���
              Flash_FifoBuf[SlaveSerial + 1] |= (1 << 20);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 1] >> 20) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 1] &= ~(0x3 << 20);                   //��������
              
              WarmUpCount[i][0] = 0;
            }
              
            
            /* PH */
            if ((ph > PHMax) || (ph < PHMin))                                   //PH����
              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 5);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 2] >> 5) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 5);                    //PH����
              
              WarmUpCount[i][4] = 0;
            }
              
            
            /* ˮ�� */
            if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))       //ˮ�±���
              Flash_FifoBuf[SlaveSerial + 2] |= (1 << 3);
            else
            {
              if ((Flash_FifoBuf[SlaveSerial + 2] >> 3) & 0x01)
                WarmClearStatue++;
              
              Flash_FifoBuf[SlaveSerial + 2] &= ~(0x3 << 3);                    //ˮ������
              
              WarmUpCount[i][1] = 0;
            }
              
            SlaveSearchCount = ((Flash_FifoBuf[SlaveSerial + 1] >> 26) & 0xf);
            if (SlaveSearchCount == (SlaveLostCount + 1))
              Beep_ON_Nop(100);
            Flash_FifoBuf[SlaveSerial + 1] &= ~(0xf << 26);
            
            FlashPage();                                                        //ˢ��ҳ��
          }
        }
        break;
      default:
        break;
    }   
  }
  if ((Uart2_ReceiveDat[12] == 0xcc) && (Uart2_ReceiveDat[13] == 0xcc))   //����Ǳ궨�Ļظ���
  {
    switch(Uart2_ReceiveDat[11])
    {
      case 0x01:
        TmpOffGetRec = SUCCESS;
        
        break;
      case 0x02:
          switch (Uart2_ReceiveDat[10])
          {
            case 0x01:
              PHOffGetRec_Neutral = SUCCESS;
              break;
            case 0x02:
              PHOffGetRec_Acidity = SUCCESS;
              break;
            case 0x03:
              PHOffGetRec_Basicity = SUCCESS;
              break;
            default:
              break;
          }
        break;
      default:
        break;
    }
      
  }    
}

/*
 * ��������TouchSlaveProcess_ZGB
 * ����  ��ͨ��ZGB��ӻ���Ҫ�ɼ���������
 * ����  ����
 * ����  ����
 */
void TouchSlaveProcess_ZGB (void)
{
  static int i = 1;
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  int SearchCount = 0;
  
  if (Flash_FifoBuf[0] >= 1)
  {
//    SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
//    if (SearchCount < SlaveLostCount)   //������豸δ��ʧ
//    {
      ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 24) & 0xff);
      ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 16) & 0xff);
      ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 8) & 0xff);
      ZGB_DatBuf[4] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 0) & 0xff);
      ZGB_DatBuf[11] = 0x01;
      ZGB_DatBuf[12] = 0xbb;
      ZGB_DatBuf[13] = 0xbb;
      USART2_SendBuf(ZGB_DatBuf);          //��ӻ�����
      USART2_SendByte('\n');
    
      SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
      if (SearchCount < SlaveLostCount)
      {
        SearchCount++;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0xf << 26);                 //��ʧ������1  ������ܵ���Ϣ����յ�
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= ((SearchCount & 0xf) << 26);
      }
      
//    }
    
    i++;
    if (i > Flash_FifoBuf[0])
      i = 1;
  }
  
}

/*
 * ��������TouchSlaveProcess_CAN
 * ����  ��ͨ��CAN��ӻ���Ҫ�ɼ���������
 * ����  ����
 * ����  ����
 */
void TouchSlaveProcess_CAN (void)
{
  static int i = 1;
  int SearchCount = 0;
  u8 CAN_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  

  CAN_DatBuf[1] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 24) & 0xff);
  CAN_DatBuf[2] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 16) & 0xff);
  CAN_DatBuf[3] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 8) & 0xff);
  CAN_DatBuf[4] = ((Flash_FifoBuf[7 + (i - 1) * 3] >> 0) & 0xff);
  CAN_DatBuf[5] = 0x01;
  CAN_DatBuf[6] = 0xbb;
  CAN_DatBuf[7] = 0xbb;
  Can_Send_Msg(CAN_DatBuf, 8);
  
  SearchCount = ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 26) & 0xf);
  if (SearchCount < SlaveLostCount)
  {
    SearchCount++;
    Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0xf << 26);                 //��ʧ������1  ������ܵ���Ϣ����յ�
    Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= ((SearchCount & 0xf) << 26);
  }
  
  i++;
  if (i > Flash_FifoBuf[0])
    i = 1;
}

/*
 * ��������Test_AllowInNet
 * ����  ������ʱд�Ĳ��Ժ���
 * ����  ����
 * ����  ����
 */
//void Test_AllowInNet (void)
//{
//  u8 CAN_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// 
//  CAN_DatBuf[1] = ((Flash_FifoBuf[6] >> 24) & 0xff);
//  CAN_DatBuf[2] = ((Flash_FifoBuf[6] >> 16) & 0xff);
//  CAN_DatBuf[3] = ((Flash_FifoBuf[6] >> 8) & 0xff);
//  CAN_DatBuf[4] = (Flash_FifoBuf[6] & 0xff);
//  CAN_DatBuf[5] = 0x03;               //�������ύ�������Ѵӻ���Ҫ�ٷ���������Ϣ
//  CAN_DatBuf[6] = 0xaa;
//  CAN_DatBuf[7] = 0xaa;
//  Can_Send_Msg(CAN_DatBuf, 8);
//}



void task_GetSlaveDat (void)
{
  UINT32 uwRet = LOS_OK;
  
  while(1)
  {
    if (Connect_Way == USE_CAN)
      TouchSlaveProcess_CAN();
    else
      TouchSlaveProcess_ZGB();
    
//    USART1_SendBuf ("SlaveDatGet_Task\r\n");
    
    uwRet = LOS_TaskDelay(1000 * TouchSlaveTime);//ϵͳ��ʱ1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 g_GetSlaveDatTskHandle;

UINT32 creat_GetSlaveDatTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;                   //�������ȼ���0���
	task_init_param.pcName = "SlaveDat";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_GetSlaveDat;//������ں���
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;//�����ջ��С
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_GetSlaveDatTskHandle,&task_init_param);//��������
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}


