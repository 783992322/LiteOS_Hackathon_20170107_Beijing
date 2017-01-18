#include "common.h"

uint8_t GetNet_Statue = Net_NoNet;    //�ж��Ƿ�������־λ
uint8_t GetMasterNet_Statue = CheckLostTime;

void Delay(__IO u32 nCount);
#define SOFT_DELAY Delay(0x0FFFFF);


UINT32 g_uwTskHiID;
UINT32 g_uwTskLoID;

void Delay(__IO uint32_t nCount) //����ʱ
{
	for(; nCount != 0; nCount--);
}

void hardware_init(void)
{ 
  u16 Rand = 0;
  
  LED_GPIO_Config();          //LED���ų�ʼ��
  ReadTouchWay();             //��ȡͨѶ��ʽ
  Delay_ms(2000);             //�ϵ���ʱһ��
  USART_Config();             //DebugUart��ʼ��
  GetChipID();                //��ȡоƬID
  WaterSpeedIn_Init();        //ˮ�����ٳ�ʼ��
  Water_Level_Init();         //ˮλ����ʼ��
  DS18B20_Init(DS18B20IN);    //���³�ʼ��
  DS18B20_Init(DS18B20EX);    //ˮ�³�ʼ��
  AD_Init();                  //PHAD��ʼ��
  Adc_Init();		  		        //ADC��ʼ��
  FlashInit();                //���ڴ洢��Ҫ���籣�����Ϣ
  
  if (Connect_Way == USE_CAN) //CAN��ͨģʽ��ʼ��, ������500Kbps 
    CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,0); //CAN��ͨģʽ��ʼ��, ������500Kbps 
  else
    USART2_Config();            //ZGB��ʼ��
  
  while(GetNet_Statue == Net_NoNet)
  {
    Rand = GetRand();
    if (Connect_Way == USE_ZGB)
      Delay_ms(Rand);           //��ʱ��ΪZGB���ǵģ�ZGBͨѶ���ʽ���
    
    if (Connect_Way == USE_CAN)
      Can_ReqGetNet();
    else
      Zgb_ReqGetNet();
    
    if (Connect_Way == USE_ZGB)
      Delay_ms(5000);           //��ʱ��ΪZGB���ǵģ�ZGBͨѶ���ʽ���
    
    switch(GetNet_Statue)
    {
      case Net_Refused:       //�����ʾ�����ܾ�����������
          while(1);           //��ѭ��
      case Net_HasBeenAsked : //�����ʾ�Ѿ���������ȴ�������ȷ��
      default:                //���һֱδ�յ���������Ϣ����������������
        break;
    }
  }
}

UINT32 osAppInit(void)
{
	UINT32 uwRet = 0;
	hardware_init();

	LED_Green(ON);
	LED_Blue(OFF);
  
  uwRet = creat_GetSelfDatTask();     //�ɼ�������������
	if(uwRet !=LOS_OK)
		return uwRet;

  uwRet = creat_GetNetAgainTask();    //����Ƿ���ߣ���������
  if(uwRet !=LOS_OK)
		return uwRet;
  
  uwRet = creat_SendAgainTask();    //����Ƿ���ߣ���������
  if(uwRet !=LOS_OK)
		return uwRet;
  
	return LOS_OK;
}
/*********************************************END OF FILE**********************/
