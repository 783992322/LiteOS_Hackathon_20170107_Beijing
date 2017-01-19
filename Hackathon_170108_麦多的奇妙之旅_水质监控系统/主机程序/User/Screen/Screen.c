#include "common.h"

int8_t PageSerial = HomePage;         //��¼��ǰ���ĸ�ҳ��
int8_t DatPage = DisMastDat;          //��¼��ǰ��ʾ�����������Ǵӻ�����
int8_t TmpOffPage = DisMastTmpOff;    //��¼��ǰ��ʾ�����������Ǵӻ��¶Ȳ���
int8_t PHOffPage = DisMastPHOff;      //��¼��ǰ��ʾ�����������Ǵӻ�PH�궨

/*
 * ��������InitDis
 * ����  ����ʼ���������ʾ����ʾ
 * ����  ��str: Ҫ��ʾ���ַ�
 * ����  ����
 */
void InitDis (char* str)
{
  uint8_t SendBuf[50];
  sprintf ((char*)SendBuf, "t0.txt=\"%s\"", str);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);      //��Ļ����������������0xff������
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  Delay_ms(100);
}

/*
 * ��������Scr_SendCmd
 * ����  ������Ļ��������
 * ����  ��str: Ҫ���͵�����
 * ����  ����
 */
void Scr_SendCmd (uint8_t* str)
{
  USART1_SendBuf(str);
  USART1_SendByte(0xff);    
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
}

/*
 * ��������DisSensorVal
 * ����  ��������ʾҳ������ݸ���
 * ����  �� Sensor��Ҫ���µĴ���������
 *          color�� Ҫ��ʾ����ɫ
 *          val��   Ҫ��ʾ��ֵ
 * ����  ����
 */
void DisSensorVal (uint8_t Sensor, uint8_t color, float val)
{
  uint8_t SendBuf[30];                //���͸���Ļ��buf
  
  switch (Sensor)
  {
    /* ������ʾ */
    case SensorEnvirTemp:          
    {
      sprintf ((char*)SendBuf, "t16.txt=\"%.1f\"", val);    //�ϳ�����ֵ
      Scr_SendCmd(SendBuf);                                 //��������
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t16.pco=RED");          //�ϳɺ�ɫ
          Scr_SendCmd(SendBuf);                             //��������
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t16.pco=1024");         //�ϳ���ɫ
          Scr_SendCmd(SendBuf);                             //��������
          break;
        default:
          break;
      }
    }
      break;
    /* ˮ����ʾ */
    case SensorWaterTemp:        
    {
      sprintf ((char*)SendBuf, "t17.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t17.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t17.pco=1024");   
          Scr_SendCmd(SendBuf);        
          break;
        default:
          break;
      }
    }
      break;
    /* ��״̬��ʾ */
    case SensorWaterSpeed:
    {
      sprintf ((char*)SendBuf, "t18.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t18.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t18.pco=1024");  
          Scr_SendCmd(SendBuf);       
          break;
        default:
          break;
      }
    }
      break;
    /* ˮλ��ʾ */
    case SensorWaterLevel:
    {
      /* ���Ϊ0��ʾ��ʱ�ޱ����������ʾ�������� */
      if (!val)           //
      {
        sprintf ((char*)SendBuf, "t19.txt=\"����\""); 
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t19.pco=1024");  
        Scr_SendCmd(SendBuf); 
      }        
      /* ���ڱ�������ʾˮλ���� */
      else         
      {
        sprintf ((char*)SendBuf, "t19.txt=\"����\"");  
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t19.pco=RED"); 
        Scr_SendCmd(SendBuf); 
      }
    }
      break;
    /* PH��ʾ */
    case SensorPH:
    {
      sprintf ((char*)SendBuf, "t20.txt=\"%.1f\"", val);    
      Scr_SendCmd(SendBuf); 
      switch (color)
      {
        case Color_Red:
          sprintf ((char*)SendBuf, "t20.pco=RED");   
          Scr_SendCmd(SendBuf);       
          break;
        case Color_Green:
          sprintf ((char*)SendBuf, "t20.pco=1024");   
          Scr_SendCmd(SendBuf);        
          break;
        default:
          break;
      }
    }
      break;
    /* ��Դ״̬��ʾ */
    case SensorPower:
    {
      if (!val)
      {
        sprintf ((char*)SendBuf, "t21.txt=\"����\"");   
        Scr_SendCmd(SendBuf); 
        sprintf ((char*)SendBuf, "t21.pco=1024");
        Scr_SendCmd(SendBuf);  
      }
      else
      {
        sprintf ((char*)SendBuf, "t21.txt=\"����\"");   
        Scr_SendCmd(SendBuf);         
        sprintf ((char*)SendBuf, "t21.pco=RED");  
        Scr_SendCmd(SendBuf);  
      }
    }
      break;
    /* ��ʾ�·���ҳ���� */
    case SerialSerial:
    {
      if ((int)val == DisMastDat)
        sprintf ((char*)SendBuf, "t26.txt=\"����(%d/%d)\"", (int)val + 1, Flash_FifoBuf[0] + 1);
      else
        sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)val - 1) * 3] & 0xffff, (int)val + 1, Flash_FifoBuf[0] + 1);
      Scr_SendCmd(SendBuf);  
    }
      break;
    case Time_UpDat:
    {
      
//      extern int User_year, User_month, User_day, User_hour, User_min, User_sec;
      
//      sprintf ((char*)SendBuf, "t0.txt=\"%d\"", User_year);
//      Scr_SendCmd(SendBuf);  
//      sprintf ((char*)SendBuf, "t2.txt=\"%d\"", User_month);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t4.txt=\"%d\"", User_day);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t6.txt=\"%d\"", User_hour);
//      Scr_SendCmd(SendBuf); 
//      sprintf ((char*)SendBuf, "t8.txt=\"%d\"", User_min);
//      Scr_SendCmd(SendBuf); 
    }
      break;
    default:
      break;    
  }
}


/*
 * ��������FlashPage
 * ����  ��ҳ��ˢ��
 * ����  ����
 * ����  ����
 */
void FlashPage (void)
{
  switch (PageSerial)                               //�жϵ�ǰ����ҳ��
  {
    case HomePage:                                  //�ʼ�Ľ���������
      break;
    case DisDatPage:                                //�����ǰ��������ʾҳ��
    {
      switch (KeyStatue)                            //�жϰ��µ����ĸ���
      {
        case KeyLF:                                 //���
          DatPage--;                                //ҳ����ż�һ
          if (DatPage < 0)                          //����Ѿ�������ҳ��ĵ�һҳ
          {
            DatPage = 0;                           //ת����ֵ����ҳ��
//            Scr_SendCmd("page LimSet");   //��תҳ��
//            PageSerial = LimSetPage;                //����ҳ���־λ����
            KeyStatue = KeyDefault;                 //��������
//            UserDelay_KeyScan(100);
//            FlashLimSetPage(KeyDefault);            //�ֶ�ˢ����ֵҳ��
            return;
          }          
          break;
        case KeyRG:                                 //�Ҽ�
          DatPage++;                                //ҳ����ż�һ
          if (DatPage >= Flash_FifoBuf[0])          //�ж�ҳ���Ƿ������ұ�
            DatPage = Flash_FifoBuf[0];       
          break;
        default:
          break;
      }
      FlashDatPage();                               //ˢ��ҳ��
    }
      break;
    case LimSetPage:                                //�����������ֵҳ��
      {
//        FlashLimSetPage(KeyStatue);                 //������ֵҳ�溯��
      }
      break;
    case TimeSetPage:
//      FlashTimeSetPage(KeyStatue);                  //ˢ��ʱ������ҳ��
      break;
    case TmpOfficeSetPage:                          //ˢ���¶Ȳ���ҳ��
//      FlashTmpOffPage(KeyStatue); 
      break;
    case PHOfficeSetPage:                           //ˢ��PH�궨ҳ��
//      FlashPHOffPage(KeyStatue);
      break;
    default:
      break;
  }
  KeyStatue = KeyDefault;                           //������־λ��λ
}

/*
 * ��������TimeSetFlashPos
 * ����  ��ʱ�����ý���������ɫ����
 * ����  ��Pos_x��Ҫ�༭���ַ��������е�xλ��
 *         Pos_y��Ҫ�༭���ַ��������е�yλ��
 *         Color��Ҫ��ʾ����ɫ
 * ����  ����
 */
void TimeSetFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 0) && (Pos_y == 0))                 //��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t1.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t1.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t1.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 0))       //��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t3.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 2) && (Pos_y == 0))       //��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t5.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t5.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))     //Сʱ
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t7.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t7.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t7.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))       //����
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t9.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t9.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t9.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 2) && (Pos_y == 1))         //��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t11.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t11.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t11.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  
  if ((Pos_x == 1) && (Pos_y == 2))         //����
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t15.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t15.pco=32799");       //��ɫ
        break;
      default:
        break;
    }
  } 
  
  if ((Pos_x == 2) && (Pos_y == 2))       //��һҳ
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //��ɫ
        break;
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 2))       //��һҳ
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");       //��ɫ
        break;
      default:
        break;
    }
  }
}


/*
 * ��������TmpOffFlashPos
 * ����  ���¶Ȳ������ý���������ɫ����
 * ����  ��Pos_x��Ҫ�༭���ַ��������е�xλ��
 *         Pos_y��Ҫ�༭���ַ��������е�yλ��
 *         Color��Ҫ��ʾ����ɫ
 * ����  ����
 */
void TmpOffFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 0) && (Pos_y == 0))                 //ȷ��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
        Scr_SendCmd((uint8_t*)"t3.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Green:
        Scr_SendCmd((uint8_t*)"t3.pco=1024");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 0))                 //ȷ��
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))                 //��һ�豸
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t5.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))                 //��һ�豸
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t6.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 2))                 //��һҳ
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 2))                 //��һҳ
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //��ɫ
      default:
        break;
    }
  }
}


/*
 * ��������PHOffFlashPos
 * ����  ��PH�궨���ý���������ɫ����
 * ����  ��Pos_x��Ҫ�༭���ַ��������е�xλ��
 *         Pos_y��Ҫ�༭���ַ��������е�yλ��
 *         Color��Ҫ��ʾ����ɫ
 * ����  ����
 */
void PHOffFlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  if ((Pos_x == 1) && (Pos_y == 0))         //�궨
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 0) && (Pos_y == 1))                 //��һ�豸
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t5.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  
  if ((Pos_x == 1) && (Pos_y == 1))                 //��һ�豸
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t6.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  if ((Pos_x == 1) && (Pos_y == 2))                 //��һ�豸
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");       //��ɫ
      default:
        break;
    }
  }
  
}


/*
 * ��������FlashPHOffPage
 * ����  ��PH�궨ˢ��ҳ��
 * ����  ��Key_Statue������״̬
 * ����  ����
 */
