#include "common.h"
#include "Bsp_Key.h"

uint8_t KeyStatue = KeyDefault;

/*
 * ��������Key_Init
 * ����  �����̰�����ʼ��
 * ����  ����
 * ����  ����
 */
void Key_Init (void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_KeyUp | RCC_APB2Periph_KeyDn | RCC_APB2Periph_KeyOK |
                         RCC_APB2Periph_KeyLF | RCC_APB2Periph_KeyRG ,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyUp; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOx_KeyUp, &GPIO_InitStructure);  
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyDn; 
  GPIO_Init(GPIOx_KeyDn, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyOK; 
  GPIO_Init(GPIOx_KeyOK, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyLF; 
  GPIO_Init(GPIOx_KeyLF, &GPIO_InitStructure); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_KeyRG; 
  GPIO_Init(GPIOx_KeyRG, &GPIO_InitStructure); 
}

/*
 * ��������ReadTouchWay
 * ����  ��ȷ��ͨ�ŷ�ʽ
 * ����  ����
 * ����  ����
 */
void ReadTouchWay (void)    
{
  uint8_t statue = 0;
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  statue = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1);
  if (statue)         
    Connect_Way = USE_ZGB;      //Ĭ����ZGB
  else
    Connect_Way = USE_CAN;      //������CAN
}



/*
 * ��������Key_Scan
 * ����  ������ɨ��
 * ����  ����
 * ����  ����
 */
void Key_Scan (void)
{
  static uint8_t statue = 0;
  
  int PressCount = 0;
  if (statue == 0)
  {
    statue = 1;
    
    if (KeyStatue == KeyDefault)
    {
      /* �� */
      if (KEY_LF == 0)
      {
        Delay_ms(10);
        if (KEY_LF == 0)
          KeyStatue = KeyLF;
        while(KEY_LF == 0);
        Delay_ms(10);
      }
      
      /* �� */
      if (KEY_UP == 0)
      {
        Delay_ms(10);
        if (KEY_UP == 0)
          KeyStatue = KeyUp;
        while(KEY_UP == 0);
        Delay_ms(10);
      }
      
      /* �� */
      if (KEY_RG == 0)
      {
        Delay_ms(10);
        if (KEY_RG == 0)
          KeyStatue = KeyRG;
        while(KEY_RG == 0);
        Delay_ms(10);
      }
      
      /* ���ܼ� */
      if (KEY_OK == 0)
      {
        Delay_ms(8);
        if (KEY_OK == 0)
          KeyStatue = KeyOK;
        while(KEY_OK == 0)
        {
          Delay_ms(10);
          PressCount++;
          if (PressCount >= 500)    //�����Ժ����豸����Ϣ
          {
            if (TCPIsUsed == 1)
            {
              Beep_ON_Nop(100);
              Beep_ON_Nop(100);
            }
            else
            {
              DevBound();
            }
          }
        }
        Delay_ms(10);
      }
      
      /* �� */
      if (KEY_DN == 0)
      {
        Delay_ms(10);
        if (KEY_DN == 0)
          KeyStatue = KeyDn;
        while(KEY_DN == 0);
        Delay_ms(10);
      }
      
      if (KeyStatue != KeyDefault)
      {
        LED_Green(ON);
        FlashPage();
      }
    }
    statue = 0;
  }
}





void Key_Scan1 (void)
{
  int PressCount = 0;
  
  if (KeyStatue == KeyDefault)
  {
    /* �� */
    if (KEY_LF == 0)
    {
      Delay_ms(10);
      if (KEY_LF == 0)
        KeyStatue = KeyLF;
      while(KEY_LF == 0);
      Delay_ms(10);
    }
    
    /* �� */
    if (KEY_UP == 0)
    {
      Delay_ms(10);
      if (KEY_UP == 0)
        KeyStatue = KeyUp;
      while(KEY_UP == 0);
      Delay_ms(10);
    }
    
    /* �� */
    if (KEY_RG == 0)
    {
      Delay_ms(10);
      if (KEY_RG == 0)
        KeyStatue = KeyRG;
      while(KEY_RG == 0);
      Delay_ms(10);
    }
    
    /* ���ܼ� */
    if (KEY_OK == 0)
    {
      Delay_ms(8);
      if (KEY_OK == 0)
        KeyStatue = KeyOK;
      while(KEY_OK == 0)
      {
        Delay_ms(10);
        PressCount++;
        if (PressCount >= 500)    //�����Ժ����豸����Ϣ
        {
          DevBound();
        }
      }
      Delay_ms(10);
    }
    
    /* �� */
    if (KEY_DN == 0)
    {
      Delay_ms(10);
      if (KEY_DN == 0)
        KeyStatue = KeyDn;
      while(KEY_DN == 0);
      Delay_ms(10);
    }
    
    if (KeyStatue != KeyDefault)
    {
      FlashPage();
    }
    
  }
  
}


VOID task_KeyScan (void)
{
  UINT32 uwRet = LOS_OK;
  
  LED_Green(ON);
  
  while(1)
  {
    Key_Scan();
    
    uwRet = LOS_TaskDelay(100);//ϵͳ��ʱ1S
		if(uwRet !=LOS_OK)
      return;
  }
}

UINT32 KeyScanTskHandle;

UINT32 creat_KeyScanTask (void)
{
	UINT32 uwRet = LOS_OK;
	TSK_INIT_PARAM_S task_init_param;
	task_init_param.usTaskPrio = 0;                   //�������ȼ���0���
	task_init_param.pcName = "KeyScan";                //???
	task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)task_KeyScan;//������ں���
	task_init_param.uwStackSize = LOSCFG_BASE_CORE_TSK_IDLE_STACK_SIZE;//�����ջ��С
	task_init_param.uwResved = LOS_TASK_STATUS_DETACHED;
	uwRet = LOS_TaskCreate(&KeyScanTskHandle,&task_init_param);//��������
  
	if(uwRet !=LOS_OK)
	{
		return uwRet;
	}
	return uwRet;
}





