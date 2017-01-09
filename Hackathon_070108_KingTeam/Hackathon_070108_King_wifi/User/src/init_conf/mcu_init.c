#include "mcu_init.h"
#include "core_cm3.h"

void RCC_Configuration(void)
{
          /* ����ö�����ͱ��� HSEStartUpStatus */
          ErrorStatus HSEStartUpStatus;
          
         /* ��λϵͳʱ������*/
          RCC_DeInit();
          RCC_HSEConfig(RCC_HSE_ON);
         /* �ȴ�HSE�����ȶ�*/
          HSEStartUpStatus = RCC_WaitForHSEStartUp();
          /* �ж�HSE���Ƿ���ɹ����������if()�ڲ� */
          if(HSEStartUpStatus == SUCCESS)
          {
                /* Enable Prefetch Buffer */
                FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
                /* ѡ��HCLK��AHB��ʱ��ԴΪSYSCLK 1��Ƶ */
                FLASH_SetLatency(FLASH_Latency_2);
                RCC_HCLKConfig(RCC_SYSCLK_Div1);      /* ѡ��PCLK2ʱ��ԴΪ HCLK��AHB�� 1��Ƶ */
                RCC_PCLK2Config(RCC_HCLK_Div1);       
                RCC_PCLK1Config(RCC_HCLK_Div2);       /* ѡ��PCLK1ʱ��ԴΪ HCLK��AHB�� 2��Ƶ */
                RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  /* ѡ�����໷��PLL��ʱ��ԴΪHSE 1��Ƶ����Ƶ��Ϊ9����PLL���Ƶ��Ϊ 8MHz * 9 = 72MHz */
                RCC_PLLCmd(ENABLE);
            
                #if defined (STM32F10X_LD_VL) || (STM32F10X_LD_VL) || (STM32F10X_LD_VL)
                RCC_ADCCLKConfig(RCC_PCLK2_Div2);
                #else
                RCC_ADCCLKConfig(RCC_PCLK2_Div4);
                #endif

                /* Wait till PLL is ready */
                while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
                {
                }

                RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);     /* Select PLL as system clock source */

                /* Wait till PLL is used as system clock source */
                while(RCC_GetSYSCLKSource() != 0x08)
                {
                }
          }
            /* TIM2 clock enable */
          RCC_APB1PeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART2, ENABLE);

          RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
                                                                                                |RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1, ENABLE);
         
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
       NVIC_InitTypeDef NVIC_InitStructure;


        #ifdef  VECT_TAB_RAM  
        /* Set the Vector Table base location at 0x20000000 */ 
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);

        #else  /* VECT_TAB_FLASH  */
        /* Configure one bit for preemption priority */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        /* Enable the TIM2 global Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
		NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);     /*��仰�ɲ�������*/
    
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);
	    //USART2
	    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);
				
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);

        /* Set the Vector Table base location at 0x08002000 -> USE AIP*/ 
        //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);  
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);  //only app, no boot included
        
        #endif
}

/*ϵͳ��������˵��λ*/
void system_softreset(void)
{
    __set_FAULTMASK(1);  /* �ر������ж� */
    NVIC_SystemReset();  /* ��λ */
    
}
