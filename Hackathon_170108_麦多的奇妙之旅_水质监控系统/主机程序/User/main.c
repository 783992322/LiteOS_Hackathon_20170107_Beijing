#include "common.h"

uint8_t Connect_Way = USE_CAN;                      //ͨѶ��ʽ��־λ USE_CAN   USE_ZGB


void TestValSet (void)
{
  /*
   *  ˮ�£�      ���ޣ�30.0    ���ޣ�15.0
   *  ���£�      ���ޣ�30.0    ���ޣ�15.0
   *  ˮ������    ���ޣ�20L/s   ���ޣ�0L/s
   *  PH��        ���ޣ�8.0     ���ޣ�6.0
   */
  
  /* *******ˮ����ֵ����������******** */
  Flash_FifoBuf[1] &= ~(0xff << 8);
  Flash_FifoBuf[1] |= ((30 & 0xff) << 8);   //��������        
  Flash_FifoBuf[1] &= ~(0xf << 4);
  Flash_FifoBuf[1] |= ((0 & 0xf) << 4);
  
  Flash_FifoBuf[1] &= ~(0xf);
  Flash_FifoBuf[1] |= ((15 >> 4)& 0xf);   //��������
  Flash_FifoBuf[2] &= ~(0xff << 24);
  Flash_FifoBuf[2] |= ((15 & 0xf) << 28);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 24);
  /* ********************************* */
  
  /* *******������ֵ����������******** */
  Flash_FifoBuf[2] &= ~(0xff << 16);
  Flash_FifoBuf[2] |= ((30 & 0xff) << 16);         //��������
  Flash_FifoBuf[2] &= ~(0xf << 12);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 12);
  
  Flash_FifoBuf[2] &= ~(0xff << 4);
  Flash_FifoBuf[2] |= ((15 & 0xff) << 4);         //��������
  Flash_FifoBuf[2] &= ~(0xf << 0);
  Flash_FifoBuf[2] |= ((0 & 0xf) << 0);
  /* ********************************* */
  
  /* *******ˮ����ֵ����������******** */
  Flash_FifoBuf[3] &= ~(0xff << 24);
  Flash_FifoBuf[3] |= ((20 & 0xff) << 24);   //��������
  Flash_FifoBuf[3] &= ~(0xf << 20);
  Flash_FifoBuf[3] |= ((0 & 0xf) << 20);
  
  Flash_FifoBuf[3] &= ~(0xff << 12);
  Flash_FifoBuf[3] |= ((0 & 0xff) << 12);   //����
  Flash_FifoBuf[3] &= ~(0xf << 8);
  Flash_FifoBuf[3] |= ((0 & 0xf) << 8);
  /* **************************** */
  
  /* **********PH����������*********** */ 
  Flash_FifoBuf[1] &= ~(0xffff << 16);
  Flash_FifoBuf[1] |= ((8 & 0xf) << 28);    //����
  Flash_FifoBuf[1] |= ((0 & 0xf) << 24);
  Flash_FifoBuf[1] |= ((6 & 0xf) << 20);    //����
  Flash_FifoBuf[1] |= ((0 & 0xf) << 16);  
  /* ********************************* */
//  Flash_ReadPageToBUF();
  
  /* ******���ӻ�����������****** */
//  Flash_FifoBuf[0] = 1;
////////  
//  Flash_FifoBuf[7] = 0x67065514;
//  Flash_FifoBuf[8] = 0x01000114;    //ģ�������
//  Flash_FifoBuf[9] = 0x11600020;
//  
//  Flash_FifoBuf[10] = 0;        
//  Flash_FifoBuf[11] = 0;   //ģ��δ��ԣ��������ѻ�Ӧ
//  Flash_FifoBuf[12] = 0;
  /* ************** */
  
  /* �������ô���� */
  //Flash_FifoBuf[0] = 0;  
//  Flash_WriteBufToPage();
//  Flash_WriteOfficDat();
  /* ************** */

//  Flash_ReadPageToBUF();
//  Flash_ReadOfficDat();
//  Flash_WriteBufToPage();
  //printf ("wait for slave\r\n");
}


void hardware_init(void)
{
  Delay_ms(2000);               //��ʾ���ϵ���Ҫ�ȴ�һ�� 
  GPRS_GPIO_Config();           //A6ģ�鿪������λ����λ  
  Key_Init();
  ReadTouchWay();               //ȷ��ͨ�ŷ�ʽ��һ��ʼ��ȷ�� 
  GPRS_GPIO_Config();           //A6ģ�鿪������λ����λ  
  Beep_Init();                  //��������ʼ��
  LED_GPIO_Config();            //LED��ʼ����ָʾ��
  USART_Config();               //����1��ʼ����������Ļ
  USART3_Config();
  Scr_SendCmd("page Home");     //�л�����ҳ
  ProBarGo(15);                 //��ʼ�����������15%
  GetChipID();                  //��ȡоƬID
  LED_Green(ON);                //LED������ʾTCP����ռ��
  GPRS_Rest();                  //A6_GPRS������ʼ�����������ӷ����������ʧ�ܾͲ�������
  LED_Green(OFF);               //TCPʹ�����
  DS18B20_Init(DS18B20IN);      //���ص��¶ȴ�������ʼ��
  DS18B20_Init(DS18B20EX);      //ˮ�µ��¶ȴ�������ʼ��
  PowDon_Init();                //�������ʼ��
  WaterSpeedIn_Init();          //ˮ�����ٳ�ʼ��
  Water_Level_Init();
  AD_Init();                    //PH_AD��ʼ��
  TestValSet();                 //����ʱ��Flash��д��һЩֵ����Ʒ������д����ֵ
  if (Connect_Way == USE_ZGB)   //CAN��ͨģʽ��ʼ��, ������500Kbps 
  {
    USART2_Config();
    Beep_ON_Nop(100);           //������һ����ʾ
  }
  else
  {
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0); 
    Beep_ON_Nop(100);           //������������ʾ
    Beep_ON_Nop(100);
  }
//  Beep_ON_Nop(100);             //��������������ʾ�ײ��豸��ʼ�����
//  Beep_ON_Nop(100);
  GotoDisDatPage();             //��ת����ʾ����
  RTC_Init();
  Beep_ON_Nop(300);             //��ʾ��ʼ��������
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
  
	hardware_init();
  
  
  uwRet = creat_GetSelfDatTask();     //�ɼ�������������
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_KeyScanTask();        //����ɨ��
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_GetSlaveDatTask();    //�ɼ��ӻ�����������
	if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_UpDat2ServerTask();   //�ϴ����ݵ�������  �����ɼ����ݡ��������ظ�����
	if(uwRet !=LOS_OK)
		return uwRet;
  
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
