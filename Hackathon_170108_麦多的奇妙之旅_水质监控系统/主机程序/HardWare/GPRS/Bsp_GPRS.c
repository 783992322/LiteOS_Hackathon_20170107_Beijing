#include "common.h"

uint8_t TCPIsUsed = 0;            //TCP�Ƿ����ڱ�ʹ�ñ�־λ
uint8_t GetNet = 0;               //�Ƿ����ӵ������־λ
//uint16_t WarmClearStatue = 0;     //�Ƿ����о������ѱ���

/*
 * ��������GetNetTime
 * ����  ����ȡ����ʱ��
 * ����  ����
 * ����  ����
 */
void GetNetTime (void)
{
  char* BufPoint = 0;
  int year = 2000, month = 0, day = 0, hour = 0, minute = 0, second = 0;;
  
  UserDelay_KeyScan(100);
  BufPoint = strstr(Uart3_ReceiveDat, "UTC(NIST)");
  BufPoint -= 31;
  
//  USART3_SendBuf((uint8_t*)BufPoint);
  
  year += (((BufPoint[0] - 0x30) * 10) + (BufPoint[1] - 0x30));
  month = (((BufPoint[3] - 0x30) * 10) + (BufPoint[4] - 0x30));
  day = (((BufPoint[6] - 0x30) * 10) + (BufPoint[7] - 0x30));
  hour = (((BufPoint[9] - 0x30) * 10) + (BufPoint[10] - 0x30));
  minute = (((BufPoint[12] - 0x30) * 10) + (BufPoint[13] - 0x30));
  second = (((BufPoint[15] - 0x30) * 10) + (BufPoint[16] - 0x30));

  hour += 8;
  if (hour > 23)
  {
    hour -= 24;
    day++;
    if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
    {
      day = 0;
      month++;
      if (month == 13)
      {
        year++;
        month = 1;
      }
    }
    
    if (((month == 1) || (month == 3) || (month == 5) || (month == 7) 
          || (month == 8) || (month == 10) || (month == 12)) && (day == 32))
    {
      day = 0;
      month++;
      if (month == 13)
      {
        year++;
        month = 1;
      }
    }
  }
  
  if (month == 13)
    month = 1;
  calendar.w_year = year;
  calendar.w_month = month;
  calendar.w_date = day;
  calendar.hour = hour;
  calendar.min = minute;
  calendar.sec = second;
  
  
}

//uint8_t UpTmpoffset2Server (uint8_t* SlaveChipIDString)
//{
//  uint8_t ErrorCount = 0;
//  uint8_t TimeDelayCount = 0;
//  uint8_t MsgSendBuf[300];
//  sprintf((char*)MsgSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveChipIDString);
//  
//  do
//  {
//    UserDelay_KeyScan(1000);
//    Uart3_BufClear();  
//    USART3_SendBuf(MsgSendBuf);  
//    TimeDelayCount = 0;
//    while(TimeDelayCount < 40)
//    {
//      UserDelay_KeyScan(100);
//      TimeDelayCount++;
//      if (strstr(Uart3_ReceiveDat, "true") != NULL)
//        return SUCCESS;  
//    } 
//    ErrorCount++;
//    if (ErrorCount > 3)
//    {
//      return ErrorFromServe;
//    }
//  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
//  return SUCCESS;  
//}

/*
 * ��������GetOffDatProcess
 * ����  ���Է������˻�õ�PH���¶Ȳ������ݽ��д���
 * ����  ����
 * ����  ����
 */
float GetOffDatProcess (uint8_t Index)
{
  char* position;
  char* TempPosition;
  uint8_t length = 0;
  float Num = 0.0;
  position = (strstr(Uart3_ReceiveDat, "value") + 7);
    
  TempPosition = (strstr(position, "."));
  length = TempPosition - position;
  
  if (*position == '-')                   //����Ǹ�����
  {
    position++;     //ָ��������һλ���ѷ����˹�
    length--;       //�������ȼ�1��֮ǰ�����˷���
  }
  
  switch (Index)
  {
    case PH_AN_Offset:
    case PH_BN_Offset:
      switch(length)        //�ж���������
      {
        case 1:
          Num = position[0] - 0x30;
          break;
        case 2:
          Num = (position[0] - 0x30) * 10 + (position[1] - 0x30);
          break;
        case 3:
          Num = (position[0] - 0x30) * 100 + (position[1] - 0x30) * 10 + (position[2] - 0x30);
          break;
        case 4:
          Num = (position[0] - 0x30) * 1000 + (position[1] - 0x30) * 100 + (position[2] - 0x30) * 10 + (position[3] - 0x30);
          break;
        case 5:
          Num = (position[0] - 0x30) * 10000 + (position[1] - 0x30) * 1000 + (position[2] - 0x30) * 100 + (position[3] - 0x30) * 10 + (position[4] - 0x30);
          break;
      }
      /* ��������ս�� */
      Num += (float)((position[length + 1] - 0x30) / 10.0 + (position[length + 2] - 0x30) / 100.0 
            + (position[length + 3] - 0x30) / 1000.0 + (position[length + 4] - 0x30) / 10000.0
            + (position[length + 5] - 0x30) / 100000.0 + (position[length + 6] - 0x30) / 1000000.0 
            + (position[length + 7] - 0x30) / 10000000.0 + (position[length + 8] - 0x30) / 100000000.0);
          break;
    case Tmp_Offset:
      switch (length)
      {
        case 1:
          Num = (float)((position[0] - 0x30) + (float)((position[2] - 0x30) / 10.0));
          break;
        case 2:
          Num = (float)(((position[0] - 0x30) * 10) + (position[1] - 0x30) + ((position[3] - 0x30) / 10.0));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
  if (*(position - 1) == '-')
              Num = -Num;
  
  return Num;
}


/*
 * ��������GetOffsetDat
 * ����  ���ӷ������˻�ȡPHб�ʼ��¶Ȳ�������
 * ����  ����
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t GetOffsetDat (void)
{
  float num = 0.0;
  uint8_t statue, i;
  uint8_t SendBuf[50];
  uint8_t MsgLength = 0;  
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;

  for (i = 1; i <= 3; i++)
  {
    sprintf ((char *)SendBuf, "{\"type\":\"number\",\"number\":0,\"value\":%d}\r\n", i);
    MsgLength = strlen((char *)SendBuf);
    
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
      
    do
    {
      USART3_SendBuf(SendBuf); 
      Delay_ms(100);
      Uart3_BufClear();  
      while(TimeDelayCount < 40)
      {
        Delay_ms(100);
        TimeDelayCount++;
        if (strstr(Uart3_ReceiveDat, ".") != NULL)
          break;
      }
      ErrorCount++;
      if (ErrorCount > 3)
      {
        return ErrorFromServe;
      }
    }while(strstr(Uart3_ReceiveDat, ".") == NULL);
    num = GetOffDatProcess(i);
    
    switch (i)
    {
      case PH_AN_Offset:
        OfficDatBuf[3] = num * 1000000;
        break;
      case PH_BN_Offset:
        OfficDatBuf[4] = num * 1000000;
        break;
      case Tmp_Offset:
        OfficDatBuf[0] = (int)(num * 10);
        break;
      default:
        break;
    }
  
  } 
  
  return SUCCESS;
}
  

/*
 * ��������UpOffsetDat
 * ����  ������������ϴ�PHб�ʼ��¶Ȳ�������
 * ����  ��Index: 
 *            PH_AN_Offset��PH����б��
 *            PH_BN_Offset��PH����б��
 *            Tmp_Offset���¶Ȳ���ֵ
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t UpOffsetDat (uint8_t Index, float Offset)
{
  uint8_t statue;
  uint8_t SendBuf[50];
  uint8_t MsgLength = 0;  
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  
  sprintf ((char *)SendBuf, "{\"type\":\"number\",\"number\":%d,\"value\":%f}\r\n",Index, Offset);
  MsgLength = strlen((char *)SendBuf);
  
  /* ��A6���뷢��TCP��Ϣ */
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
    return statue;
  
  do
  {
    Uart3_BufClear();  
    USART3_SendBuf(SendBuf); 
    while(TimeDelayCount < 40)
    {
      Delay_ms(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        return SUCCESS; 
    }
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);


  return SUCCESS;
}

/*
 * ��������UpLostMsg
 * ����  ���ϱ��ӻ���ʧ��Ϣ
 * ����  ��SlaveID����ʧ�Ĵӻ�ID
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t UpLostMsg (uint8_t* SlaveID)
{
  uint8_t statue;
  uint8_t ReqSendBuf[100];
  uint8_t SalveIDStrBuf[9];
  int MsgLength = 0;  
  
  
  sprintf ((char *)ReqSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveID);
  MsgLength = strlen((char *)ReqSendBuf);
  
  /* ��A6���뷢��TCP��Ϣ */
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
    return statue;

  /* ������Ӵӻ��� */
  statue = UpSlaveLost2Server(SlaveID);
  if (statue != SUCCESS)
    return statue;
  
  return SUCCESS;
}

/*
 * ��������Bsp_Init
 * ����  ���豸��
 * ����  ����
 * ����  ����
 */
uint8_t DevBound (void)
{
  uint8_t ReqSendBuf[100];
  uint8_t statue = 0;
  int MsgLength = 0;
  uint8_t ErrorCount;
  uint16_t Delay_TimeCount;
  
  while(TCPIsUsed);
  if (TCPIsUsed == 0)
  {
    TCPIsUsed = 1;
    TIM_Cmd(TIM2, DISABLE);		
    
    Beep_ON_Nop(100);                                             //��ʾ��ʼ���Ͱ���Ϣ
    sprintf ((char *)ReqSendBuf, "{\"type\":\"add2user\",\"add2user\":\"%s\"}\r\n", ChipID_String);
    MsgLength = strlen((char *)ReqSendBuf);                       //���㳤��
    
    ErrorCount = 0;    
    do
    {
      statue = ReqSendTcp(MsgLength);
      if (statue != SUCCESS)
      {
        TCPIsUsed = 0;
        return statue;
      }
      Uart3_BufClear();  
      UserDelay_KeyScan(1500);
      USART3_SendBuf(ReqSendBuf);  
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 200)
      {
        UserDelay_KeyScan(100);
        Delay_TimeCount++;
        if (strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":true") != NULL)
          break;
        if (strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":false") != NULL)
        {                            
          Beep_ON_Nop(500);   //������ʾ����ʾ��ʧ��
          Beep_ON_Nop(500);
          TCPIsUsed = 0;
          return FAILUED;     
        }
      }     
      
      ErrorCount++;
      if (ErrorCount > 2)
      {
        Beep_ON_Nop(500);   //������ʾ����ʾ��ʧ��
        Beep_ON_Nop(500);
        TCPIsUsed = 0;
        return FAILUED;      
      }
    }while(strstr(Uart3_ReceiveDat, "\"type\":\"add2user\",\"add2user\":true") == NULL);
    
    Beep_ON_Nop(1000);      //һ������ʾ����ʾ�󶨳ɹ�
    TIM_Cmd(TIM2, ENABLE);
    TCPIsUsed = 0;
    return SUCCESS;
  }
  TCPIsUsed = 0;
  return SUCCESS;
}

