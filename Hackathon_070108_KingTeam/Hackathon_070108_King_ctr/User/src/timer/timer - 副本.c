//��ʱ��
#include "timer.h"
#include <stdio.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "xassert.h"
#include "ds18b20.h"
#include "adc.h"
#include "usart3.h"
#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)
static int16_t temp_val;
static uint16_t level_val;
static uint16_t do_val;
static uint16_t ec_val;
static uint16_t ph_val;
extern vu16  After_filter[M];    //���������ƽ��ֵ֮��Ľ��
uint8_t frame_head[4] = {0x55, 0xaa, 0x08, 0x04};
void Timer_Config(void)         //??????

{  
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  //Timer2ʹ��
		TIM_DeInit(TIM2);                              //��λTIM2��ʱ��     
		TIM_TimeBaseStructure.TIM_Period=6000;          //��ʱ�����ڣ������ʾ��ʱ=100*500us = 50ms��
		TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //Ԥ��Ƶ��,�����ʾ500us
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM2ʱ�ӷ��䣬1��ʾ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//��ʱ������Ϊ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);     
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);           //�����ʱ��2�������־λ
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      //ʹ�ܶ�ʱ��2����ж�
		TIM_Cmd(TIM2, ENABLE);                       //��ʱ��2ʹ��
}

/*��ʼ����ʱ��*/
void timer_init(void)
{
		
		Timer_Config();
}

/*��ʱ��2�жϲ���*/
void TIM2_IRQHandler(void)
{
	
    static uint8_t clk_cnt = 0;
    static uint8_t temp[10];
    CLI();
    
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //????????????1
	{
			
		  TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //?????????
		  switch(clk_cnt){
              case 0:
                  temp_val = get_temperature_after_filter();
                  break;
              case 1:
                  filter();
                  do_val = After_filter[0];
                  level_val = After_filter[4];
                  ph_val = After_filter[1];
                  ec_val = After_filter[3];
                  break;
              case 2:
                  
                  temp[0] = (uint8_t)(temp_val >> 8);
                  temp[1] = (uint8_t)(temp_val & 0x00ff);
                  temp[2] = (uint8_t)(level_val >> 8);
                  temp[3] = (uint8_t)(level_val & 0x00ff);
                  temp[4] = (uint8_t)(do_val >> 8);
                  temp[5] = (uint8_t)(do_val & 0x00ff);
                  temp[6] = (uint8_t)(ph_val >> 8);
                  temp[7] = (uint8_t)(ph_val & 0x00ff);
                  temp[8] = (uint8_t)(ec_val >> 8);
                  temp[9] = (uint8_t)(ec_val & 0x00ff);
                  //USART3_Puts(frame_head);
                  USART3_Putc(0x55);
                  USART3_Putc(0xaa);
                  USART3_Putc(0x0C);
                  USART3_Putc(0x0A);  //ID
                  USART3_Putc(temp[0]);
                  USART3_Putc(temp[1]);
                  USART3_Putc(temp[2]);
                  USART3_Putc(temp[3]);
                  USART3_Putc(temp[4]);
                  USART3_Putc(temp[5]);  
                  USART3_Putc(temp[6]);
                  USART3_Putc(temp[7]);
                  USART3_Putc(temp[8]);
                  USART3_Putc(temp[9]);
                  USART3_Putc(0xff);              
              default:
                  break;
			}
			if(++clk_cnt == 3)
					clk_cnt = 0;
		  
	}
    SEI();
}
