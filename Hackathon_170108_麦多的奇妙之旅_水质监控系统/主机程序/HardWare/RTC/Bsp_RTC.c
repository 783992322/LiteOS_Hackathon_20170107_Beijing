#include "common.h"

long int NowTime = 0;

_calendar_obj calendar;   //ʱ�ӽṹ�� 

int User_year = 2016, User_month = 12, User_day = 12, User_hour = 12, User_min = 12, User_sec = 12;

//int User_year, User_month, User_day, User_hour, User_min, User_sec;

void RTC_irqhandle (void)
{
  if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)         //�����ж�
	{
    RTC_ClearITPendingBit(RTC_IT_ALR);		        //�������ж�	 
    RTC_WaitForLastTask();

    USART1_SendBuf ("RTC_Task  ***********************haha*************\r\n");
    
    
    NowTime = RTC_GetCounter();
    RTC_SetAlarm(NowTime + AlarmTime);            //������������
    
    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //�������ж�
    RTC_WaitForLastTask();	 
    
  }
  

}

void RTC_nvic(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

VOID RTC_Interrupt(VOID)
{
  UINTPTR uvIntSave;
  uvIntSave = LOS_IntLock();
  RTC_nvic();
  
  LOS_HwiCreate(41, 4,0,RTC_irqhandle,0);//�����ж�
  LOS_IntRestore(uvIntSave);
}

/*
 * ��������RTC_Init
 * ����  ��ʵʱʱ������
 * ����  ����
 * ����  ��0�� �ɹ�
 */
u8 RTC_Init(void)
{
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	                      //ʹ�ܺ󱸼Ĵ�������  
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
//	{	 			
//		BKP_DeInit();	                                    //��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ON);	                      //�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	    //���ָ����RCC��־λ�������,�ȴ����پ������
			{
        temp++;
        UserDelay_KeyScan(10);
			}
      
    if(temp>=250)return 1;                            //��ʼ��ʱ��ʧ��,����������	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		        //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	                          //ʹ��RTCʱ��  
		RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		                          //�ȴ�RTC�Ĵ���ͬ��  
		//RTC_ITConfig(RTC_IT_SEC, ENABLE);		            //ʹ��RTC���ж�
    RTC_ITConfig(RTC_IT_ALR,ENABLE) ;                 //ʹ�������ж�
		RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();                            // ��������	
		RTC_SetPrescaler(32767);                          //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
 
    RTC_ExitConfigMode();                             //�˳�����ģʽ  
    NowTime=RTC_GetCounter();
    RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_SetAlarm(NowTime+AlarmTime);                  //��������
//    if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)
//    {
      RTC_Set(User_year, User_month,User_day,User_hour,User_min, User_sec);              //����ʱ��	
//    }
		RTC_ExitConfigMode();                             //�˳�����ģʽ  
    NowTime=RTC_GetCounter();
    RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_SetAlarm(NowTime+AlarmTime);                  //��������
//		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	        //��ָ���ĺ󱸼Ĵ�����д���û���������
//	}
//	else//ϵͳ������ʱ
//	{
//		RTC_WaitForSynchro();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//    RTC_ITConfig(RTC_IT_ALR,ENABLE) ;               //ʹ�������ж�
//		RTC_WaitForLastTask();	                        //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//    NowTime=RTC_GetCounter();
//    RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//    RTC_SetAlarm(NowTime+AlarmTime);                  //��������
//	}
//	RTC_Get();      

  RTC_Interrupt();  
  
	return 0;

}		 				    

/*
 * ��������Is_Leap_Year
 * ����  ���ж��Ƿ������꺯��
 *          �·�   1  2  3  4  5  6  7  8  9  10 11 12
 *          ����   31 29 31 30 31 30 31 31 30 31 30 31
 *          ������ 31 28 31 30 31 30 31 31 30 31 30 31
 * ����  ��year�����
 * ����  �����:������ǲ�������.1,��.0,����
 */
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   


//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*
 * ��������RTC_Set
 * ����  ������ʱ��
 *          �������ʱ��ת��Ϊ���ӣ���1970��1��1��Ϊ��׼
 * ����  ����
 * ����  ��0,�ɹ�;����:�������.
 */
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	                          //��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;          //�����������
		else seccount+=31536000;			                  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	                              //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;              //�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;   //����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;                    //��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;                         //Сʱ������
    seccount+=(u32)min*60;	                        //����������
	seccount+=sec;                                    //�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	                    //ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);                         //����RTC��������ֵ

	RTC_WaitForLastTask();	                          //�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;	    
}