/*
 * ��������SendWarmingPack
 * ����  �����ͱ�����Ϣ
 * ����  ��sensor��   ������    
 *            ph��sensortemp��cputemp��speed��water��power
 *         warntype�� ��������
 *            -1��̫��    0������   1��̫��
 *         did��      �豸ID
 * ����  ��SUCCESS:  �ɹ�
 *         ������    ����
 */
uint8_t SendWarmingPack (uint8_t* sensor, int8_t warntype, uint8_t* did)
{
  uint8_t MsgSendBuf[300];
  uint8_t ReqSendBuf[30];
  uint8_t MsgLengthBuf[15];
  int MsgLength = 0;
  int ErrorCount = 0;
  int ErrorCount1 = 0;
  int Delay_TimeCount = 0;
  uint8_t statue;
  
  do
  { 
    sprintf ((char *)ReqSendBuf, "{\"type\":\"warn\",\"warn\":\"%s\"}\r\n", sensor);    //�����;�����Ϣ
    MsgLength = strlen((char *)ReqSendBuf);                         //���㳤��
      
    ErrorCount = 0;
    ErrorCount1 = 0;
    /* ����TCP����;���ǰ�� */
    
    do
    {
      statue = ReqSendTcp(MsgLength);
      if (statue != SUCCESS)
        return statue;
      
      Uart3_BufClear();                   //��մ��ڻ�����
      UserDelay_KeyScan(1500);                     //ʵ�ʷ�����ʱһ���ȶ�������ȥ
      USART3_SendBuf(ReqSendBuf);         //��������
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 30)
      {
        UserDelay_KeyScan(500);
        Delay_TimeCount++;
        if (strstr(Uart3_ReceiveDat, "true") != NULL)   //�ȴ������������ɹ�
          break;
      }      
      ErrorCount++;
      if (ErrorCount > 3)                //����������Ӧ��ʱ�˳�
      {
        return ServeNoResponse;      
      }
    }while(strstr(Uart3_ReceiveDat, "true") == NULL);
    
    sprintf ((char*)MsgSendBuf, "{\"type\":\"%d\",\"value\":\"%s\"}\r\n",warntype, did); 
    MsgLength = strlen((char*)MsgSendBuf);
    
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
    
    UserDelay_KeyScan(500);
    Uart3_BufClear();
    USART3_SendBuf(MsgSendBuf);
    Delay_TimeCount = 0;

    while(Delay_TimeCount < 50)
    {
      UserDelay_KeyScan(100);
      Delay_TimeCount++;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":true}") != NULL || (strstr(Uart3_ReceiveDat, "no user") != NULL))
        break;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":false}") != NULL)
        return FAILUED;
    }
    ErrorCount1++;
    if (ErrorCount1 > 3)
      return ServeNoResponse; 
  }while((strstr(Uart3_ReceiveDat, "{\"type\":\"warn\",\"warn\":true}") == NULL) && (strstr(Uart3_ReceiveDat, "no user") == NULL));
  return SUCCESS;
}

//float WaterMaxTest = 0.0;
//float WaterMinTest = 0.0;
//float WaterTempTest = 0.0;

/*
 * ��������UpWarmingDat_TCP
 * ����  ���ϴ�������Ϣ
 * ����  ����
 * ����  ��SUCCESS: �ɹ�
 *          ������ʧ��
 */
uint8_t EnvirTempWarmCount = 0;
uint8_t WaterTempWarmCount = 0;
uint8_t WaterSpeedWarmCount = 0;
uint8_t WaterLevelWarmCount = 0;
uint8_t PHWarmCount = 0;
uint8_t PowerWarmCount = 0;

//uint8_t WarmUpCount[MAX_SlaveNum + 1][6];