void FlashPHOffPage (uint8_t Key_Statue)
{
  uint8_t statue = 0;
  float PH;
  uint8_t SendBuf[30];      //����Ļ���͵�buf  
  static int8_t WritePos[2] = {1, 2};   //������ڵ�    
  static int8_t WriteLastPos[2] = {1, 2};   //����ϴ����ڵ�
  static uint8_t IsWrite = 0;     //��д״̬
  float DifPH = 0.0;             //�¶Ȳ�ֵ
  static int8_t valPoint = 0;     //ҳ����
  static uint8_t FirstCommin = 1; //��һ�ν���ҳ��
  static uint8_t DemarcateChice = Neutral;    //��ʼΪ����
  
  static float PH_Acidity = 0.0;
  static float PH_Neutral = 0.0;
  static float PH_Basicity = 0.0;
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};   //ZGB��
  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN�����õ�����

  if (Key_Statue != KeyDefault)           //�����κ�һ��������֮ǰ���ı���ʾ����λ
  {
    Scr_SendCmd("t7.txt=\"\"");           //�ı�����ʾ��
  }
  if (PHOffGetRec_Neutral == SUCCESS)     //���Ա궨���
  {
    PHOffGetRec_Neutral = FAILUED;        //��λ��־λ
    Scr_SendCmd("t7.txt=\"������궨����\"");
    DemarcateChice = Acidity;             //���궨״̬��Ϊ����
    FirstCommin = 1;                      //��һ�ν���ҳ���־λ���漰��ҳ�����ݵĳ�ʼ��
  }
  if (PHOffGetRec_Acidity == SUCCESS)
  {
    PHOffGetRec_Acidity = FAILUED;
    Scr_SendCmd("t7.txt=\"������궨����\"");
    DemarcateChice = Basicity;
    FirstCommin = 1;
  }
  if (PHOffGetRec_Basicity == SUCCESS)
  { 
    PHOffGetRec_Basicity = FAILUED;
    Scr_SendCmd("t7.txt=\"�궨�ɹ�!\"");
    Delay_ms(1000);

    FirstCommin = 1;                  //����ҳ����Ϣ��ʼ��
    WritePos[0] = 1;          
    WritePos[1] = 2;
    WriteLastPos[0] = 1;
    WriteLastPos[1] = 2;
    IsWrite = 0;
    valPoint = 0;
    DemarcateChice = Neutral;
    FirstCommin = 1;
    PHOffPage = DisMastPHOff;
    
    PageSerial = DisDatPage;          //��ת��������ʾҳ��
    Scr_SendCmd((uint8_t*)"page DisDat");
    
    return;
  }
  //��ʾ�е�PHֵ
  switch (DemarcateChice)
  {
    case Acidity:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "4.00");        //��ֵ��ȥ��Ļ��ӡ
      Scr_SendCmd(SendBuf);
      break;
    case Neutral:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "6.86");        //��ֵ��ȥ��Ļ��ӡ
      Scr_SendCmd(SendBuf);
      break;
    case Basicity:
      sprintf ((char*)SendBuf, "t3.txt=\"%s\"", "9.18");        //��ֵ��ȥ��Ļ��ӡ
      Scr_SendCmd(SendBuf);
      break;
    default:
      break;
  }
  
  if (PHOffPage == DisMastPHOff)                                //�����ʾ��������PHֵ
  {
    if(FirstCommin)
    {
      PH = ((Flash_FifoBuf[5] >> 10) & 0xf) + (((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
      sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", PH);          //��ֵ��ȥ��Ļ��ӡ
      Scr_SendCmd(SendBuf);
    }

    /* ��ʾҳ�����·���ҳ�� */
    sprintf ((char*)SendBuf, "t26.txt=\"����(%d/%d)\"", (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  else
  {
    if(FirstCommin)
    {
      PH = (((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
      sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", PH);          //��ֵ��ȥ��Ļ��ӡ
      Scr_SendCmd(SendBuf);
    }
    
    sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xffff, (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  
  switch (Key_Statue)           //�жϰ���
  {
    case KeyDefault:
      break;
    case KeyLF:                 //����  x�����һ
      if ((WritePos[1] == 1) && (WritePos[0] > 0))
      {
        WritePos[0]--;
      }
      break;
    case KeyUp:                 //����  y�����һ
      if (WritePos[1] == 1)
      {
        WritePos[1]--;
        WritePos[0] = 1;
      }
      else if (WritePos[1] > 0)
        WritePos[1]--;
      break;
    case KeyRG:                 //����  x�����һ
      if (WritePos[0] < 1)      //����Ϊ����
        WritePos[0]++;
      break;
    case KeyDn:                 //����  y�����һ
      if (WritePos[1] == 1)
      {
        WritePos[1]++;
        WritePos[0] = 1;
      }
      if (WritePos[1] < 2)      //����Ϊ����
          WritePos[1]++;
      break;
    case KeyOK:
      if ((WritePos[1] == 2) && (WritePos[0] == 1))      //��һҳ
      {
        PageSerial = TmpOfficeSetPage;
        Scr_SendCmd((uint8_t*)"page TempOffsetSet");
        FlashTmpOffPage(KeyDefault);
        
        WritePos[0] = 1;          
        WritePos[1] = 2;
        WriteLastPos[0] = 1;
        WriteLastPos[1] = 2;
        IsWrite = 0;
        valPoint = 0;
        DemarcateChice = Neutral;
        FirstCommin = 1;
        PHOffPage = DisMastPHOff;
      }

      if ((WritePos[1] == 0) && (WritePos[0] == 1))   //�궨
      {
        if (PHOffPage == DisMastPHOff)
        {
          switch (DemarcateChice)
          {
            Scr_SendCmd("t2.txt=\"���ڱ궨\"");
            PH_Neutral = GetPH();
            Delay_ms(200);
            
            case Neutral:     //����
              DemarcateChice = Acidity;
              Scr_SendCmd("t2.txt=\"�Եȼ����궨����\"");
              break;
            case Acidity:
              DemarcateChice = Basicity;
              Scr_SendCmd("t2.txt=\"�Եȼ����궨����\"");
              break;
            case Basicity:
              DemarcateChice = Neutral;
//              OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity));      //���Ժ�����֮��
//              OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral));     //���Ժͼ���֮��            
            
              /* �����PH���Ժͼ���б�� */
              statue = UpOffsetDat(PH_BN_Offset, ((9.18 - 6.86) / (PH_Basicity - PH_Neutral)));
              statue = UpOffsetDat(PH_AN_Offset, ((6.86 - 4.00) / (PH_Neutral - PH_Acidity)));
            
              OfficDatBuf[3] = ((6.86 - 4.00) / (PH_Neutral - PH_Acidity)) * 1000000;
              OfficDatBuf[4] = ((9.18 - 6.86) / (PH_Basicity - PH_Neutral)) * 1000000; 
            
              Scr_SendCmd("t7.txt=\"�����ϴ�\"");
              if(TCPIsUsed == 0)
              {
                TCPIsUsed = 1;
                TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
                LED_Green(ON);                //����ɫָʾ��
                
                statue = UpOffsetDat(PH_BN_Offset, OfficDatBuf[4]);   //��������
                
                LED_Green(OFF);               //�ر���ɫָʾ��
                TIM_Cmd(TIM2, ENABLE);
                TCPIsUsed = 0;  
              }
              else
              {
                Scr_SendCmd("t7.txt=\"��ǰ����æ�����Ժ�����\"");
              }
              
              
              if(TCPIsUsed == 0)
              {
                TCPIsUsed = 1;
                TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
                LED_Green(ON);                //����ɫָʾ��
                
                statue = UpOffsetDat(PH_AN_Offset, OfficDatBuf[3]);
                
                if (statue == SUCCESS)
                Scr_SendCmd("t2.txt=\"�궨�ɹ�\"");
                else
                  Scr_SendCmd("t7.txt=\"�����ϴ�ʧ�ܣ�������\"");
              
                LED_Green(OFF);               //�ر���ɫָʾ��
                TIM_Cmd(TIM2, ENABLE);
                TCPIsUsed = 0;  
              }
              else
              {
                Scr_SendCmd("t7.txt=\"��ǰ����æ�����Ժ�����\"");
              }
              
              Delay_ms(1000);
            
              WritePos[0] = 1;              //��ʼ����־λ
              WritePos[1] = 2;
              WriteLastPos[0] = 1;
              WriteLastPos[1] = 2;
              IsWrite = 0;
              valPoint = 0;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              PHOffPage = DisMastPHOff;
              
              PageSerial = DisDatPage;      //��תҳ�浽������ʾҳ��
              Scr_SendCmd((uint8_t*)"page DisDat");

              FlashPage();
              break;
            default:
              break;
          }
        }
        else
        {
          if (Connect_Way == USE_CAN)
          {
            Can_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
            Can_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
            Can_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
            Can_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
            Can_DatBuf[5] = 0xff;
            Can_DatBuf[6] = 0xff;
            switch (DemarcateChice)
            {
              Scr_SendCmd("t2.txt=\"���ڱ궨\"");
              case Neutral:     //����
                Can_DatBuf[7] = 0xd0;
                break;
              case Acidity:
                Can_DatBuf[7] = 0xd1;
                break;
              case Basicity:
                Can_DatBuf[7] = 0xd2;
                break;
              default:
                break;
            }
            Can_Send_Msg(Can_DatBuf, 8);   
            FirstCommin = 0;
          }
          else
          {
            ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
            ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
            ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
            ZGB_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
            ZGB_DatBuf[5] = 0xff;
            ZGB_DatBuf[6] = 0xff;
            ZGB_DatBuf[7] = 0xff;
            ZGB_DatBuf[8] = 0xff;
            ZGB_DatBuf[9] = 0xff;
            ZGB_DatBuf[11] = 0x02;
            ZGB_DatBuf[12] = 0xcc;
            ZGB_DatBuf[13] = 0xcc;
            switch (DemarcateChice)
            {
              Scr_SendCmd("t2.txt=\"���ڱ궨\"");
              case Neutral:     //����
                ZGB_DatBuf[10] = 0x01;
                break;
              case Acidity:
                ZGB_DatBuf[10] = 0x02;
                break;
              case Basicity:
                ZGB_DatBuf[10] = 0x03;
                break;
              default:
                break;
            }
            USART2_SendBuf(ZGB_DatBuf);       //��ӻ�����У���
            USART2_SendByte('\n');     
            FirstCommin = 0;
            
            Scr_SendCmd("t7.txt=\"��15s�޷�Ӧ�����µ���궨��ť\"");
          }
        }
      }
      
      if (WritePos[1] == 1)      //��һ�豸����һ�豸
      {
        switch (WritePos[0])
        {
          case 0:     //��һ�豸
            if (valPoint > 0)
            {
              valPoint--;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              Scr_SendCmd("t2.txt=\"���ڻ�ȡ\"");
            }
            if (valPoint == 0)
            {
              PHOffPage = DisMastPHOff;
            }                
            break;
          case 1:     //��һ�豸
            if (valPoint < Flash_FifoBuf[0])
            {
              valPoint++;
              DemarcateChice = Neutral;
              FirstCommin = 1;
              Scr_SendCmd("t2.txt=\"���ڻ�ȡ\"");
              PHOffPage = !DisMastPHOff;
            }
            break;
        }
      }
      break;
  }
  
  if (Key_Statue != KeyOK)                                                //������µĲ���OK��
  {
    if ((WriteLastPos[1] == 1) || (WriteLastPos[1] == 2))                 //�������һҳ�������ɫ����ɫ��
      PHOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
    else
    {
      PHOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //�ָ�֮ǰ���ڵİ���ҹɫ
    }   
    
    PHOffFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //���������ڵĿռ��ɻ�ɫ
    WriteLastPos[0] = WritePos[0];                                        //������һ������λ��
    WriteLastPos[1] = WritePos[1];
  }
}


/*
 * ��������FlashTmpOffPage
 * ����  ���¶Ȳ���ˢ��ҳ��
 * ����  ��Key_Statue������״̬
 * ����  ����
 */
void FlashTmpOffPage (uint8_t Key_Statue)
{
  uint8_t statue = 0;
  float WaterTemp;
  uint8_t SendBuf[30];      //����Ļ���͵�buf  
  static int8_t WritePos[2] = {0, 2};   //������ڵ�    
  static int8_t WriteLastPos[2] = {0, 2};   //����ϴ����ڵ�
  static uint8_t IsWrite = 0;     //��д״̬
  static float TmpSetVal = 0;   //ʵ���¶�����ֵ  
  float DifTmp = 0.0;             //�¶Ȳ�ֵ
  static int8_t valPoint = 0;     //ҳ����
  static uint8_t FirstCommin = 1; //��һ�ν���ҳ��
  
  u8 ZGB_DatBuf[14] = {0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};   //ZGB��
  u8 Can_DatBuf[8] = {0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};    //CAN�����õ�����
  
  if (TmpOffGetRec == SUCCESS)
  {
    Scr_SendCmd("t7.txt=\"���óɹ�!\"");
    
    Delay_ms(1000);
    Scr_SendCmd("t7.txt=\"���ݽ���һ��ʱ������\"");
    TmpOffGetRec = FAILUED;
    Delay_ms(1000);
    
    FirstCommin = 1;
    WritePos[0] = 0;          
    WritePos[1] = 2;
    WriteLastPos[0] = 0;
    WriteLastPos[1] = 2;
    IsWrite = 0;
    valPoint = 0;
    TmpOffPage = DisMastTmpOff;
    
    PageSerial = DisDatPage;
    Scr_SendCmd((uint8_t*)"page DisDat");
    return;
  }
  
  if (TmpOffPage == DisMastTmpOff)    //�����ʾ���������¶�ֵ
  {
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30) & 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);
    sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", WaterTemp);        //��ֵ��ȥ��Ļ��ӡ
    Scr_SendCmd(SendBuf);

    if (FirstCommin)
    {
      FirstCommin = 0;
      TmpSetVal = WaterTemp;
    }
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);        //��ֵ��ȥ��Ļ��ӡ
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t26.txt=\"����(%d/%d)\"", (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);
  }
  else        //�����ʾ���Ǵӻ����¶�
  {
    sprintf ((char*)SendBuf, "t26.txt=\"%04X(%d/%d)\"",Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xffff, (int)valPoint + 1, Flash_FifoBuf[0] + 1);
    Scr_SendCmd(SendBuf);

    WaterTemp = (((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (valPoint - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
    sprintf ((char*)SendBuf, "t2.txt=\"%.1f\"", WaterTemp);        //��ֵ��ȥ��Ļ��ӡ
    Scr_SendCmd(SendBuf);
    
    if (FirstCommin)
    {
      FirstCommin = 0;
      TmpSetVal = WaterTemp;
    }
    
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);        //��ֵ��ȥ��Ļ��ӡ
    Scr_SendCmd(SendBuf);
  }
  
  if(Key_Statue != KeyDefault)
  {
    Scr_SendCmd("t7.txt=\"\"");
  }
  if (IsWrite == 0)               //�ƶ����״̬
  {
    switch (Key_Statue)           //�жϰ���
    {
      case KeyDefault:
        break;
      case KeyLF:                 //����  x�����һ
          if (WritePos[0] > 0)
            WritePos[0]--;
        break;
      case KeyUp:                 //����  y�����һ
          if (WritePos[1] > 0)
          WritePos[1]--;
        break;
      case KeyRG:                 //����  x�����һ
        if (WritePos[0] < 1)      //����Ϊ����
          WritePos[0]++;
        break;
      case KeyDn:                 //����  y�����һ
        if (WritePos[1] < 2)      //����Ϊ����
            WritePos[1]++;
        break;
      case KeyOK:
        if (WritePos[1] == 2)      //��һҳ����һҳ
        {
          switch (WritePos[0])
          {
            case 0:
              PageSerial = PHOfficeSetPage;
              Scr_SendCmd((uint8_t*)"page PHOffsetSet");
              FlashPHOffPage(KeyDefault);
              break;
            case 1:
              PageSerial = TimeSetPage;
              Scr_SendCmd((uint8_t*)"page TimeSet");
              FlashTimeSetPage(KeyDefault); 
              break;
          }
          FirstCommin = 1;
          WritePos[0] = 0;          
          WritePos[1] = 2;
          WriteLastPos[0] = 0;
          WriteLastPos[1] = 2;
          IsWrite = 0;
          valPoint = 0;
          TmpOffPage = DisMastTmpOff;
        }
        if ((WritePos[1] == 0) && (WritePos[0] == 1))   //ȷ��
        {
          if (TmpOffPage == DisMastTmpOff)    //������������µ�
          {
            DifTmp = TmpSetVal - WaterTemp;
            DifTmp += OfficDatBuf[0] / 10.0;
            
            Scr_SendCmd("t7.txt=\"�����ϴ�\"");
            
            if(TCPIsUsed == 0)
            {
              TCPIsUsed = 1;
              TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
              LED_Green(ON);                //����ɫָʾ��
              
              statue = UpOffsetDat(Tmp_Offset, DifTmp);
              
              if (statue == SUCCESS)
              {
                OfficDatBuf[0] = DifTmp * 10;
                Scr_SendCmd("t7.txt=\"�Ѳ������\"");
              }
              else
                Scr_SendCmd("t7.txt=\"�����ϴ�ʧ�ܣ�������\"");
              
              UserDelay_KeyScan(1000);
              
              LED_Green(OFF);               //�ر���ɫָʾ��
              TIM_Cmd(TIM2, ENABLE);
              TCPIsUsed = 0;  
            }
            else
            {
              Scr_SendCmd("t7.txt=\"��ǰ����æ�����Ժ�����\"");
              Delay_ms(1000);
            }
            
            FirstCommin = 1;
            WritePos[0] = 0;
            WritePos[1] = 2;
            WriteLastPos[0] = 0;
            WriteLastPos[1] = 2;
            IsWrite = 0;
            valPoint = 0;
            TmpOffPage = DisMastTmpOff;
            PageSerial = DisDatPage;
            Scr_SendCmd((uint8_t*)"page DisDat");
            return;
          }
          else        //�ӻ��¶�����
          {
            DifTmp = TmpSetVal - WaterTemp;
            DifTmp = DifTmp * 10;    //�Ŵ�ʮ��
            TmpOffGetRec = FAILUED;   //��־λ��λ
            
            if (Connect_Way == USE_CAN)   //�����CAN��ʽͨ��
            {
              Can_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
              Can_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
              Can_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
              Can_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
              Can_DatBuf[5] = (((int)DifTmp >> 24) & 0xff);
              Can_DatBuf[6] = (((int)DifTmp >> 16) & 0xff);
              Can_DatBuf[7] = 0xc0;
              Can_Send_Msg(Can_DatBuf, 8);
              
              UserDelay_KeyScan(100);
              Can_DatBuf[5] = (((int)DifTmp >> 8) & 0xff);
              Can_DatBuf[6] = ((int)DifTmp & 0xff);
              Can_DatBuf[7] = 0xc1;
              Can_Send_Msg(Can_DatBuf, 8);

              Scr_SendCmd("t7.txt=\"��15s�޷�Ӧ�����µ��������ť\"");
            }
            else    //ZGB��ʽͨ��
            {
              ZGB_DatBuf[1] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 24) & 0xff);
              ZGB_DatBuf[2] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 16) & 0xff);
              ZGB_DatBuf[3] = ((Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] >> 8) & 0xff);
              ZGB_DatBuf[4] = (Flash_FifoBuf[7 + ((int)valPoint - 1) * 3] & 0xff);
              if (DifTmp < 0)
              {
                DifTmp = -DifTmp;
                ZGB_DatBuf[9] = 0x01;
              }
              else
                ZGB_DatBuf[9] = 0x02;
                
              ZGB_DatBuf[5] = ((((int)DifTmp >> 24) & 0xff) + 1);
              ZGB_DatBuf[6] = ((((int)DifTmp >> 16) & 0xff) + 1);
              ZGB_DatBuf[7] = ((((int)DifTmp >> 8) & 0xff) + 1);
              ZGB_DatBuf[8] = (((int)DifTmp & 0xff) + 1);
              ZGB_DatBuf[10] = 0xff;
              ZGB_DatBuf[11] = 0x01;
              ZGB_DatBuf[12] = 0xcc;
              ZGB_DatBuf[13] = 0xcc;
              
              USART2_SendBuf(ZGB_DatBuf);       //��ӻ�����У���
              USART2_SendByte('\n');
              
              Scr_SendCmd("t7.txt=\"������ӻ�ͨ��\"");
              UserDelay_KeyScan(500);
              
              Scr_SendCmd("t7.txt=\"��15s�޷�Ӧ�����µ��������ť\"");
            }
          }
        }
        
        if (WritePos[1] == 1)      //��һ�豸����һ�豸
        {
          switch (WritePos[0])
          {
            case 0:     //��һ�豸
              
              if (valPoint > 0)
              {
                valPoint--;
                FirstCommin = 1;
                Scr_SendCmd("t2.txt=\"���ڻ�ȡ\"");
                Scr_SendCmd("t3.txt=\"���ڻ�ȡ\"");
              }
              if (valPoint == 0)
              {
                TmpOffPage = DisMastTmpOff;
              }                
              break;
            case 1:     //��һ�豸
              if (valPoint < Flash_FifoBuf[0])
              {
                valPoint++;
                FirstCommin = 1;
                Scr_SendCmd("t2.txt=\"���ڻ�ȡ\"");
                Scr_SendCmd("t3.txt=\"���ڻ�ȡ\"");
                TmpOffPage = !DisMastTmpOff;
              }
              break;
          }
        }

        if ((WritePos[1] == 0) && (WritePos[0] == 0))     //���µ���ʵ��ˮ�µ�ֵ
        {
          TmpOffFlashPos(WritePos[0], WritePos[1], Color_Red);
          IsWrite = 1;     
        }
        break;
       }
      if (Key_Statue != KeyOK)                                                //������µĲ���OK��
      {
        if ((WriteLastPos[1] == 1) || (WriteLastPos[1] == 2))                 //�������һҳ�������ɫ����ɫ��
          TmpOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
        else
        {
          TmpOffFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //�ָ�֮ǰ���ڵİ���ҹɫ
        }   
        
        TmpOffFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //���������ڵĿռ��ɻ�ɫ
        WriteLastPos[0] = WritePos[0];                                        //������һ������λ��
        WriteLastPos[1] = WritePos[1];
      }
  }
  else
  {
    /* ʵ��ˮ�� */
    if ((WritePos[0] == 0) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          TmpSetVal += 0.5;                                                      
          if (TmpSetVal > 100)                                                
            TmpSetVal = 100; 
        
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);               
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          TmpSetVal -= 0.5;
          if (TmpSetVal < 0)
            TmpSetVal = 0;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", TmpSetVal);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * ��������FlashTimeSetPage
 * ����  ��ʱ������ҳ��ˢ��
 * ����  ����
 * ����  ����
 */
void FlashTimeSetPage (uint8_t Key_Statue)
{
  static int8_t WritePos[2] = {0, 2};
  static int8_t WriteLastPos[2] = {0, 2};
  static uint8_t IsWrite = 0;
  uint8_t SendBuf[50];
  static uint8_t FistCominTemp = 1;
  static int year, month, day, hour, minute, second;
  
  if (FistCominTemp)
  {
    sprintf ((char*)SendBuf, "t14.txt=\"%s\"", ChipID_String);    //��ʾ�豸ID
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", calendar.w_year);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", calendar.w_month);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", calendar.w_date);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", calendar.hour);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", calendar.min);
    Scr_SendCmd(SendBuf);
    
    sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", calendar.sec);
    Scr_SendCmd(SendBuf);
    
    year = calendar.w_year;
    month = calendar.w_month;
    day = calendar.w_date;
    hour = calendar.hour;
    minute = calendar.min;
    second = calendar.sec;
    
    FistCominTemp = 0;
  }
  
  if (IsWrite == 0)               //�ƶ����״̬
  {
    switch (Key_Statue)           //�жϰ���
    {
      case KeyDefault:
        break;
      case KeyLF:                 //����  x�����һ
          if (WritePos[0] > 0)
            WritePos[0]--;
//          if ((WritePos[0] == 1) && (WritePos[1] == 2))
//            WritePos[0] = 0;
        break;
      case KeyUp:                 //����  y�����һ
          if (WritePos[1] > 0)
          WritePos[1]--;
        break;
      case KeyRG:                 //����  x�����һ
        if (WritePos[0] < 2)      //����Ϊ����
          WritePos[0]++;
//        if ((WritePos[0] == 1) && (WritePos[1] == 2))
//          WritePos[0] = 2;
        break;
      case KeyDn:                 //����  y�����һ
        if (WritePos[1] < 2)      //����Ϊ����
            WritePos[1]++;
//        if ((WritePos[0] == 1) && (WritePos[1] == 2))
//          WritePos[1]--;
        break;
      case KeyOK:
        if (WritePos[1] == 2)     //�������һҳ
        {
          switch (WritePos[0])
          {
            case 0:
              PageSerial = TmpOfficeSetPage;
              Scr_SendCmd((uint8_t*)"page TempOffsetSet");
              FlashTmpOffPage(KeyDefault);
              break;
            case 1:
              RTC_Set(year, month, day, hour, minute, second);
              PageSerial = DisDatPage;
              DatPage = 0; 
              Scr_SendCmd((uint8_t*)"page DisDat");
              FlashDatPage(); 
              break;
            case 2:
              PageSerial = LimSetPage;
              Scr_SendCmd((uint8_t*)"page LimSet");
              FlashLimSetPage(KeyDefault);
              break;
          }
//          if(WritePos[0] == 0)    //����
//          {
//            RTC_Set(year, month, day, hour, minute, second);
//            PageSerial = DisDatPage;
//            DatPage = 0; 
//            Scr_SendCmd((uint8_t*)"page DisDat");
//            FlashDatPage(); 
//          }
//          else                    //��һҳ
//          {
//            PageSerial = LimSetPage;
//            Scr_SendCmd((uint8_t*)"page LimSet");
//            FlashLimSetPage(KeyDefault);
//          }
          FistCominTemp = 1;
          WritePos[0] = 0;          
          WritePos[1] = 2;
          WriteLastPos[0] = 0;
          WriteLastPos[1] = 2;
        }
        else
        {
          TimeSetFlashPos(WritePos[0], WritePos[1], Color_Red);
          IsWrite = 1;     
        }
        
        break;
    }
    
    if (Key_Statue != KeyOK)                                                //������µĲ���OK��
    {
      if (WriteLastPos[1] == 2)                                             //�������һҳ�������ɫ����ɫ��
        TimeSetFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
      else
      {
        TimeSetFlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);     //�ָ�֮ǰ���ڵİ���ҹɫ
      }   
      
      TimeSetFlashPos(WritePos[0], WritePos[1], Color_Yellow);              //���������ڵĿռ��ɻ�ɫ
      WriteLastPos[0] = WritePos[0];                                        //������һ������λ��
      WriteLastPos[1] = WritePos[1];
    }
  }
  else
  {
    /* ��� */
    if ((WritePos[0] == 0) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          year += 1;                                                      
          if (year > 2099)                                                
            year = 2000; 
        
          sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", year);               
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          year -= 1;
          if (year < 2000)
            year = 2099;
          
          sprintf ((char*)SendBuf, "t1.txt=\"%4d\"", year);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t1.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* �·� */
    if ((WritePos[0] == 1) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          month += 1;                                           
          if (month > 12)                                     
            month = 1; 
          
          if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
            day = 30;
            
          sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", month);   
          Scr_SendCmd(SendBuf);
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          month -= 1;
          if (month < 1)
            month = 12;
          
          if (((month == 2) || (month == 4) || (month == 6) || (month == 9) || (month == 11)) && (day == 31))
            day = 30;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%2d\"", month);  
          Scr_SendCmd(SendBuf);
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* ���� */
    if ((WritePos[0] == 2) && (WritePos[1] == 0))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          day += 1;                                           
          if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
          {
            if (day > 31)
              day = 1;
          }
          else
          {
            if (day > 30)
              day = 1;
          }        
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          day -= 1;
          if ((month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 12))
          {
            if (day < 1)
              day = 31;
          }
          else
          {
            if (day < 1)
              day = 30;
          }  
          
          sprintf ((char*)SendBuf, "t5.txt=\"%2d\"", day);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t5.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* Сʱ */
    if ((WritePos[0] == 0) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          hour += 1;                                           
          if (hour > 23)                                      
            hour = 0; 
        
          sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", hour);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          hour -= 1;
          if (hour < 0)
            hour = 23;
          
          sprintf ((char*)SendBuf, "t7.txt=\"%2d\"", hour);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t7.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    /* ���� */
    if ((WritePos[0] == 1) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          minute += 1;                                           
          if (minute > 59)                                     
            minute = 0; 
        
          sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", minute);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          minute -= 1;
          if (minute < 0)
            minute = 59;
          
          sprintf ((char*)SendBuf, "t9.txt=\"%2d\"", minute);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t9.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    
    if ((WritePos[0] == 2) && (WritePos[1] == 1))  
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          second += 1;                                       
          if (second > 59)                                   
            second = 0; 
        
          sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", second);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          second -= 1;
          if (second < 0)
            second = 59;
          
          sprintf ((char*)SendBuf, "t11.txt=\"%2d\"", second);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t11.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * ��������LimPage_FlashPos
 * ����  ����ֵ����ҳ���������ɫ��ʾ
 * ����  ��Pos_x��Ҫ�޸ĵĿؼ���x����λ��
 *         Pos_y��Ҫ�޸ĵĿռ��y����λ��
 *         Color����ʾ����ɫ
 * ����  ����
 */
void LimPage_FlashPos (int8_t Pos_x, int8_t Pos_y, uint8_t Color)
{
  /* �������� */
  if ((Pos_x == 0) && (Pos_y == 0))  
  {
    switch(Color)
    {
      case Color_Yellow:  
          Scr_SendCmd((uint8_t*)"t3.pco=YELLOW");   //��ʾ��ɫ
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t3.pco=RED");      //��ʾ��ɫ
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t3.pco=BLACK");      //��ʾ��ɫ
      default:
        break;
    }
  }   
  /* �������� */
  if ((Pos_x == 1) && (Pos_y == 0))
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t4.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t4.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t4.pco=BLACK");
      default:
        break;
    }
  } 
  /* ˮ������ */
  if ((Pos_x == 0) && (Pos_y == 1)) 
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t5.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t5.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t5.pco=BLACK");
      default:
        break;
    }
  } 
  /* ˮ������ */
  if ((Pos_x == 1) && (Pos_y == 1))
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t6.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t6.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t6.pco=BLACK");
      default:
        break;
    }
  } 
  /* ��״̬���� */
  if ((Pos_x == 0) && (Pos_y == 2))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t7.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t7.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t7.pco=BLACK");
      default:
        break;
    }
  } 
  /* ��״̬���� */
  if ((Pos_x == 1) && (Pos_y == 2)) 
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t8.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t8.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t8.pco=BLACK");
      default:
        break;
    }
  } 
  /* PHֵ���� */
  if ((Pos_x == 0) && (Pos_y == 3))   
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t9.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t9.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t9.pco=BLACK");
      default:
        break;
    }
  } 
  /* PHֵ���� */
  if ((Pos_x == 1) && (Pos_y == 3))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t13.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t13.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t13.pco=BLACK");
      default:
        break;
    }
  } 
  /* ���� */
  if ((Pos_x == 0) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t15.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t15.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t15.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t15.pco=32799");   //��ɫ
        break;
      default:
        break;
    }
  } 
  /* ��һҳ */
  if ((Pos_x == 1) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t23.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t23.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t23.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t23.pco=32799");   //��ɫ
        break;
      default:
        break;
    }
  } 
  /* ��һҳ */
  if ((Pos_x == -1) && (Pos_y == 4))  
  {
    switch(Color)
    {
      case Color_Yellow:
          Scr_SendCmd((uint8_t*)"t22.pco=YELLOW");
        break;
      case Color_Red:
          Scr_SendCmd((uint8_t*)"t22.pco=RED");
        break;
      case Color_Black:
        Scr_SendCmd((uint8_t*)"t22.pco=BLACK");
      case Color_Popul:
        Scr_SendCmd((uint8_t*)"t22.pco=32799");   //��ɫ
        break;
      default:
        break;
    }
  }
}