/*
 * ��������RTC_Alarm_Set
 * ����  ����ʼ������		
 * ����  �����
 * ����  ��0,�ɹ�;����:�������.
 */
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	                            //��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;            //�����������
		else seccount+=31536000;			                    //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	                                //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;                //�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;     //����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;                      //��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;                           //Сʱ������
    seccount+=(u32)min*60;	                          //����������
	seccount+=sec;                                      //�������Ӽ���ȥ 			    
  /* ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	                                            //ʹ�ܺ󱸼Ĵ�������  	
	RTC_SetAlarm(seccount); 
	RTC_WaitForLastTask();	                            //�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	
	return 0;	    
}

/*
 * ��������RTC_Get
 * ����  ���õ���ǰ��ʱ��
 * ����  ����
 * ����  ��0,�ɹ�;����:�������.
 */
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=RTC_GetCounter();	 
 	temp=timecount/86400;                 //�õ�����(��������Ӧ��)
	if(daycnt!=temp)                      //����һ����
	{	  
		daycnt=temp;
		temp1=1970;	                        //��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))           //������
			{
				if(temp>=366)temp-=366;         //�����������
				else {temp1++;break;}  
			}
			else temp-=365;	                  //ƽ�� 
			temp1++;  
		}   
		calendar.w_year=temp1;              //�õ����
		temp1=0;
		while(temp>=28)                     //������һ����
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)   //�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;                       //�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];   //ƽ��
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	        //�õ��·�
		calendar.w_date=temp+1;  	        //�õ����� 
    
    if ((calendar.w_month == 13) && (calendar.w_date == 1))
    {
      calendar.w_month = 12;
      calendar.w_date = 31;
    }
	}
	temp=timecount%86400;     		      //�õ�������   	   
	calendar.hour=temp/3600;     	      //Сʱ
	calendar.min=(temp%3600)/60; 	      //����	
	calendar.sec=(temp%3600)%60; 	      //����
	return 0;
}	 

/*
 * ��������RTC_IRQHandler
 * ����  ��RTCʱ���ж�
 * ����  ����
 * ����  ����
 */
//void RTC_IRQHandler(void)
//{		 
////  static int UpLoadTimeCount = 10;   //10
////  static int UpWarmingTimeCount = 0;
////  static int UpWarmingAgainCount = 0;
////  static int ClearWarmingUpCount = 0;
////  
////  uint8_t ErrorCount = 0;
////  uint8_t statue = 0;
////  uint8_t i = 0;
////  uint8_t SlaveSerial = 0;
//  
//	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)         //�����ж�
//	{
////    UpLoadTimeCount++;
////    UpWarmingTimeCount++;
////    UpWarmingAgainCount++;
////    ClearWarmingUpCount++;
////    
////		RTC_ClearITPendingBit(RTC_IT_ALR);		        //�������ж�	 
////    RTC_WaitForLastTask();