uint8_t UpWarmingDatAgain_TCP (void)
{
  uint8_t i = 0, k = 0, statue = 0;
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;
  uint8_t SalveIDStrBuf[9];
  
  /* ����������� */
  for (i = 0; i < 6; i++)
  {
    if ((WarmUpCount[0][i] >= 1) && (WarmUpCount[0][i] < 3))   //������������ұ�������С������
    {
      switch (i)
      {
        case 0:
          EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
          EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
          EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);
        
          if (EnvirTemp > EnvirTempMax)
            statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)ChipID_String);
          else if (EnvirTemp < EnvirTempMin)
            statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][0]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 19);  
          }
          else
            return FAILUED;
          break;
        case 1:
          WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
          WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
          WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0); 
          
          
        
          if (WaterTemp > WaterTempMax)
            statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)ChipID_String);
          else if (WaterTemp < WaterTempMin)
            statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][1]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 15);   
          }
          else
            return FAILUED;
          break;
        case 2:
          WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
          WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
          WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
          
          
        
          if (WaterSpeed > WaterSpeedMax)
            statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
          else if (WaterSpeed < WaterSpeedMin)
            statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
          else 
            statue = SUCCESS;
          
          WarmUpCount[0][2]++;
          
          if (statue == SUCCESS)
          {
            Flash_FifoBuf[5] |= (1 << 23); 
          }
          else
            return FAILUED;
          break;
        case 3:
          statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)ChipID_String);
          WarmUpCount[0][3]++;
        
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 25);                //�Ѿ��������������Ϣ
          }
          else
          {
            return FAILUED;
          }

          break;
        case 4:
          PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
          PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
          PH = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
          
          
        
          if (PH > PHMax)
            statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)ChipID_String);
          else if (PH < PHMin)
            statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)ChipID_String);
          else 
      //      statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)ChipID_String);
            statue = SUCCESS;
          
          WarmUpCount[0][4]++;
          
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 17);  
          }
          else
            return FAILUED;
          break;
        case 5:
          statue = SendWarmingPack((uint8_t*)"power", -1, (uint8_t*)ChipID_String);
          WarmUpCount[0][5]++;
        
          if (statue == SUCCESS)
          {
            
            Flash_FifoBuf[5] |= (1 << 21);                //�Ѿ��������������Ϣ
          }
          else
          {
            return FAILUED;
          }
          break;
      }
    }
  }
  for (k = 1; k <= Flash_FifoBuf[0]; k++)
  {
    Chip2String(Flash_FifoBuf[7 + (k - 1) * 3], (char*)SalveIDStrBuf);
    for (i = 0; i < 6; i++)
    {
      if ((WarmUpCount[k][i] >= 1) && (WarmUpCount[k][i] < 3))   //������������ұ�������С������
      {
        switch (i)
        {
          case 0:
            {
              EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
              EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
              EnvirTemp = ((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
              
              WarmUpCount[k][0]++;
            
              if (EnvirTemp > EnvirTempMax)
                statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)SalveIDStrBuf);
              else if (EnvirTemp < EnvirTempMin)
                statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 21);   
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 1:
              {
                WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
                WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
                WaterTemp = (((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
                
                WarmUpCount[k][1]++;
              
                if (WaterTemp > WaterTempMax)
                  statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)SalveIDStrBuf);
                else if (WaterTemp < WaterTempMin)
                  statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)SalveIDStrBuf);
                else 
                  statue = SUCCESS;
                
                if (statue == SUCCESS)
                {
                  
                  Flash_FifoBuf[7 + (k - 1) * 3 + 2] |= (1 << 4);  
                }
                else
                {
                  return FAILUED;
                }
              }
            break;
          case 2:
            {
              WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
              WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
              WaterSpeed = ((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (k - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
              
              WarmUpCount[k][2]++;
            
              if (WaterSpeed > WaterSpeedMax)
                statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)SalveIDStrBuf);
              else if (WaterSpeed < WaterSpeedMin)
                statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 23);   
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 3:
            {
              statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)SalveIDStrBuf);
              WarmUpCount[k][3]++;
            
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 1] |= (1 << 25);                //�Ѿ��������������Ϣ
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 4:
            {
              PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
              PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
              PH = (((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (k - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
                    
              WarmUpCount[k][4]++;
            
              if (PH > PHMax)
                statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)SalveIDStrBuf);
              else if (PH < PHMin)
                statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)SalveIDStrBuf);
              else 
                statue = SUCCESS;
              
              if (statue == SUCCESS)
              {
                
                Flash_FifoBuf[7 + (k - 1) * 3 + 2] |= (1 << 6);  
              }
              else
              {
                return FAILUED;
              }
            }
            break;
          case 5:
            break;
          default:
            break;
        }
      }
    }
  }
  return SUCCESS;
}

/*
 * ��������UpWarmingDat_TCP
 * ����  ���ϴ�������Ϣ
 * ����  ����
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t UpWarmingDat_TCP (void)
{
  uint8_t i, statue = 0;
  uint8_t WarmCount = 0;
  int ErrorCount = 0;
  int Delay_TimeCount = 0;
  uint8_t MsgLengthBuf[15];
  uint8_t SalveIDStrBuf[9];
  int MsgLength = 0;
  
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;
  /* �����ɼ�ֵ����Ƿ��г���ֵ�� */
  
  /* ˮλ���� */
  if (((Flash_FifoBuf[5] >> 24) & 0x03) == 0x01)    //ˮλ  ������ھ�����δ�ϱ�������
  {
    WarmCount++;
    statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)ChipID_String);
    if (statue == SUCCESS)
    {
      WarmUpCount[0][3] = 1;
      Flash_FifoBuf[5] |= (1 << 25);                //�Ѿ��������������Ϣ
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* ���籨�� */
  if (((Flash_FifoBuf[5] >> 20) & 0x03) == 0x01)
  {
    WarmCount++;
    statue = SendWarmingPack((uint8_t*)"power", -1, (uint8_t*)ChipID_String);
    if (statue == SUCCESS)
    {
      WarmUpCount[0][5] = 1;
      Flash_FifoBuf[5] |= (1 << 21);                //�Ѿ��������������Ϣ
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* ���ٱ��� */
  if (((Flash_FifoBuf[5] >> 22) & 0x03) == 0x01)
  {
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
    
    if (WaterSpeed > WaterSpeedMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
    }
    else if (WaterSpeed < WaterSpeedMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][2] = 1;
      Flash_FifoBuf[5] |= (1 << 23); 
    }
    else
    {
      return FAILUED;
    }
  }
  

  
  /* ���±��� */
  if (((Flash_FifoBuf[5] >> 18) & 0x03) == 0x01)
  {
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);
    
    if (EnvirTemp > EnvirTempMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)ChipID_String);
    }
    else if (EnvirTemp < EnvirTempMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"cputemp", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][0] = 1;
      Flash_FifoBuf[5] |= (1 << 19);  
    }
    else
    {
      return FAILUED;
    }
  }
  

  
  /* PH���� */
  if (((Flash_FifoBuf[5] >> 16) & 0x03) == 0x01)
  {
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    PH = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
    
    if (PH > PHMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)ChipID_String);
    }
    else if (PH < PHMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][4] = 1;
      Flash_FifoBuf[5] |= (1 << 17);  
    }
    else
    {
      return FAILUED;
    }
  }
  
  /* ˮ�±��� */
  if (((Flash_FifoBuf[5] >> 14) & 0x03) == 0x01)
  {
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//    WaterTempMin =    (Flash_FifoBuf[1] & 0xf  << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) +  (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0); 

    
//    WaterMaxTest = WaterTempMax;
//    WaterMinTest = WaterTempMin;
//    WaterTempTest = WaterTemp;
    
    if (WaterTemp > WaterTempMax)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)ChipID_String);
    }
    else if (WaterTemp < WaterTempMin)
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)ChipID_String);
    }
    else 
