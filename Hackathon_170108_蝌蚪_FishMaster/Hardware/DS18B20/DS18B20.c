#include "DS18B20.h"

#define HIGH  1
#define LOW   0

#define DS18B20_IN1_CLK     RCC_APB2Periph_GPIOA
#define DS18B20_IN1_PIN     GPIO_Pin_0                 
#define DS18B20_IN1_PORT		GPIOA 

/*******************************************************************************
* ������	: Delay_1us
* ����	    : us��ʱ����
* �������  : ��
* ���ز���  : ��
********************************************************************************/
extern void Delay_1us(uint16_t time)
{
	uint16_t i=0;
	while(time--)
	{
		i=10;
		while(i--);
	}
}

/*******************************************************************************
* ������	: DS18B20_IN1_GPIO_Config 
* ����	    : 18B20��������Ϊ����ģʽ
* �������  : ��
* ���ز���  : ��
********************************************************************************/
void DS18B20_IN1_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����DS18B20_IN1_PORT������ʱ��*/
	RCC_APB2PeriphClockCmd(DS18B20_IN1_CLK, ENABLE); 

	/*ѡ��Ҫ���Ƶ�DS18B20_IN1_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��DS18B20_IN1_PORT*/
  	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
	
	GPIO_SetBits(DS18B20_IN1_PORT, DS18B20_IN1_PIN);	 
}

/*******************************************************************************
* ������	: DS18B20_IN1_Mode_IPU 
* ����	    : ʹDS18B20_IN1-DATA���ű�Ϊ����ģʽ
* �������  : ��
* ���ز���  : ��
********************************************************************************/
static void DS18B20_IN1_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ѡ��Ҫ���Ƶ�DS18B20_IN1_PORT����*/	
	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;

	/*��������ģʽΪ��������ģʽ*/ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	

	/*���ÿ⺯������ʼ��DS18B20_IN1_PORT*/
	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* ������	: DS18B20_IN1_Mode_Out_PP 
* ����	    : ʹDS18B20_IN1-DATA���ű�Ϊ���ģʽ
* �������  : ��
* ���ز���  : ��
********************************************************************************/
static void DS18B20_IN1_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	 	/*ѡ��Ҫ���Ƶ�DS18B20_IN1_PORT����*/															   
  	GPIO_InitStructure.GPIO_Pin = DS18B20_IN1_PIN;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��DS18B20_IN1_PORT*/
  	GPIO_Init(DS18B20_IN1_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
* ������	: DS18B20_IN1_Rst 
* ����	    : �������ӻ����͸�λ����
* �������  : ��
* ���ز���  : ��
********************************************************************************/
static void DS18B20_IN1_Rst(void)
{
	/* ��������Ϊ������� */
	DS18B20_IN1_Mode_Out_PP();
	
	DS18B20_IN1_DATA_OUT(LOW);
	/* �������ٲ���480us�ĵ͵�ƽ��λ�ź� */
	Delay_1us(750);
	
	/* �����ڲ�����λ�źź��轫�������� */
	DS18B20_IN1_DATA_OUT(HIGH);
	
	/*�ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������*/
	Delay_1us(15);
}

/*******************************************************************************
* ������	: DS18B20_IN1_Presence 
* ����	    : ���ӻ����������صĴ�������
* �������  : ��
* ���ز���  : 0���ɹ�
* 			  1��ʧ��
********************************************************************************/
static uint8_t DS18B20_IN1_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* ��������Ϊ�������� */
	DS18B20_IN1_Mode_IPU();
	
	/* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
	 * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
	 */
	while( DS18B20_IN1_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_1us(1);
	}	
	/* ����100us�󣬴������嶼��û�е���*/
	if( pulse_time >=100 )
		return 1;
	else
		pulse_time = 0;
	
	/* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
	while( !DS18B20_IN1_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		Delay_1us(1);
	}	
	if( pulse_time >=240 )
		return 1;
	else
		return 0;
}

/*******************************************************************************
* ������	: DS18B20_IN1_Read_Bit 
* ����	    : ��DS18B20_IN1��ȡһ��bit
* �������  : ��
* ���ز���  : uint8_t dat
********************************************************************************/
static uint8_t DS18B20_IN1_Read_Bit(void)
{
	uint8_t dat;
	
	/* ��0�Ͷ�1��ʱ������Ҫ����60us */	
	DS18B20_IN1_Mode_Out_PP();
	/* ��ʱ�����ʼ���������������� >1us <15us �ĵ͵�ƽ�ź� */
	DS18B20_IN1_DATA_OUT(LOW);
	Delay_1us(10);
	
	/* ���ó����룬�ͷ����ߣ����ⲿ�������轫�������� */
	DS18B20_IN1_Mode_IPU();
	//Delay_1us(2);
	
	if( DS18B20_IN1_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* �����ʱ������ο�ʱ��ͼ */
	Delay_1us(45);
	
	return dat;
}

/*******************************************************************************
* ������	: DS18B20_IN1_Read_Byte 
* ����	    : ��DS18B20_IN1��һ���ֽڣ���λ����
* �������  : ��
* ���ز���  : uint8_t dat
********************************************************************************/
uint8_t DS18B20_IN1_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_IN1_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*******************************************************************************
* ������	: DS18B20_IN1_Write_Byte 
* ����	    : дһ���ֽڵ�DS18B20_IN1����λ����
* �������  : ��
* ���ز���  : uint8_t dat
********************************************************************************/
void DS18B20_IN1_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_IN1_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* д0��д1��ʱ������Ҫ����60us */
		if (testb)
		{			
			DS18B20_IN1_DATA_OUT(LOW);
			/* 1us < �����ʱ < 15us */
			Delay_1us(8);
			
			DS18B20_IN1_DATA_OUT(HIGH);
			Delay_1us(58);
		}		
		else
		{			
			DS18B20_IN1_DATA_OUT(LOW);
			/* 60us < Tx 0 < 120us */
			Delay_1us(70);
			
			DS18B20_IN1_DATA_OUT(HIGH);			
			/* 1us < Trec(�ָ�ʱ��) < �����*/
			Delay_1us(2);
		}
	}
}

