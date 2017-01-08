//��ʱ��
#include "timer.h"
#include <stdio.h>
#include <ctype.h>
#include "stm32f10x.h"
#include "xassert.h"
#include "adc.h"
#include "sbus.h"
#include "comm_struct.h"
#include "usart3.h"
#include "string.h"
#include "usart.h"

#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)

extern volatile uint8_t rcv_jsonpack_flag ; 
extern uint8_t rx_buf_len;
void Timer_Config(void)         //??????

{  
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);  //Timer2ʹ��
		TIM_DeInit(TIM2);                              //��λTIM2��ʱ��     
		TIM_TimeBaseStructure.TIM_Period=2000;          //��ʱ�����ڣ������ʾ��ʱ=100*500us = 50ms��
		TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //Ԥ��Ƶ��,�����ʾ500us
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM2ʱ�ӷ��䣬1��ʾ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//��ʱ������Ϊ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);     
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);           //�����ʱ��2�������־λ
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      //ʹ�ܶ�ʱ��2����ж�
		//TIM_Cmd(TIM2, ENABLE);                       //��ʱ��2ʹ��,����time2�����ж�һ֡���ݣ���ô�տ�ʼ�ǲ�ʹ�ܵģ��Ƚ��յ�����֮����ʹ��
}
void Timer3_Config(void)
{
		TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);  //Timer3ʹ��
	    TIM_DeInit(TIM3);                              //��λTIM3��ʱ��  
		TIM_TimeBaseStructure.TIM_Period=200;          //��ʱ�����ڣ������ʾ��ʱ=100 *500us = 50ms��
		TIM_TimeBaseStructure.TIM_Prescaler=36000-1;     //Ԥ��Ƶ��,�����ʾ500us
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //TIM3ʱ�ӷ��䣬1��ʾ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//��ʱ������Ϊ���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);     
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);           //�����ʱ��3�������־λ
	 	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);      //ʹ�ܶ�ʱ��3����ж�
	    TIM_Cmd(TIM3, ENABLE);  
	
}
/*��ʼ����ʱ��*/
void timer_init(void)
{
		
		Timer_Config();
}
/*��ʼ����ʱ��3*/
void timer3_init(void)
{
		
		Timer3_Config();
}
/*��ʱ��2�жϲ���*/
void TIM2_IRQHandler(void)
{
	
    CLI();
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //????????????1
	{
			
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //?????????
        rcv_jsonpack_flag = 1;
        //rx_buf_len = 0;   /*���¿�ʼ����*/
        time2_disable();
		  
	}
    SEI();
}

///*��ʱ��2�жϲ���*/
//void TIM2_IRQHandler(void)
//{
//	
//    uint8_t i;
//    //static uint8_t temp[10];
//    CLI();
//    
//	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) //????????????1
//	{
//			//��ʱ1s
//		  TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); //?????????
//		  if(rx_buf_len > 0) //����ж��Ƕ����
//				rcv_jsonpack_flag = 1;
//			for(i = 0 ; i < rx_buf_len; i++){
//					usart2_printf("%c",USART2_RX_BUF[i]);
//			}		
//      usart2_printf("\r\n");			
//      time2_disable();	
//	}
//    SEI();
//}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3,  TIM_IT_Update);  //���TIMx�����жϱ�־ 
		//your code
			//printf("this is a test\r\n");
			
		}
}
//��ֹ��ʱ��2
void time2_disable(void)
{
		TIM_Cmd(TIM2, DISABLE);                       //��ʱ��2��ֹ
	
}
//ʹ�ܶ�ʱ��2
void timer2_enable(void)
{
		TIM_Cmd(TIM2, ENABLE);                       //��ʱ��2ʹ��
	
}