//      statue = SendWarmingPack((uint8_t*)"sensortemp", 0, (uint8_t*)ChipID_String);
      statue = SUCCESS;
    
    if (statue == SUCCESS)
    {
      WarmUpCount[0][1] = 1;
      Flash_FifoBuf[5] |= (1 << 15);   
    }
    else
    {
      return FAILUED;
    }
  }
  

  /* *************�ӻ�������ޱ���***************** */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SalveIDStrBuf);
    
    /* ˮλ��� */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 24) & 0x03) == 0x01)    //ˮλ  ������ھ�����δ�ϱ�������
    {
      WarmCount++;
      statue = SendWarmingPack((uint8_t*)"water", -1, (uint8_t*)SalveIDStrBuf);
      if (statue == SUCCESS)
      {
        WarmUpCount[i][3] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 25);                //�Ѿ��������������Ϣ
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* ���ټ�� */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 22) & 0x03) == 0x01)
    {
      WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
      WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
      WaterSpeed = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
      
      if (WaterSpeed > WaterSpeedMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"speed", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (WaterSpeed < WaterSpeedMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"speed", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"speed", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][2] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 23);   
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* ���¼�� */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 20) & 0x03) == 0x01)
    {
      EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
      EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
      EnvirTemp = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
      
      if (EnvirTemp > EnvirTempMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"cputemp", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (EnvirTemp < EnvirTempMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"cputemp", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"cputemp", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][0] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 21);   
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* PH��� */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 5) & 0x03) == 0x01)
    {
      PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
      PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
      PH = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
            
      if (PH > PHMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"ph", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (PH < PHMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"ph", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"ph", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      if (statue == SUCCESS)
      {
        WarmUpCount[i][4] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] |= (1 << 6);  
      }
      else
      {
        return FAILUED;
      }
    }
    
    /* ˮ�¼�� */
    if (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 3) & 0x03) == 0x01)
    {
      WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
//      WaterTempMin = (Flash_FifoBuf[1] & 0xf << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
      WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
      WaterTemp = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
      
      if (WaterTemp > WaterTempMax)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"sensortemp", 1, (uint8_t*)SalveIDStrBuf);
      }
      else if (WaterTemp < WaterTempMin)
      {
        WarmCount++;
        statue = SendWarmingPack((uint8_t*)"sensortemp", -1, (uint8_t*)SalveIDStrBuf);
      }
      else 
//        statue = SendWarmingPack((uint8_t*)"sensortemp", 0, (uint8_t*)SalveIDStrBuf);
        statue = SUCCESS;
      
      if (statue == SUCCESS)
      {
        WarmUpCount[i][1] = 1;
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] |= (1 << 4);  
      }
      else
      {
        return FAILUED;
      }
    }
  }
  
  if (WarmCount > 0)
  {
    do
    {
      statue = UpLoadDat_TCP();
      ErrorCount++;
      if (ErrorCount > 10)
      {
        ErrorCount = 0;
        TCPIsUsed = 0;
        GPRS_Rest();
        TCPIsUsed = 1;
      }
    }while(statue != SUCCESS);
    
  }
  
  return SUCCESS;
}


/*
 * ��������Bool2String
 * ����  ��boolת��Ϊstring
 * ����  ����
 * ����  ����
 */
void Bool2String (int num, uint8_t* string)
{
  if (num)
  {
    strcpy((char*)string, "true");
  }
  else
  {
    strcpy((char*)string, "false");
  }
}

/*
 * ��������StrCpy
 * ����  �������ַ���
 * ����  ��Src��    Դ
 *         Dest��   Ŀ��
 *         Length�� Ҫ���Ƶĳ���
 * ����  ����
 */
void StrCpy (char* Src, char* Dest, int Length)
{
  int i = 0;
  for (i = 0; i < Length; i++)
  {
    *Dest = *Src;
    Dest++;
    Src++;
  }
  *Dest = '\0';
}

/*
 * ��������StringFloat
 * ����  ���ַ���ת��Ϊfloat
 * ����  ��num��    Ҫת����float
 *         String�� ת���ɹ���string
 * ����  ����
 */
void StringFloat (float* num, char String[])
{
  char length = 0;
  length = strlen(String);
  
  switch (length)
  {
    case 3:
      String[0] -= 0x30;
      String[2] -= 0x30;
      *num = String[0] + (String[2] / 10.0);
      break;
    case 4:
      String[0] -= 0x30;
      String[1] -= 0x30;
      String[3] -= 0x30;
      *num = String[0] * 10 + String[1] + (String[3] / 10.0);
      break;
    default:
      break;
  }
  
}

/*
 * ��������BufClear
 * ����  �����A6���ڽ���Buf
 * ����  ����
 * ����  ����
 */
void BufClear (void)
{
  int i =0;
  for (i = 0; i < 299; i++)
  {
    DatFromServer[i] = 0x00;
  }
  
}

/*
 * ��������GPRS_ReceMsgProcess
 * ����  ��A6���ڽ��մ���������ȡ��ֵ
 * ����  ����
 * ����  ����
 */