////    /* �����ظ��ϴ� */
////    if (UpWarmingAgainCount >= UpWarmingAgainCountTime)
////    {
////      if(TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
////        LED_Green(ON);                //����ɫָʾ��
////        UpWarmingAgainCount = 0;       //���ü�����־λ
////                
////        ErrorCount = 0;               //���ô������ֵ
////        do
////        {
////          statue = UpWarmingDatAgain_TCP();    //�ϴ�����
////          ErrorCount++;             
////          if (ErrorCount > 2) 
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();                  //����ϱ���������GPRS
////            statue = DefaultValue;
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        
////        LED_Green(OFF);               //�ر���ɫָʾ��
////        TIM_Cmd(TIM2, ENABLE);
////        TCPIsUsed = 0;  
////      }
////    }
////    
////    /* ���һ�����ޱ����Լ��������豸��Ҫ�ϱ� */
////    if (UpWarmingTimeCount >= UpWarmingTime)      //���һ�����޾���
////    {
////      if(TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
////        LED_Green(ON);                //����ɫָʾ��
////        UpWarmingTimeCount = 0;       //���ü�����־λ
////        
////        if (SlaveNeedAddBuf[0] > 0)       //�ж��Ƿ������豸��Ҫע��
////        {
////          for (i = 1; i <= SlaveNeedAddBuf[0]; i++)
////          {
////            SlaveSerial = Search_SlaveID(SlaveNeedAddBuf[i]);
////          
////            ErrorCount = 0;
////            do
////            {
////              statue = AddSlaveID2Server(SlaveNeedAddBuf[i], SlaveSerial);
////              ErrorCount++;
////              if (ErrorCount >= 2)
////              {
////                ErrorCount = 0;
////                TCPIsUsed = 0;
////                GPRS_Rest();
////                statue = DefaultValue;
////                TCPIsUsed = 1;
////              }
////             }while(statue != SUCCESS);
////          }
////          SlaveNeedAddBuf[0] = 0;
////        }
////        
////        ErrorCount = 0;               //���ô������ֵ
////        do
////        {
////          statue = UpWarmingDat_TCP();    //�ϴ�����
////          ErrorCount++;             
////          if (ErrorCount > 2) 
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();                  //����ϱ���������GPRS
////            statue = DefaultValue;
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        
////        LED_Green(OFF);               //�ر���ɫָʾ��
////        TIM_Cmd(                TIM2, ENABLE);
////        TCPIsUsed = 0;  
////      }
////    }
////    
////    ErrorCount = 0;
////    if (ClearWarmingUpCount >= ClearWarmingUpTime)
////    {
////      if (TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        LED_Green(ON);
////        ClearWarmingUpCount = 0;
////        TIM_Cmd(TIM2, DISABLE);	
////        
////        if (WarmClearStatue > 0)
////        {
////          WarmClearStatue = 0;
////          do
////          {
////            statue = UpLoadDat_TCP();
////            ErrorCount++;
////            if (ErrorCount > 2)
////            {
////              ErrorCount = 0;
////              TCPIsUsed = 0;
////              GPRS_Rest();
////              TCPIsUsed = 1;
////            }
////          }while(statue != SUCCESS);
////        }
////        
////        TIM_Cmd(TIM2, ENABLE);	
////        LED_Green(OFF);
//////        Flash_WriteBufToPage();
////        TCPIsUsed = 0;
////      }
////    }
////    
////    ErrorCount = 0;
////    if (UpLoadTimeCount >= UpLoadingTime)         //��������ϴ�һ������
////    {
////      if (TCPIsUsed == 0)
////      {
////        TCPIsUsed = 1;
////        
////        LED_Green(ON);
////        UpLoadTimeCount = 0;
////        TIM_Cmd(TIM2, DISABLE);	
////        do
////        {
////          statue = UpLoadDat_TCP();
////          ErrorCount++;
////          if (ErrorCount > 2)
////          {
////            ErrorCount = 0;
////            TCPIsUsed = 0;
////            GPRS_Rest();
////            TCPIsUsed = 1;
////          }
////        }while(statue != SUCCESS);
////        TIM_Cmd(TIM2, ENABLE);	
////        LED_Green(OFF);
//////        Flash_WriteBufToPage();
////        TCPIsUsed = 0;
////      }
////    }
////    
////    RTC_Get();                                    //����ʱ��      
////	  NowTime = RTC_GetCounter();
////    RTC_SetAlarm(NowTime + AlarmTime);            //������������
////    
////    RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);  //�������ж�
////    RTC_WaitForLastTask();	 
//  } 				  								   						 	   	 
//}
