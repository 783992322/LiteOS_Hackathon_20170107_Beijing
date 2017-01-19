#include "common.h"

UINT32 g_UpDatTskHandle;

void task_DatUp2Server (void)
{
  UINT32 uwRet = LOS_OK;
  
  uint8_t UpLoadTimeCount = 10;   //10
  uint8_t UpWarmingTimeCount = 0;
  uint8_t UpWarmingAgainCount = 0;
  uint8_t ClearWarmingUpCount = 0;
  
  uint8_t ErrorCount = 0;
  uint8_t statue = 0;
  uint8_t i = 0;
  uint8_t SlaveSerial = 0;
  
  while(1)
  {
    ErrorCount = 0;
    statue = 0;
    i = 0;
    SlaveSerial = 0;
    
    UpLoadTimeCount++;
    UpWarmingTimeCount++;
    UpWarmingAgainCount++;
    ClearWarmingUpCount++;
    
    
    if (UpWarmingAgainCount >= UpWarmingAgainCountTime)
    {
      if(TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
        LED_Green(ON);                //����ɫָʾ��
        UpWarmingAgainCount = 0;       //���ü�����־λ
                
        ErrorCount = 0;               //���ô������ֵ
        do
        {
          statue = UpWarmingDatAgain_TCP();    //�ϴ�����
          ErrorCount++;             
          if (ErrorCount > 2) 
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();                  //����ϱ���������GPRS
            statue = DefaultValue;
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        
        LED_Green(OFF);               //�ر���ɫָʾ��
        TIM_Cmd(TIM2, ENABLE);
        TCPIsUsed = 0;  
      }
    }
    
    /* ���һ�����ޱ����Լ��������豸��Ҫ�ϱ� */
    if (UpWarmingTimeCount >= UpWarmingTime)      //���һ�����޾���
    {
      if(TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        TIM_Cmd(TIM2, DISABLE);	      //�ر�������ȶ�ʱ��
        LED_Green(ON);                //����ɫָʾ��
        UpWarmingTimeCount = 0;       //���ü�����־λ
        
        if (SlaveNeedAddBuf[0] > 0)       //�ж��Ƿ������豸��Ҫע��
        {
          for (i = 1; i <= SlaveNeedAddBuf[0]; i++)
          {
            SlaveSerial = Search_SlaveID(SlaveNeedAddBuf[i]);
          
            ErrorCount = 0;
            do
            {
              statue = AddSlaveID2Server(SlaveNeedAddBuf[i], SlaveSerial);
              ErrorCount++;
              if (ErrorCount >= 2)
              {
                ErrorCount = 0;
                TCPIsUsed = 0;
                GPRS_Rest();
                statue = DefaultValue;
                TCPIsUsed = 1;
              }
             }while(statue != SUCCESS);
          }
          SlaveNeedAddBuf[0] = 0;
        }
        
        ErrorCount = 0;               //���ô������ֵ
        do
        {
          statue = UpWarmingDat_TCP();    //�ϴ�����
          ErrorCount++;             
          if (ErrorCount > 2) 
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();                  //����ϱ���������GPRS
            statue = DefaultValue;
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        
        LED_Green(OFF);               //�ر���ɫָʾ��
        TIM_Cmd(                TIM2, ENABLE);
        TCPIsUsed = 0;  
      }
    }
    
    ErrorCount = 0;
    if (ClearWarmingUpCount >= ClearWarmingUpTime)
    {
      if (TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        LED_Green(ON);
        ClearWarmingUpCount = 0;
        TIM_Cmd(TIM2, DISABLE);	
        
        if (WarmClearStatue > 0)
        {
          WarmClearStatue = 0;
          do
          {
            statue = UpLoadDat_TCP();
            ErrorCount++;
            if (ErrorCount > 2)
            {
              ErrorCount = 0;
              TCPIsUsed = 0;
              GPRS_Rest();
              TCPIsUsed = 1;
            }
          }while(statue != SUCCESS);
        }
        
        TIM_Cmd(TIM2, ENABLE);	
        LED_Green(OFF);
//        Flash_WriteBufToPage();
        TCPIsUsed = 0;
      }
    }
    
    ErrorCount = 0;
    if (UpLoadTimeCount >= UpLoadingTime)         //��������ϴ�һ������
    {
      if (TCPIsUsed == 0)
      {
        TCPIsUsed = 1;
        
        LED_Green(ON);
        UpLoadTimeCount = 0;
        TIM_Cmd(TIM2, DISABLE);	
        do
        {
          statue = UpLoadDat_TCP();
          ErrorCount++;
          if (ErrorCount > 2)
          {
            ErrorCount = 0;
            TCPIsUsed = 0;
            GPRS_Rest();
            TCPIsUsed = 1;
          }
        }while(statue != SUCCESS);
        TIM_Cmd(TIM2, ENABLE);	
        LED_Green(OFF);
//        Flash_WriteBufToPage();
        TCPIsUsed = 0;
      }
    }
    
    uwRet = LOS_TaskDelay(1000 * AlarmTime);//ϵͳ��ʱ1S
		if(uwRet !=LOS_OK)
      return;
  }
}


UINT32 creat_UpDat2ServerTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 4;                   //�������ȼ���0���
	task_init_param.pcName = "DatUp";                 //name
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_DatUp2Server;//������ں���
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;//�����ջ��С
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&g_UpDatTskHandle,&task_init_param);//��������
  
  
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}