void GPRS_ReceMsgProcess (void)
{
  uint8_t i = 0;
  char* TempPosition = 0;
  char* position = 0;
  char Length = 0;
  
  char DevID_String[25];            //ID
  char PHMin_String[5];             //PH
  char WaterTempMin_String[5];      //ˮ����Сֵ
  char WaterTempMax_String[5];      //ˮ����Сֵ
  char EnvirTempMin_String[5];      //������Сֵ
  char EnvirTempMax_String[5];      //������Сֵ
  char WaterSpeedMin_String[5];     //��Сˮ��
  char WaterSpeedMax_String[5];     //��Сˮ��
  char PHMax_String[5];
  
  float DevID_Float;
  float WaterSpeedMax_float;   
  float PHMin_Float;
  float WaterTempMin_Float;
  float EnvirTempMin_Flaot;
  float WaterSpeedMin_Float;
  float PHMax_Float;
  float WaterTempMax_Float;
  float EnvirTempMax_Float;
  float WaterSpeedMax_Float;
  
  int TempInt;
  int TempFloat;
  
 
  if (strstr(DatFromServer, "minph") != NULL)         //����Ƿ��������������͵�����ֵ�йصİ�
  {
    position = (strstr(DatFromServer, "did") + 6);
    StrCpy(position, DevID_String, 24);               //��ȡ��ID
    if (strstr(DevID_String, ChipID_String) != NULL)
    {
      /* PH���� */
      position = (strstr(DatFromServer, "minph") + 7);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, PHMin_String, Length);         //��ȡPH
      StringFloat(&PHMin_Float, PHMin_String);        //ת��
      TempInt = (int)PHMin_Float;
      TempFloat = ((int)(PHMin_Float * 10) % 10);
 
      if ((PHMin_Float >= PH_Min) && (PHMin_Float <= PH_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 16);
        Flash_FifoBuf[1] |= ((TempInt & 0xf) << 20);    //����
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 16);  
      }     
      
      /* ˮ������ */
      position = (strstr(DatFromServer, "minsensortemp") + 15);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterTempMin_String, Length);            //��ȡˮ��
      StringFloat(&WaterTempMin_Float, WaterTempMin_String);    //ת��
      TempInt = (int)WaterTempMin_Float;
      TempFloat = ((int)(WaterTempMin_Float * 10) % 10);
      
      if ((WaterTempMin_Float >= WaterTmp_Min) && (WaterTempMin_Float <= WaterTmp_Max))
      {
        Flash_FifoBuf[1] &= ~(0xf);
        Flash_FifoBuf[1] |= ((TempInt >> 4)& 0xf);                //��������
        Flash_FifoBuf[2] &= ~(0xff << 24);
        Flash_FifoBuf[2] |= ((TempInt & 0xf) << 28);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 24);
      }
      /* ************************************* */
      
      /* �������� */    
      position = (strstr(DatFromServer, "mincputemp") + 12);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, EnvirTempMin_String, Length);            //��ȡ����
      StringFloat(&EnvirTempMin_Flaot, EnvirTempMin_String);    //ת��
      TempInt = (int)EnvirTempMin_Flaot;
      TempFloat = ((int)(EnvirTempMin_Flaot * 10) % 10);
      
      if ((EnvirTempMin_Flaot >= EnvirTemp_Min) && (EnvirTempMin_Flaot <= EnvirTemp_Max))
      {
        Flash_FifoBuf[2] &= ~(0xff << 4);
        Flash_FifoBuf[2] |= ((TempInt & 0xff) << 4);              //��������
        Flash_FifoBuf[2] &= ~(0xf << 0);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 0);
      }
      /* ************************************* */
      
      /* ˮ������ */
      position = (strstr(DatFromServer, "minspeed") + 10);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterSpeedMin_String, Length);         
      StringFloat(&WaterSpeedMin_Float, WaterSpeedMin_String); 
      TempInt = (int)WaterSpeedMin_Float;
      TempFloat = ((int)(WaterSpeedMin_Float * 10) % 10);
      
      if ((WaterSpeedMin_Float >= WaterSpeed_Min) && (WaterSpeedMin_Float <= WaterSpeed_Max))
      {
        Flash_FifoBuf[3] &= ~(0xff << 12);
        Flash_FifoBuf[3] |= ((TempInt & 0xff) << 12);             //����
        Flash_FifoBuf[3] &= ~(0xf << 8);
        Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 8);
      }      
      /* ************************************* */
      
      /* PH���� */
      position = (strstr(DatFromServer, "maxph") + 7);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, PHMax_String, Length);                  
      StringFloat(&PHMax_Float, PHMax_String);                 
      TempInt = (int)PHMax_Float;
      TempFloat = ((int)(PHMax_Float * 10) % 10);
      
      if ((PHMax_Float >= PH_Min) && (PHMax_Float <= PH_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 24);
        Flash_FifoBuf[1] |= ((TempInt & 0xf) << 28);              //����
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 24);
      }
      /* ************************************* */

      /* ˮ������ */
      position = (strstr(DatFromServer, "maxsensortemp") + 15);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterTempMax_String, Length);           
      StringFloat(&WaterTempMax_Float, WaterTempMax_String);    
      TempInt = (int)WaterTempMax_Float;
      TempFloat = ((int)(WaterTempMax_Float * 10) % 10);
      
      if ((WaterTempMax_Float >= WaterTmp_Min) && (WaterTempMax_Float <= WaterTmp_Max))
      {
        Flash_FifoBuf[1] &= ~(0xff << 8);
        Flash_FifoBuf[1] |= ((TempInt & 0xff) << 8);              //��������        
        Flash_FifoBuf[1] &= ~(0xf << 4);
        Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 4);
      }
      /* ************************************* */

      /* �������� */
      position = (strstr(DatFromServer, "maxcputemp") + 12);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, EnvirTempMax_String, Length);           
      StringFloat(&EnvirTempMax_Float, EnvirTempMax_String);    //ת��
      TempInt = (int)EnvirTempMax_Float;
      TempFloat = ((int)(EnvirTempMax_Float * 10) % 10);
      
      if ((EnvirTempMax_Float >= EnvirTemp_Min) && (EnvirTempMax_Float <= EnvirTemp_Max))
      {
        Flash_FifoBuf[2] &= ~(0xff << 16);
        Flash_FifoBuf[2] |= ((TempInt & 0xff) << 16);             //��������
        Flash_FifoBuf[2] &= ~(0xf << 12);
        Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 12);
      }      
      /* ************************************* */
      
      /* ˮ������ */
      position = (strstr(DatFromServer, "maxspeed") + 10);
      TempPosition = (strstr(position, ","));
      Length = TempPosition - position;
      StrCpy(position, WaterSpeedMax_String, Length);          
      StringFloat(&WaterSpeedMax_Float, WaterSpeedMax_String); 
      TempInt = (int)WaterSpeedMax_Float;
      TempFloat = ((int)(WaterSpeedMax_Float * 10) % 10);
      
      if ((WaterSpeedMax_Float >= WaterSpeed_Min) && (WaterSpeedMax_Float <= WaterSpeed_Max))
      {
        Flash_FifoBuf[3] &= ~(0xff << 24);
        Flash_FifoBuf[3] |= ((TempInt & 0xff) << 24);             //��������
        Flash_FifoBuf[3] &= ~(0xf << 20);
        Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 20);
      }   
      
      /* ����������־λ��� */
      Flash_FifoBuf[5] &= ~(0x3 << 23);   //����
      Flash_FifoBuf[5] &= ~(0x3 << 19);   //����
      Flash_FifoBuf[5] &= ~(0x3 << 17);   //PH
      Flash_FifoBuf[5] &= ~(0x3 << 15);   //ˮ��
      WarmUpCount[0][0] = 0;
      WarmUpCount[0][1] = 0;
      WarmUpCount[0][2] = 0;
      WarmUpCount[0][3] = 0;
      WarmUpCount[0][4] = 0;
      
      /* �ӻ�������־λ��� */
      for (i = 1; i <= Flash_FifoBuf[0]; i++)
      {
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0x3 << 23);	    //����
        Flash_FifoBuf[7 + (i - 1) * 3 + 1] &= ~(0x3 << 21);	    //����
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] &= ~(0x3 << 6);  	  //PH
        Flash_FifoBuf[7 + (i - 1) * 3 + 2] &= ~(0x3); 	        //ˮ��
        WarmUpCount[i][0] = 0;
        WarmUpCount[i][1] = 0;
        WarmUpCount[i][2] = 0;
        WarmUpCount[i][3] = 0;
        WarmUpCount[i][4] = 0;
        WarmUpCount[i][5] = 0;
      }
      
      //GoLimPage();
    }
    BufClear();
  }
  
}


/*
 * ��������GPRS_Init
 * ����  ��TCP���ӷ�������ʼ��
 * ����  ����
 * ����  ����
 */
void GPRS_Rest (void)
{
  uint8_t statue = DefaultValue;
  
  /* ��ʼ��TCPǰ��Ҫȷ����ǰû��������ʹ��TCP */
  if (TCPIsUsed == 0)
  {
    TCPIsUsed = 1;
    do
    {
      statue = GRPS_NetInit();    //GPRS��ʼ��
      switch(statue)
      {
        
        case NoResponse:          //GRPS����Ӧ
          InitDis(" GRPS����Ӧ ");
          break;
        case SUCCESS:             //��ʼ���ɹ�
          GetNet = 1;
          break;
        case NoSIMCard:           //δ��⵽SD��
          InitDis(" ��SIM�� ");
          while(1);
        case GRPSGetFailue:       //GPRS����ʧ��
          InitDis(" GPRS����ʧ��");
          break;
        case PDPGetFailue:        //PDP����ʧ��
          InitDis(" PDP����ʧ�ܣ��������� ");
          break;
        case ServeNoResponse:     //���ӷ�����ʱû�з���
          InitDis(" ����������Ӧ���������� ");
          break;    
        case GetTCPFailue:
          InitDis(" �޷�����������ӣ��������� ");
          break;
        default:
          InitDis(" ����δ֪������������ ");
          break;
      }

    }while(statue != SUCCESS);    //������ɹ���ѭ�����ϳ���
    TCPIsUsed = 0;                //�ɹ������TCP��ռ�ñ�־λ
  }
}

/*
 * ��������SendMsg2Server
 * ����  ����������������ݣ��˺����������������ã�һ�㲻ֱ��ʹ��
 * ����  �� SendBuf:���͵İ�
 *          CheckBuf���ж��Ƿ��лذ��Ĺؼ�������
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t SendMsg2Server (uint8_t* SendBuf, char* CheckBuf)
{
  uint8_t ErrorCount1 = 0;    //�������1
  uint8_t ErrorCount2 = 0;    //�������2
  uint8_t statue = 0;
  
  Uart3_BufClear();  
  
  do                   
  {
    USART3_SendBuf(SendBuf);    
    ErrorCount1 = 0; 
    ErrorCount2 = 0;
    while(ErrorCount1 < 100)   //��10s�ڲ鿴�Ƿ񷵻سɹ�״̬
    {
      UserDelay_KeyScan(100);
      ErrorCount1++;
      if (strstr(Uart3_ReceiveDat, CheckBuf) != NULL)
        break;
      if (strstr(Uart3_ReceiveDat, "ERROR") != NULL)        //�����������ʧ��
        return LocalFault;
    }
    ErrorCount2++;
    if (ErrorCount2 > 2)      //�ܹ��ȴ�30s
      return NoResponse;
   }while(strstr(Uart3_ReceiveDat, CheckBuf) == NULL);
  
  UserDelay_KeyScan(200);
   
  return SUCCESS;             //ִ������ָ��˵��ǰ��һ��˳��
}

/*
 * ��������GRPS_NetInit
 * ����  ��GPRS�����ʼ��
 * ����  ����
 * ����  ����
 */