/*
 * ��������FlashLimSetPage
 * ����  ��ˢ����ֵ����ҳ��
 * ����  ��Key_Statue�����µİ���
 * ����  ����
 */
void FlashLimSetPage (uint8_t Key_Statue)
{
  static int8_t WritePos[2] = {-1, 4};          //[0]��ʾX����  [1]��ʾY����
  static int8_t WriteLastPos[2] = {-1, 4};      //[0]��ʾ��һ��X����  [1]��ʾ��һ��Y����
  static uint8_t IsWrite = 0;                   //�û������趨    0:��    1����
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;       //������������
  static float PHMax_Set, PHMin_Set, WaterTempMax_Set, WaterTempMin_Set, EnvirTempMax_Set, EnvirTempMin_Set, WaterSpeedMax_Set, WaterSpeedMin_Set; //������������
  uint8_t SendBuf[15];                          //Ҫ���͸���Ļ���ַ���
  static uint8_t FistCominTemp = 1;             //�Ƿ��ǽ����ҳ�������֮ǰ���Ѿ��ڱ�ҳ��
  uint8_t TempInt, TempFloat;                   //�����ֵʱ����ʱ����
  
  if (FistCominTemp)                            //������ν��뺯���ǵ�һ�δ�ҳ��
  {
    /* �Ȼ�ȡ���õĸ������������� */
    PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
    PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
    WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
    WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
    EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
    EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
    WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
    WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
    
    /* ����ֵ��Ϣ��ֵ���趨ֵ */
    PHMax_Set = PHMax;
    PHMin_Set = PHMin;
    WaterTempMax_Set = WaterTempMax;
    WaterTempMin_Set = WaterTempMin;
    EnvirTempMax_Set = EnvirTempMax;
    EnvirTempMin_Set = EnvirTempMin;
    WaterSpeedMax_Set = WaterSpeedMax;
    WaterSpeedMin_Set = WaterSpeedMin;
    
    /* ����ǰ��ֵ��ʾ */
    sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);
    Scr_SendCmd(SendBuf);
    sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);
    Scr_SendCmd(SendBuf);
    
    FistCominTemp = 0;                              //��λ��־λ����һ��ˢ��ҳ��ʱ����������Щ
  }

  if (IsWrite == 0)                                 //��������ڱ༭״̬�������ƶ����
  {
    switch (Key_Statue)                             //�жϰ���
    {
      case KeyDefault:
        break;
      case KeyLF:                                   //����   x�����һ
        if (WritePos[1] == 4)
        {
          if (WritePos[0] > -1)
            WritePos[0]--;
        }
        else
        {
          if (WritePos[0] > 0)
            WritePos[0]--;
        }
        break;
      case KeyUp:                                   //����  y�����һ
        if ((WritePos[0] != -1) || (WritePos[1] != 4))
        {
          if (WritePos[1] > 0)
          WritePos[1]--;
        }
        break;
      case KeyRG:                                   //����  x�����һ
        if (WritePos[0] < 1)                        //����Ϊ����
          WritePos[0]++;
        break;
      case KeyDn:                                   //����  y�����һ
        if (WritePos[1] < 4)                        //����Ϊ����
          WritePos[1]++;
        break;
      case KeyOK:
        if(WritePos[1] == 4)                        //������ڱ������һҳ��һ��
        {
          if (WritePos[0] == 0)                     //����
          {
            /* �������ޱ��� */
            TempInt = (int)EnvirTempMax_Set;
            TempFloat = ((int)(EnvirTempMax_Set * 10) % 10); 
            Flash_FifoBuf[2] &= ~(0xff << 16);
            Flash_FifoBuf[2] |= ((TempInt & 0xff) << 16);                   //��������
            Flash_FifoBuf[2] &= ~(0xf << 12);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 12);
            
            /* �������ޱ��� */
            TempInt = (int)EnvirTempMin_Set;
            TempFloat = ((int)(EnvirTempMin_Set * 10) % 10); 
            Flash_FifoBuf[2] &= ~(0xff << 4);
            Flash_FifoBuf[2] |= ((TempInt & 0xff) << 4);                    //��������
            Flash_FifoBuf[2] &= ~(0xf << 0);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 0);
            
            /* ˮ�����ޱ��� */
            TempInt = (int)WaterTempMax_Set;
            TempFloat = ((int)(WaterTempMax_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 8);
            Flash_FifoBuf[1] |= ((TempInt & 0xff) << 8);                    //��������        
            Flash_FifoBuf[1] &= ~(0xf << 4);
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 4);
          
            /* ˮ�����ޱ��� */
            TempInt = (int)WaterTempMin_Set;
            TempFloat = ((int)(WaterTempMin_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xf);
            Flash_FifoBuf[1] |= ((TempInt >> 4)& 0xf);                      //��������
            Flash_FifoBuf[2] &= ~(0xff << 24);
            Flash_FifoBuf[2] |= ((TempInt & 0xf) << 28);
            Flash_FifoBuf[2] |= ((TempFloat & 0xf) << 24);      
      
            /* ��״̬���ޱ��� */
            TempInt = (int)WaterSpeedMax_Set;
            TempFloat = ((int)(WaterSpeedMax_Set * 10) % 10); 
            
            Flash_FifoBuf[3] &= ~(0xff << 24);
            Flash_FifoBuf[3] |= ((TempInt & 0xff) << 24);                   //��������
            Flash_FifoBuf[3] &= ~(0xf << 20);
            Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 20);
      
            /* ��״̬���ޱ��� */
            TempInt = (int)WaterSpeedMin_Set;
            TempFloat = ((int)(WaterSpeedMin_Set * 10) % 10); 
            
            Flash_FifoBuf[3] &= ~(0xff << 12);
            Flash_FifoBuf[3] |= ((TempInt & 0xff) << 12);                   //����
            Flash_FifoBuf[3] &= ~(0xf << 8);
            Flash_FifoBuf[3] |= ((TempFloat & 0xf) << 8);
      
            /* PH���ޱ��� */
            TempInt = (int)PHMax_Set;
            TempFloat = ((int)(PHMax_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 24);
            Flash_FifoBuf[1] |= ((TempInt & 0xf) << 28);                    //����
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 24);
      
            /* PH���ޱ��� */
            TempInt = (int)PHMin_Set;
            TempFloat = ((int)(PHMin_Set * 10) % 10); 
            
            Flash_FifoBuf[1] &= ~(0xff << 16);
            Flash_FifoBuf[1] |= ((TempInt & 0xf) << 20);                    //����
            Flash_FifoBuf[1] |= ((TempFloat & 0xf) << 16);  
            
            PageSerial = DisDatPage;                                        //��ת��������ʾҳ��
            DatPage = 0;                                                    //��־λ����
            Scr_SendCmd((uint8_t*)"page DisDat");                           //��תҳ��            
            FlashDatPage();                                                 //ˢ��ҳ��            
          }
            
          if (WritePos[0] == -1)                                            //��ʱ������ҳ��
          {
            PageSerial = TimeSetPage;
            Scr_SendCmd((uint8_t*)"page TimeSet");                          //��תҳ��  
            FlashTimeSetPage(KeyDefault);             
          }
          
          if (WritePos[0] == 1)                                             //��һҳ
          {
            PageSerial = DisDatPage;                                        //��ת��������ʾҳ��
            DatPage = 0;                                                    //��־λ����
            Scr_SendCmd((uint8_t*)"page DisDat");                           //��תҳ��  
            FlashDatPage();                                                 //ˢ��ҳ��                      
          }
          
          FistCominTemp = 1;
          /* ��λλ�ñ�־λ���Ա��´δ򿪱�ҳ�� */
          WritePos[0] = -1;          
          WritePos[1] = 4;
          WriteLastPos[0] = -1;
          WriteLastPos[1] = 4;
        }
        else
        {
          LimPage_FlashPos(WritePos[0], WritePos[1], Color_Red);            //��ѡ�еĿؼ����
          IsWrite = 1;                                                      //����дλ��1����ʾ��ǰ���޸�ֵ״̬
        }
        break;
      default:
        break;
    }
    
    if (Key_Statue != KeyOK)                                                //������µĲ���OK��
    {
      if (WriteLastPos[1] == 4)                                             //�������һҳ�������ɫ����ɫ��
        LimPage_FlashPos(WriteLastPos[0], WriteLastPos[1], Color_Popul);      
      else
      {
        LimPage_FlashPos(WriteLastPos[0], WriteLastPos[1], Color_Black);    //�ָ�֮ǰ���ڵİ���ҹɫ
      }   
      
      LimPage_FlashPos(WritePos[0], WritePos[1], Color_Yellow);             //���������ڵĿռ��ɻ�ɫ
      WriteLastPos[0] = WritePos[0];                                        //������һ������λ��
      WriteLastPos[1] = WritePos[1];
    }
  }
  else                                    //�û����ڱ༭
  {
    if ((WritePos[0] == 0) && (WritePos[1] == 0))                           //������������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          EnvirTempMax_Set += 1;                                            //���¼�1
          if (EnvirTempMax_Set > 100)                                       //�޷�100
            EnvirTempMax_Set = 100; 
        
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);    //���͸���Ļ
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          EnvirTempMax_Set -= 1;
          if (EnvirTempMax_Set <= 0)
            EnvirTempMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t3.txt=\"%.1f\"", EnvirTempMax_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t3.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
      
    }   
    
    if ((WritePos[0] == 1) && (WritePos[1] == 0))                            //������������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          EnvirTempMin_Set += 1;
          if (EnvirTempMin_Set > 100)
            EnvirTempMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          EnvirTempMin_Set -= 1;
          if (EnvirTempMin_Set <= 0)
            EnvirTempMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t4.txt=\"%.1f\"", EnvirTempMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t4.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
      
    if ((WritePos[0] == 0) && (WritePos[1] == 1))                             //ˮ����������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterTempMax_Set += 1;
          if (WaterTempMax_Set > 100)
            WaterTempMax_Set = 100;
        
          sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterTempMax_Set -= 1;
          if (WaterTempMax_Set <= 0)
            WaterTempMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t5.txt=\"%.1f\"", WaterTempMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t5.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }  
    
    if ((WritePos[0] == 1) && (WritePos[1] == 1))                               //ˮ����������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterTempMin_Set += 1;
          if (WaterTempMin_Set > 100)
            WaterTempMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterTempMin_Set -= 1;
          if (WaterTempMin_Set <= 0)
            WaterTempMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t6.txt=\"%.1f\"", WaterTempMin_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t6.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }  
      
    if ((WritePos[0] == 0) && (WritePos[1] == 2))                               //��״̬��������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterSpeedMax_Set += 1;
          if (WaterSpeedMax_Set > 100)
            WaterSpeedMax_Set = 100;
        
          sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterSpeedMax_Set -= 1;
          if (WaterSpeedMax_Set <= 0)
            WaterSpeedMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t7.txt=\"%.1f\"", WaterSpeedMax_Set); 
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t7.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
    if ((WritePos[0] == 1) && (WritePos[1] == 2))                               //��״̬��������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          WaterSpeedMin_Set += 1;
          if (WaterSpeedMin_Set > 100)
            WaterSpeedMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          WaterSpeedMin_Set -= 1;
          if (WaterSpeedMin_Set <= 0)
            WaterSpeedMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t8.txt=\"%.1f\"", WaterSpeedMin_Set);  
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t8.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
      
    if ((WritePos[0] == 0) && (WritePos[1] == 3))                                //PH��������
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          PHMax_Set += 1;
          if (PHMax_Set > 16)
            PHMax_Set = 16;
        
          sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          PHMax_Set -= 1;
          if (PHMax_Set <= 0)
            PHMax_Set = 0;
          
          sprintf ((char*)SendBuf, "t9.txt=\"%.1f\"", PHMax_Set);   
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t9.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    } 
    if ((WritePos[0] == 1) && (WritePos[1] == 3))                                 //PH��������   
    {
      switch (Key_Statue)
      {
        case KeyUp:
        {
          PHMin_Set += 1;
          if (PHMin_Set > 100)
            PHMin_Set = 100;
        
          sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);    
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyDn:
        {
          PHMin_Set -= 1;
          if (PHMin_Set <= 0)
            PHMin_Set = 0;
          
          sprintf ((char*)SendBuf, "t13.txt=\"%.1f\"", PHMin_Set);    
          Scr_SendCmd(SendBuf);
        }
          break;
        case KeyOK:
        {
          Scr_SendCmd((uint8_t *)"t13.pco=YELLOW");
          Scr_SendCmd(SendBuf);
          IsWrite = 0;
        }
          break;
        default:
          break;
      }
    }
  }
}

