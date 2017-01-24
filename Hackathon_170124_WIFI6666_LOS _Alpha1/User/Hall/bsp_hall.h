#ifndef __HALL_H
#define	__HALL_H


#include "stm32f10x.h"

//  ���Ŷ���
#define    HALL1_GPIO_CLK     RCC_APB2Periph_GPIOE
#define    HALL1_GPIO_PORT    GPIOE			   
#define    HALL1_GPIO_PIN		  GPIO_Pin_5

//#define    HALL2_GPIO_CLK     RCC_APB2Periph_GPIOC
//#define    HALL2_GPIO_PORT    GPIOE		   
//#define    HALL2_GPIO_PIN		  GPIO_Pin_2


 /** �������±��ú�
	*  ��������Ϊ�ߵ�ƽ������ HALL_ON=1�� HALL_OFF=0
	*  ����������Ϊ�͵�ƽ���Ѻ����ó�HALL_ON=0 ��HALL_OFF=1 ����
	*/
#define HALL_ON	1
#define HALL_OFF	0

void HALL_GPIO_Config(void);
uint8_t HALL_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);


#endif /* __HALL_H */