uint8_t GRPS_NetInit (void)
{
  uint8_t i, statue = 0;
  int ErrorCount = 0;         //�������
  int Delay_TimeCount = 0;    
  uint8_t MsgSendBuf[300];    //���͵İ�
  uint8_t MsgLengthBuf[15];   //�����Ȱ�
  uint8_t SalveIDStrBuf[9];   //�ӻ��豸ID��
  int MsgLength = 0;          //������
  uint8_t getNetTimeStatue = SUCCESS;
  /* ��λ */
  A6_Rst(OFF);
  UserDelay_KeyScan(200);
  A6_Rst(ON);
  
  UserDelay_KeyScan(10000);      //��ʱ10s  ��ʱ�Ƚ���ʱ���������Ҫ�Ƶ��������ĵط�ȥ   ��ʱ��Ϊ��GPRS����������
  
  statue = SendMsg2Server((uint8_t *)"AT\r\n", "OK");
  if (statue != SUCCESS)
    return NoResponse;

  /* �������SIM�� */
  InitDis(" ���ڼ��SIM�� ");
  statue = SendMsg2Server((uint8_t *)"AT+CCID\r\n", "SIM Card ID");
  if (statue != SUCCESS)
    return NoSIMCard;
  InitDis(" ��⵽SIM�� ");
  
  /* GPRS���� */
  InitDis(" ���ڸ���GPRS ");
  UserDelay_KeyScan(3000);
  statue = SendMsg2Server((uint8_t *)"AT+CGATT=1\r\n", "OK");
  if (statue != SUCCESS)
    return GRPSGetFailue;
  InitDis(" GPRS���ųɹ� ");
  
  /* PDP���� */
  InitDis(" ����PDP���� ");
  UserDelay_KeyScan(5000);
  statue = SendMsg2Server((uint8_t *)"AT+CGACT=1,1\r\n", "OK");
  if (statue != SUCCESS)
    return PDPGetFailue;
  InitDis(" PDP����ɹ� ");
  
  /* ��⵱ǰ�Ƿ�������TCP */
  
  UserDelay_KeyScan(2000);
  statue = SendMsg2Server((uint8_t *)"AT+CIPSTATUS\r\n", "+IPSTATUS");
  if (statue != SUCCESS)
    return statue;
  
//  InitDis(" ���ڻ�ȡ����ʱ�� ");
//  ErrorCount = 0;
//  Uart3_BufClear();
//  /* ����TCP */
//  USART3_SendBuf("at+cipstart=\"TCP\",\"132.163.4.102\",13\r\n");  
//  while((strstr(Uart3_ReceiveDat, "UTC(NIST)") == NULL) && getNetTimeStatue == SUCCESS)
//  {
//    UserDelay_KeyScan(20);
//    ErrorCount++;
//    if (ErrorCount > 1000)
//      getNetTimeStatue = FAILUED;
//  }
//  if (getNetTimeStatue == SUCCESS)
//    GetNetTime();
//  else
//  {
//    calendar.w_year = 2016;
//    calendar.w_month = 6;
//    calendar.w_date = 15;
//    calendar.hour = 12;
//    calendar.min = 30;
//    calendar.sec = 30;
//    InitDis(" ����ʱ���ȡʧ�ܣ��뿪�����ֶ�����");
//  }
//    
//  Delay_ms(1000);
  
//  UserDelay_KeyScan(100);  
//  statue = SendMsg2Server("AT+CIPCLOSE\r\n", "OK");
//  if (statue != SUCCESS)
//    return statue;
  
  InitDis(" ��������TCP ");
  ErrorCount = 0;
  Uart3_BufClear();
  /* ����TCP */
  USART3_SendBuf("at+cipstart=\"TCP\",\"115.28.72.3\",4040\r\n");  
  while(strstr(Uart3_ReceiveDat, "CIPRCV") == NULL)
  {
    UserDelay_KeyScan(20);
    ErrorCount++;
    if (ErrorCount > 1000)
      return GetTCPFailue;
  }
  InitDis(" TCP���ӳɹ� ");
  
  InitDis(" ���ڹҽ��豸 ");
  sprintf((char*)MsgSendBuf, "{\"type\":\"did\",\"did\":\"%s\"}\r\n", ChipID_String);
  MsgLength = strlen((char*)MsgSendBuf);     
  sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //�������ںϽ����
  /* �趨��һ�����ķ������ݰ����� */
  
  statue = SendMsg2Server((uint8_t *)MsgLengthBuf, ">");
  if (statue != SUCCESS)
    return statue;
  
  /* ���ʷ�����������������Ϣ */
  statue = SendMsg2Server((uint8_t *)MsgSendBuf, ChipID_String);
  if (statue != SUCCESS)
    return statue;
  
  
  /* ���δ����Լ�δ����������� */
  if ((((Flash_FifoBuf[5] >> 31) & 0x01) == 0) && (((Flash_FifoBuf[5] >> 30) & 0x01) == 0))
  {
      /* �趨��һ����䳤�� */
    sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", ChipID_String);   //�Ⱥϲ�����֡���
    MsgLength = strlen((char*)MsgSendBuf);                                                //��������֡��䳤��
    sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //�������ںϽ����
    
    statue = SendMsg2Server((uint8_t *)MsgLengthBuf, ">");
    if (statue != SUCCESS)
      return statue;
    
    /* �ڷ���������ӱ������豸 */
    /* ������ݿ����Ѿ���û�У�����ӣ�����У��Ͳ���� */
    ErrorCount = 0;
    do
    {
      UserDelay_KeyScan(3000);
      Uart3_BufClear();  
      USART3_SendBuf(MsgSendBuf);  
      Delay_TimeCount = 0;
      while(Delay_TimeCount < 10)
      {
        UserDelay_KeyScan(100);
        Delay_TimeCount++;
        if ((strstr(Uart3_ReceiveDat, ChipID_String) != NULL) || (strstr(Uart3_ReceiveDat, "repeat") != NULL))
        {
          Flash_FifoBuf[5] |= (1 << 30);    //������
          Flash_FifoBuf[5] |= (1 << 31);    //�����          
          break;
        }
      }
      ErrorCount++;
      if (ErrorCount > 10)
        return ServeNoResponse;
    }while((strstr(Uart3_ReceiveDat, ChipID_String) == NULL) && (strstr(Uart3_ReceiveDat, "repeat") == NULL));
  }
  
  /* �ڷ���������Ӵӻ��豸 */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    if((((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 31) & 0x01) == 0) &&           //�豸δ�����δ���������������
         (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 30) & 0x01) == 0))
    {
      Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SalveIDStrBuf);
      sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SalveIDStrBuf);   //�Ⱥϲ�����֡���
      MsgLength = strlen((char*)MsgSendBuf);    
      sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", MsgLength);     //�������ںϽ����
      
      do
      {
        UserDelay_KeyScan(1000);
        Uart3_BufClear();  
        USART3_SendBuf(MsgLengthBuf);  
        Delay_TimeCount = 0;
        while(Delay_TimeCount < 30)
        {
          UserDelay_KeyScan(100);
          Delay_TimeCount++;
          if (strstr(Uart3_ReceiveDat, ">") != NULL)
            break;
        }
      }while(strstr(Uart3_ReceiveDat, ">") == NULL);
      
      ErrorCount = 0;
      do
      {
        UserDelay_KeyScan(100);
        Uart3_BufClear();  
        USART3_SendBuf(MsgSendBuf);  
        Delay_TimeCount = 0;
        while(Delay_TimeCount < 10)
        {
          UserDelay_KeyScan(100);
          Delay_TimeCount++;
          if ((strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) || strstr(Uart3_ReceiveDat, "repeat")) != NULL)
          {
            Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 30);    //������
            Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 31);    //�����          
            break;
          }
        }
        ErrorCount++;
        if (ErrorCount > 10)
          return ErrorFromServe;
      }while((strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) || strstr(Uart3_ReceiveDat, "repeat")) == NULL);
      
      Flash_FifoBuf[7 + (i - 1) * 3 + 1] |= (1 << 30);     //�Ѿ����������������
    }
  }
  
  statue = GetOffsetDat();
  if (statue != SUCCESS)
      return statue;
  InitDis(" ����������� ");
  
  return SUCCESS;  
}