/*******************************************************************************
* ������	: DS18B20_IN1_Start 
* ����	    : DS18B20_IN1����
* �������  : ��
* ���ز���  : ��
********************************************************************************/
void DS18B20_IN1_Start(void)
{
	DS18B20_IN1_Rst();	   
	DS18B20_IN1_Presence();	 
	DS18B20_IN1_Write_Byte(0XCC);		/* ���� ROM */
	DS18B20_IN1_Write_Byte(0X44);		/* ��ʼת�� */
}

/*******************************************************************************
* ������	: DS18B20_IN1_Init 
* ����	    : DS18B20_IN1��ʼ��
* �������  : ��
* ���ز���  : 0���ɹ�
* 			  1��ʧ��
********************************************************************************/
uint8_t DS18B20_IN1_Init(void)
{
	DS18B20_IN1_GPIO_Config();
	DS18B20_IN1_Rst();
	
	return DS18B20_IN1_Presence();
}

/*
 * �洢���¶���16 λ�Ĵ�������չ�Ķ����Ʋ�����ʽ
 * ��������12λ�ֱ���ʱ������5������λ��7������λ��4��С��λ
 *
 *         |---------����----------|-----С�� �ֱ��� 1/(2^4)=0.0625----|
 * ���ֽ�  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----����λ��0->��  1->��-------|-----------����-----------|
 * ���ֽ�  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * �¶� = ����λ + ���� + С��*0.0625
 */
u8 DS18B20_IN1_Get_Temp(void)
{
    u8 TL,TH,t1,t;
	
	DS18B20_IN1_Rst();	   
	DS18B20_IN1_Presence();	 
	DS18B20_IN1_Write_Byte(0XCC);				/* ���� ROM */
	DS18B20_IN1_Write_Byte(0X44);				/* ��ʼת�� */
	
	DS18B20_IN1_Rst();
    DS18B20_IN1_Presence();
	DS18B20_IN1_Write_Byte(0XCC);				/* ���� ROM */
    DS18B20_IN1_Write_Byte(0XBE);				/* ���¶�ֵ */
	
	TL = DS18B20_IN1_Read_Byte();
	TH = DS18B20_IN1_Read_Byte();
	//ȡ�����ķ�ʽ
	t1=(TL&0x0f)*0.0625;//ȡС������

	t=((TH<<4)|(TL>>4));//ȡ��������
	if(t>0xc0)
	{
	t=~t+1; 

	}
	t=t+t1;


	return(t); 
}

/*************************************END OF FILE******************************/