/*
 * ��������DisSensorLost
 * ����  ���ӻ��豸��ʧʱ����ҳ������ʾ�豸��ʧ
 * ����  ����
 * ����  ����
 */
void DisSensorLost (void)
{
  uint8_t SendBuf[15];  
 
  Scr_SendCmd((uint8_t *)"t16.pco=RED");
  Scr_SendCmd((uint8_t *)"t16.txt=\"�豸��ʧ\"");
  
  Scr_SendCmd((uint8_t *)"t17.pco=RED");
  Scr_SendCmd((uint8_t *)"t17.txt=\"�豸��ʧ\"");
  
  Scr_SendCmd((uint8_t *)"t18.pco=RED");
  Scr_SendCmd((uint8_t *)"t18.txt=\"�豸��ʧ\"");
  
  Scr_SendCmd((uint8_t *)"t19.pco=RED");
  Scr_SendCmd((uint8_t *)"t19.txt=\"�豸��ʧ\"");
  
  Scr_SendCmd((uint8_t *)"t20.pco=RED");
  Scr_SendCmd((uint8_t *)"t20.txt=\"�豸��ʧ\"");
  
  Scr_SendCmd((uint8_t *)"t21.pco=RED");
  Scr_SendCmd((uint8_t *)"t21.txt=\"�豸��ʧ\"");
}