/*
 * ��������AddSlaveID2Server
 * ����  ������������һ���ӻ�
 * ����  �� SlaveID:�ӻ�ID
 *          SlaveSerial���ӻ��ڱ��ص����
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t AddSlaveID2Server (long SlaveID, uint8_t SlaveSerial)
{
  uint8_t statue = 0;
  uint8_t SalveIDStrBuf[9];
  uint8_t MsgLength;
  uint8_t MsgSendBuf[100];
  uint16_t Delay_TimeCount = 0, ErrorCount = 0; 
  
  Chip2String(SlaveID, (char*)SalveIDStrBuf);
  sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SalveIDStrBuf);   //�Ⱥϲ�����֡���
  MsgLength = strlen((char*)MsgSendBuf);   
  
  statue = ReqSendTcp(MsgLength);
  if (statue != SUCCESS)
      return statue;
  
  statue = SendMsg2Server((uint8_t *)MsgSendBuf, (char*)SalveIDStrBuf);
  
  if (statue != SUCCESS)
    return ErrorFromServe;
  
//  ErrorCount = 0;
//  do
//  {
//    UserDelay_KeyScan100);
//    Uart3_BufClear();  
//    USART3_SendBuf(MsgSendBuf);  
//    Delay_TimeCount = 0;
//    while(Delay_TimeCount < 10)
//    {
//      UserDelay_KeyScan100);
//      Delay_TimeCount++;
//      if (strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) != NULL)
//        break;
//    }
//    ErrorCount++;
//    if (ErrorCount > 10)
//      return ErrorFromServe;
//  }while(strstr(Uart3_ReceiveDat, (char*)SalveIDStrBuf) == NULL);
  
  Flash_FifoBuf[SlaveSerial + 1] |= (1 << 30);    //������
  Flash_FifoBuf[SlaveSerial + 1] |= (1 << 31);    //�����        
  
  return SUCCESS;
}

/*
 * ��������UpSlaveLost2Server
 * ����  ����������ϱ�һ���ӻ���ʧ
 * ����  �� SlaveChipIDString:�ӻ�ID �ַ�����ʽ
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t UpSlaveLost2Server (uint8_t* SlaveChipIDString)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgSendBuf[300];
  sprintf((char*)MsgSendBuf, "{\"type\":\"lost\",\"lost\":\"%s\"}\r\n", SlaveChipIDString);
  
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 40)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        return SUCCESS;  
    } 
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
  return SUCCESS;  
}

/*
 * ��������AddSlaveDev2Server
 * ����  �����������Ӵӻ�
 * ����  ��FlashPos���ӻ���Flash�е������ʶλ
 *         SlaveChipIDString�� �ӻ�ID
 * ����  ��SUCCESS�� �ɹ�   ������ʧ��
 */
uint8_t AddSlaveDev2Server (uint8_t FlashPos, uint8_t* SlaveChipIDString)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgSendBuf[300];
  sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);
  
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 10)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, (char*)SlaveChipIDString) != NULL)
      {
        Flash_FifoBuf[FlashPos] |= (1 << 30);    //������
        Flash_FifoBuf[FlashPos] |= (1 << 31);    //�����
        break;
      }
    } 
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return ErrorFromServe;
    }
  }while(strstr(Uart3_ReceiveDat, (char*)SlaveChipIDString) == NULL);
  return SUCCESS;  
}

/*
 * ��������ReqSendTcp
 * ����  ����A6������TCPЭ��
 * ����  ��length��Ҫ���͵�Э��ĳ���
 * ����  ��SUCCESS�� �ɹ�   ������ʧ��
 */
uint8_t ReqSendTcp (uint8_t length)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgLengthBuf[20];
  sprintf((char*)MsgLengthBuf, "AT+CIPSEND=%d\r\n", length);     //�������ںϽ����
  
  ErrorCount = 0;
  do
  {
    UserDelay_KeyScan(1000);
    Uart3_BufClear();  
    USART3_SendBuf(MsgLengthBuf);  
    TimeDelayCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, ">") != NULL)
        break;
    }      
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return GRPSGetFailue;
    }
  }while(strstr(Uart3_ReceiveDat, ">") == NULL);
  return SUCCESS;
}

/*
 * ��������SendDatTCP2Server_Ready
 * ����  ���������ݰ�ǰ�����ݱ�ʶ��
 * ����  ����
 * ����  ��SUCCESS�� �ɹ�   ������ʧ��
 */
uint8_t SendDatTCP2Server_Ready (uint8_t MsgLength)
{
  uint8_t statue = 0;
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  
  do
  {
    Uart3_BufClear();  
    statue = ReqSendTcp(MsgLength);   //������TCP
    if (statue != SUCCESS)
      return statue;
    UserDelay_KeyScan(500);
    USART3_SendBuf((uint8_t *)"{\"type\":\"data\",\"data\":\"message\"}\r\n");  
    TimeDelayCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(100);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "true") != NULL)
        break;
    }
    ErrorCount++;
    if (ErrorCount > 10)
    {
      return ServeNoResponse;
    }
  }while(strstr(Uart3_ReceiveDat, "true") == NULL);
  return SUCCESS;
}

/*
 * ��������SendDat2Server
 * ����  �����Ͳɼ��������ݵ�������
 * ����  ��MsgSendBuf��Ҫ���͵İ�
 * ����  ��SUCCESS�� �ɹ�   ������ʧ��
 */
uint8_t SendDat2Server (uint8_t* MsgSendBuf)
{
  uint8_t ErrorCount = 0;
  uint8_t TimeDelayCount = 0;
  uint8_t MsgLength, MsgLength1;
  uint8_t statue = 0;
  
  MsgLength = strlen((char*)MsgSendBuf);     //��������֡��䳤��
  
  do
  {
    statue = ReqSendTcp(MsgLength);
    if (statue != SUCCESS)
      return statue;
    UserDelay_KeyScan(500);
    Uart3_BufClear();  
    USART3_SendBuf(MsgSendBuf);   //���Ͳɼ�����������Ϣ
    TimeDelayCount = 0;
    ErrorCount = 0;
    while(TimeDelayCount < 30)
    {
      UserDelay_KeyScan(200);
      TimeDelayCount++;
      if (strstr(Uart3_ReceiveDat, "{\"type\":\"data\",\"data\":true}") != NULL)    //��⵽������ȷ
        break;
      if (strstr(Uart3_ReceiveDat, "\"format\":false") != NULL)         //��⵽��ʽ����
      {    
        /* �趨�����͵���䳤�� */
        MsgLength1 = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");   //��������֡��䳤��
        
        statue = SendDatTCP2Server_Ready(MsgLength1); //����������ʼ��
        if (statue != SUCCESS)
          return statue;        
        break;
      }
    }    
    ErrorCount++;
    if (ErrorCount > 3)
    {
      return ServeNoResponse;
    }
  }while(strstr(Uart3_ReceiveDat, "{\"type\":\"data\",\"data\":true}") == NULL);
  return SUCCESS;
}

/*
 * ��������UpOneSlaveDat2Server
 * ����  ����������ϱ�һ���ӻ���Ϣ
 * ����  �� SlaveNum:�ӻ��ڱ��ص����
 * ����  �� SUCCESS: �ɹ�
 *          else:    ʧ��
 */