/*
 * ��������FlashDatPage
 * ����  ��ˢ��������ʾҳ��
 * ����  ����
 * ����  ����
 */
void FlashDatPage (void)
{
  uint8_t SlaveSearchCount = 0;
  float PH, WaterTemp, EnvirTemp, WaterSpeed;         //�ɼ���������
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;   //������������
  uint8_t WaterLevel, PowerDown;
  uint8_t WarmingTmp = 0;                             //���ڱ�����־λ
  uint8_t SendBuf[30];                                //���͸���Ļ��buf
  
  
  /* �Ȼ�ȡ���õĸ������������� */
  PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
  PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
  WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
  WaterTempMin = ((((Flash_FifoBuf[1] & 0xf) << 4) | ((Flash_FifoBuf[2] >> 28) & 0xf)) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0));
  EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
  EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
  WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
  WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
  
//  WaterMaxTest = WaterTempMax;
//  WaterMinTest = WaterTempMin;
//  WaterTempTest = WaterTemp;
  
  /* ��ʾ�Ϸ���ʱ�� */
//  RTC_Get();
//  DisSensorVal(Time_UpDat, Color_Green, 0);       //������������������Ч����ʹ�ã��������д����
  
  sprintf ((char*)SendBuf, "t0.txt=\"%s\"", ChipID_String);    //��ʾ�豸ID
  Scr_SendCmd(SendBuf);
    
  /* ��ʾ���� */
  if (DatPage == DisMastDat)                      //�����ʾ������������
    EnvirTemp = ((Flash_FifoBuf[6] >> 18) & 0xff) + (((Flash_FifoBuf[6] >> 14) & 0xf) / 10.0);    //��ȡ����
  else
    EnvirTemp = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 24) & 0xff) + (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 20) & 0xf) / 10.0);
    
//  sprintf ((char*)SendBuf, "t16.txt=\"%.1f\"", EnvirTemp);        //��ֵ��ȥ��Ļ��ӡ
  if ((EnvirTemp > EnvirTempMax) || (EnvirTemp < EnvirTempMin))   //�������
  {
    DisSensorVal(SensorEnvirTemp, Color_Red, EnvirTemp);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorEnvirTemp, Color_Green, EnvirTemp);
  
  /* ��ʾˮ�� */
  if (DatPage == DisMastDat)
    WaterTemp = ((Flash_FifoBuf[5] & 0x3f) << 2) + ((Flash_FifoBuf[6] >> 30) & 0x3) + (((Flash_FifoBuf[6] >> 26) & 0xf) / 10.0);
  else
    WaterTemp = (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 4) & 0xff) + ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 0) & 0xf) / 10.0);
  if ((WaterTemp > WaterTempMax) || (WaterTemp < WaterTempMin))
  {
    DisSensorVal(SensorWaterTemp, Color_Red, WaterTemp);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterTemp, Color_Green, WaterTemp);
  
  /* ��ȡ��״̬ */
  if (DatPage == DisMastDat)
    WaterSpeed = ((Flash_FifoBuf[6] >> 6) & 0xff) + (((Flash_FifoBuf[6] >> 2) & 0xf) / 10.0);
  else
    WaterSpeed = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 12) & 0xff) + (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 8) & 0xf) / 10.0);
  if ((WaterSpeed > WaterSpeedMax) || (WaterSpeed < WaterSpeedMin))
  {
    DisSensorVal(SensorWaterSpeed, Color_Red, WaterSpeed);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterSpeed, Color_Green, WaterSpeed);
  
  /* ��ʾˮλ */
  if (DatPage == DisMastDat)
    WaterLevel = ((Flash_FifoBuf[5] >> 24) & 0x1);
  else
    WaterLevel = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 2] >> 7) & 0x01);

  if (WaterLevel == 1)
  {
    DisSensorVal(SensorWaterLevel, Color_Red, WaterLevel);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorWaterLevel, Color_Green, WaterLevel);
  
  /* ��ʾPH */
  if (DatPage == DisMastDat)
    PH = ((Flash_FifoBuf[5] >> 10) & 0xf) + (((Flash_FifoBuf[5] >> 6) & 0xf) / 10.0);
  else
    PH = (((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 16) & 0xf) + ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 12) & 0xf) / 10.0);
  if ((PH > PHMax) || (PH < PHMin))                       //�����������ɫΪ��ɫ
  {
    DisSensorVal(SensorPH, Color_Red, PH);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorPH, Color_Green, PH);
  
  /* ��ʾ������� */