uint8_t UpOneSlaveDat2Server (uint8_t SlaveNum)
{
  uint8_t statue = DefaultValue;
  int MsgLength = 0;
    
  uint8_t WaterLevel = 0;   //ˮλ
  uint8_t PowerDown = 0;    //����
  float WaterSpeed = 0.0;   //����
  float EnvirTemp = 0.0;    //����
  float ph = 0.0;           //ph
  float WaterTemp = 0.0;    //ˮ��
  
  uint8_t WaterLevelString[5]; 
  uint8_t SlaveChipIDString[15];
  uint8_t MsgSendBuf[300];
  uint8_t PowerDownString[5];  
  
  PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);  
  WaterLevel = ((Flash_FifoBuf[SlaveNum + 2] >> 7) & 0x01);
  Bool2String(WaterLevel, WaterLevelString);
  WaterSpeed = ((Flash_FifoBuf[SlaveNum + 2] >> 12) & 0xff) + (((Flash_FifoBuf[SlaveNum + 2] >> 8) & 0xf) / 10.0);
  EnvirTemp = ((Flash_FifoBuf[SlaveNum + 2] >> 24) & 0xff) + (((Flash_FifoBuf[SlaveNum + 2] >> 20) & 0xf) / 10.0);
  ph = (((Flash_FifoBuf[SlaveNum + 1] >> 16) & 0xf) + ((Flash_FifoBuf[SlaveNum + 1] >> 12) & 0xf) / 10.0);
  WaterTemp = (((Flash_FifoBuf[SlaveNum + 1] >> 4) & 0xff) + ((Flash_FifoBuf[SlaveNum + 1] >> 0) & 0xf) / 10.0);
  
  Chip2String(Flash_FifoBuf[SlaveNum], (char*)SlaveChipIDString);
  Bool2String(!PowerDown, PowerDownString);   
  
//  if((((Flash_FifoBuf[SlaveNum + 1] >> 31) & 0x01) == 0) &&           //�豸δ�����δ���������������
//         (((Flash_FifoBuf[SlaveNum + 1] >> 30) & 0x01) == 0))
//  {
//    sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);   //�Ⱥϲ�����֡���
//    MsgLength = strlen((char*)MsgSendBuf);    
//    
//    /* ��A6���뷢��TCP��Ϣ */
//    statue = ReqSendTcp(MsgLength);
//    if (statue != SUCCESS)
//      return statue;

//    /* ������Ӵӻ��� */
//    statue = AddSlaveDev2Server(SlaveNum + 1, SlaveChipIDString);
//    if (statue != SUCCESS)
//      return statue;
//  }
  MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");
//  /* Ҫ��֪ͨ������������Ҫ���Ͳɼ������� */
//  statue = ReqSendTcp(MsgLength);
//  if (statue != SUCCESS)
//    return statue;
  
  /* �������ݰ�ǰ�� */
  statue = SendDatTCP2Server_Ready(MsgLength);
  if (statue != SUCCESS)
    return statue;
  
  sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
              SlaveChipIDString, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
  
  statue = SendDat2Server(MsgSendBuf);   //���ʹӻ�����
  if (statue != SUCCESS)
  return statue;
  
  return SUCCESS;
}

/*
 * ��������UpLoadDat_TCP
 * ����  �����������������
 * ����  ����
 * ����  ����
 */
uint8_t UpLoadDat_TCP (void)
{
  /* �������ÿ��Сʱ�����������һ�����ݵĴ��� */
  uint8_t statue = DefaultValue;
  
  int i = 0, MsgLength = 0, ErrorCount = 0, ErrorCount1 = 0;
  int Delay_TimeCount = 0;
  uint8_t WaterLevel = 0;   //ˮλ
  uint8_t PowerDown = 0;    //����
  float WaterSpeed = 0.0;   //����
  float EnvirTemp = 0.0;    //����
  float ph = 0.0;           //ph
  float WaterTemp = 0.0;    //ˮ��
  
  /* �������������ַ�����ʽ */
  uint8_t WaterLevelString[5]; 
  uint8_t PowerDownString[5];  
  uint8_t SlaveChipIDString[15];
  uint8_t MsgSendBuf[300];
 
//  for (i = 1; i <= Flash_FifoBuf[0]; i++)
//  {
//    if((((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 31) & 0x01) == 0) &&           //�豸δ�����δ���������������
//         (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 30) & 0x01) == 0))
//    {
//      Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SlaveChipIDString);
//      sprintf((char*)MsgSendBuf, "{\"type\":\"newdid\",\"newdid\":\"%s\"}\r\n", SlaveChipIDString);   //�Ⱥϲ�����֡���
//      MsgLength = strlen((char*)MsgSendBuf);    
//      
//      /* ��A6���뷢��TCP��Ϣ */
//      statue = ReqSendTcp(MsgLength);
//      if (statue != SUCCESS)
//        return statue;

//      /* ������Ӵӻ��� */
//      statue = AddSlaveDev2Server(7 + (i - 1) * 3 + 1, SlaveChipIDString);
//      if (statue != SUCCESS)
//        return statue;
//    }
//  }

  /* �����ɼ������ݷ��� */
  WaterLevel = ((Flash_FifoBuf[5] >> 24) & 0x01);                                                                                 /* ˮλ */
  Bool2String(!WaterLevel, WaterLevelString);
  PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);                                                                                  /* ���� */
  Bool2String(!PowerDown, PowerDownString);     
  WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);                                       /* ���� */
  EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);                                      /* ���� */
  ph = (((Flash_FifoBuf[5] >> 10) & 0xf) + ((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);                                               /* ph */
  WaterTemp = ((Flash_FifoBuf[5] & 0x1f) << 2) + ((Flash_FifoBuf[6] >> 30)& 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);     /* ˮ�� */
  
  /* �趨�����͵���䳤�� */
  MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");                                                //��������֡��䳤��
  
  /* Ҫ��֪ͨ������������Ҫ���Ͳɼ������� */
//  statue = ReqSendTcp(MsgLength);
//  if (statue != SUCCESS)
//    return statue;
  
  /* �������ݰ�ǰ�� */
  statue = SendDatTCP2Server_Ready(MsgLength);
  if (statue != SUCCESS)
    return statue;
                                 
  /* �ϳ����������ϱ��������� */
  sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
      ChipID_String, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
  MsgLength = strlen((char*)MsgSendBuf);
  
  /* ������������ */
  statue = SendDat2Server(MsgSendBuf);  
  if (statue != SUCCESS)
    return statue;

    
  /* ���ӻ������ϱ��������� */
  for (i = 1; i <= Flash_FifoBuf[0]; i++)
  {
    /* ��ȡ��Ӧ�Ĵӻ��ɼ����� */
    WaterLevel = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 7) & 0x01);
    Bool2String(!WaterLevel, WaterLevelString);
    WaterSpeed = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
    EnvirTemp = ((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (i - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
    ph = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
    WaterTemp = (((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (i - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
    Chip2String(Flash_FifoBuf[7 + (i - 1) * 3], (char*)SlaveChipIDString);
    
    /* �趨�����͵���䳤�� */
    MsgLength = strlen("{\"type\":\"data\",\"data\":\"message\"}\r\n");
    
    /* Ҫ��֪ͨ������������Ҫ���Ͳɼ������� */
//    statue = ReqSendTcp(MsgLength);
//    if (statue != SUCCESS)
//      return statue;
    
    /* �������ݰ�ǰ�� */
    statue = SendDatTCP2Server_Ready(MsgLength);
    if (statue != SUCCESS)
      return statue;
    
    sprintf ((char*)MsgSendBuf, "{\"did\":\"%s\",\"ph\":%.1f,\"cputemp\":%.1f,\"sensortemp\":%.1f,\"speed\":%.1f,\"water\":%s,\"power\":%s}\r\n",
                SlaveChipIDString, ph, EnvirTemp, WaterTemp, WaterSpeed, WaterLevelString, PowerDownString);
    
    statue = SendDat2Server(MsgSendBuf);   //���ʹӻ�����
    if (statue != SUCCESS)
    return statue;
  }
    
  return SUCCESS;
}


/*
 * ��������GPRS_GPIO_Config
 * ����  ����ʼ��A6�Ŀ�������
 * ����  ����
 * ����  ����
 */
void GPRS_GPIO_Config(void)
{		
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_A6_Rst | RCC_APB2Periph_GPIOx_A6_PWR, ENABLE); 
												   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_A6_Rst;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_A6_Rst, &GPIO_InitStructure);	
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_A6_PWR;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		GPIO_Init(GPIOx_A6_PWR, &GPIO_InitStructure);	

    A6_Rst(ON);     //��λ�����߲���λ
    A6_PWR(OFF);    //���������Ϳ���
}