//  if (DatPage == DisMastDat)
    PowerDown = ((Flash_FifoBuf[5] >> 20) & 0x01);
  if (PowerDown == 1)
  {
    DisSensorVal(SensorPower, Color_Red, PowerDown);
    WarmingTmp++;
  }
  else
    DisSensorVal(SensorPower, Color_Green, PowerDown);
  
  /* ��ʾ���·���ҳ���� */
  DisSensorVal(SerialSerial, Color_Green, DatPage);       //��ɫ������û���õ�
    
  
  if (DatPage != DisMastDat)    //�����ʾ���Ǵӻ�����ҳ��
  {
    SlaveSearchCount = ((Flash_FifoBuf[7 + (DatPage - 1) * 3 + 1] >> 26) & 0xf);
    if  (SlaveSearchCount >= SlaveLostTime)  //����豸�Ѿ���ʧ
    {
      DisSensorLost();
    }
  }
  

      
  /* ֻҪ���ھ��������� */
  if (WarmingTmp > 0)
  {
    //Beep_ON_Nop(700);
  }
}

/*
 * ��������GotoDisDatPage
 * ����  ������ʾҳ����ת��������ʾҳ��
 * ����  ����
 * ����  ����
 */
void GotoDisDatPage (void)
{  
  /* �л�ҳ�� */
  Scr_SendCmd((uint8_t*)"page DisDat");  
  PageSerial = DisDatPage;
  UserDelay_KeyScan(500);
  
  FlashPage();               //ˢ��ҳ��
}


void FlashLimPage (void)
{
  uint8_t SendBuf[50];
  int PosTable_x[] = {121, 227};
  int PosTable_y[] = {78, 110, 143, 174};
  float PHMax, PHMin, WaterTempMax, WaterTempMin, EnvirTempMax, EnvirTempMin, WaterSpeedMax, WaterSpeedMin;
    
  PHMax = ((Flash_FifoBuf[1] >> 28) & 0xf) + (((Flash_FifoBuf[1] >> 24) & 0xf) / 10.0);
  PHMin = ((Flash_FifoBuf[1] >> 20) & 0xf) + (((Flash_FifoBuf[1] >> 16) & 0xf) / 10.0);
  WaterTempMax = ((Flash_FifoBuf[1] >> 8) & 0xff) + (((Flash_FifoBuf[1] >> 4) & 0xf) / 10.0);
  WaterTempMin = (Flash_FifoBuf[1] & 0xf << 4) + ((Flash_FifoBuf[2] >> 28) & 0xf) + (((Flash_FifoBuf[2] >> 24) & 0xf) / 10.0);
  EnvirTempMax = ((Flash_FifoBuf[2] >> 16) & 0xff) + (((Flash_FifoBuf[2] >> 12) & 0xf) / 10.0);
  EnvirTempMin = ((Flash_FifoBuf[2] >> 4) & 0xff) + (((Flash_FifoBuf[2] >> 0) & 0xf) / 10.0);
  WaterSpeedMax = ((Flash_FifoBuf[3] >> 24) & 0xff) + (((Flash_FifoBuf[3] >> 20) & 0xf) / 10.0);
  WaterSpeedMin = ((Flash_FifoBuf[3] >> 12) & 0xff) + (((Flash_FifoBuf[3] >> 8) & 0xf) / 10.0);
  
  
  /* ���� */
  USART1_SendBuf("fill 90,75,200,125,36797");
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* PH���ֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[0], PHMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* PH��Сֵ*/
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[0], PHMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* ˮ�����ֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[1], WaterTempMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* ˮ����Сֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[1], WaterTempMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* �������ֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[2], EnvirTempMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* ������Сֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[2], EnvirTempMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* �������ֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[0], PosTable_y[3], WaterSpeedMax);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  
  /* ������Сֵ */
  sprintf ((char*)SendBuf, "xstr %d,%d,49,22,0,BLACK,BLACK,1,1,3,\"%.1f\"", PosTable_x[1], PosTable_y[3], WaterSpeedMin);
  USART1_SendBuf(SendBuf);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
}

/*
 * ��������GoLimPage
 * ����  ��������������ֵ��ˢ��ҳ����ʾ���룬֮��ص���ʾҳ��
 * ����  ����
 * ����  ����
 */
void GoLimPage (void)
{
  /* �л�ҳ�� */
  USART1_SendBuf("page LimUpDat");    //��ת����ֵ��ʾҳ��
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  UserDelay_KeyScan(50);
  
  FlashLimPage();                     //��ʾ�µ���ֵ
  
  UserDelay_KeyScan(3000);
  
  switch(PageSerial)
  {
    case DisDatPage:
      Scr_SendCmd("page DisDat");  //����ԭ�ȵ�ҳ��
     break;
    case LimSetPage:
      Scr_SendCmd("page LimSet");
      break;
    default:
      break;
  }
  //FlashPage();
  
}

/*
 * ��������ScrRest
 * ����  ����Ļ��λ
 * ����  ����
 * ����  ����
 */
void ScrRest (void)
{
  Delay_ms(500);
  USART1_SendBuf("rest");
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  USART1_SendByte(0xff);
  Delay_ms(200);
}

/*
 * ��������ProBarGo
 * ����  ����ʼ��������������ȿ���
 * ����  ����
 * ����  ����
 */
void ProBarGo (int val)
{
  static uint8_t i = 0;
  int k = 0;
  uint8_t SendBuf[10];
  
  if (val > i)
  {
    for (k= i; k < val; k++)
    {
      sprintf((char*)SendBuf, "j0.val=%d", k);
      USART1_SendBuf(SendBuf);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      Delay_ms(50);
    }
    i = val;
  }
  if (val < i)
  {
    for (k= i; k > val; k--)
    {
      sprintf((char*)SendBuf, "j0.val=%d", k);
      USART1_SendBuf(SendBuf);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      USART1_SendByte(0xff);
      Delay_ms(10);
    }
    i = val;
  }
}


