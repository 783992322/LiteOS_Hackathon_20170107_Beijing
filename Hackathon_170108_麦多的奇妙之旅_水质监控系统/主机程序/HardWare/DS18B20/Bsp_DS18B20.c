#include "Common.h"

  /**
  * @brief  ����DS18B20���������ƽ
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
static void DS18B20_DATA_OUT (int InOrEx, int Statue)
{
  if (InOrEx == DS18B20IN)	 
  {       
    if (Statue)  
    {     
      GPIO_SetBits(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);     
    }     
    else	
    {     
      GPIO_ResetBits(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);   
    }     
  }   
  else
  {
    if (Statue)  
    {     
      GPIO_SetBits(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);     
    }     
    else	
    {     
      GPIO_ResetBits(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);   
    } 
  }  
}

  /**
  * @brief  ��ȡDS18B20���ŵ�ƽ
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
static uint8_t DS18B20_DATA_IN (int InOrEx)
{
  if (InOrEx == DS18B20IN)    
  {                           
    return GPIO_ReadInputDataBit(GPIOx_DS18B20IN,GPIO_Pin_DS18B20IN);  
  } 
  else
  {
    return GPIO_ReadInputDataBit(GPIOx_DS18B20EX,GPIO_Pin_DS18B20EX);  
  }
}


  /**
  * @brief  ��DS18B20���ű�Ϊ����ģʽ
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
  static void DS18B20_Mode_IPU(int InOrEx)
  {
    if (InOrEx == DS18B20IN)
    {
      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
      GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
    }
    else
    {
      GPIO_InitTypeDef GPIO_InitStructure;
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
      GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
    }
  }

 /**
  * @brief  ��DS18B20���ű�Ϊ���ģʽ
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
static void DS18B20_Mode_Out_PP(int InOrEx)
{
 	GPIO_InitTypeDef GPIO_InitStructure;		
  
  if (InOrEx == DS18B20IN)
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
  }
  else
  {
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
  }
}


 /**
  * @brief  ������DS18B20���͸�λ����
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
void DS18B20_Rst(int InOrEx)	   
{                 
  DS18B20_Mode_Out_PP(InOrEx);
  DS18B20_DATA_OUT(InOrEx, LOW); 
  Delay_us(750);   
  DS18B20_DATA_OUT(InOrEx, HIGH); 
  Delay_us(15);   
}


 /**
  * @brief  ���ӻ����������صĴ�������
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ��
  */
static uint8_t DS18B20_Presence(int InOrEx)
{
	uint8_t pulse_time = 0;
	
  DS18B20_Mode_IPU(InOrEx);     /* ��������Ϊ�������� */
  
  /* �ȴ���������ĵ�������������Ϊһ��60~240us�ĵ͵�ƽ�ź� 
   * �����������û����������ʱ�����ӻ����յ������ĸ�λ�źź󣬻���15~60us���������һ����������
   */
  while(DS18B20_DATA_IN(InOrEx) && pulse_time<100)
  {
    pulse_time++;
    Delay_us(1);
  }	
 
  if( pulse_time >=100 )        /* ����100us�󣬴������嶼��û�е���*/
    return 1;
  else
    pulse_time = 0;
  
  while(!DS18B20_DATA_IN(InOrEx) && pulse_time<240)   /* �������嵽�����Ҵ��ڵ�ʱ�䲻�ܳ���240us */
  {
    pulse_time++;
    Delay_us(1);
  }	
  if( pulse_time >=240 )
    return 1;
  else
    return 0;	
}

 /**
  * @brief  ��DS18B20����һ���ֽ�����
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  *         dat   :   Ҫ���͵�����
  * @retval ��
  */
void DS18B20_Write_Byte(int InOrEx, uint8_t dat)
{
	uint8_t i, testb;
	DS18B20_Mode_Out_PP(InOrEx);
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		if (testb)
		{			
      DS18B20_DATA_OUT(InOrEx, LOW);
      Delay_us(8); 
      DS18B20_DATA_OUT(InOrEx, HIGH);
      Delay_us(58);
		}		
		else
		{		
      DS18B20_DATA_OUT(InOrEx, LOW);
      Delay_us(70);
      DS18B20_DATA_OUT(InOrEx, HIGH);			
      Delay_us(2);
		}
	}
}

 /**
  * @brief  ��DS18B20��һλ����
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ����������
  */
static uint8_t DS18B20_Read_Bit(int InOrEx)
{
	uint8_t dat;
	
	DS18B20_Mode_Out_PP(InOrEx);
	DS18B20_DATA_OUT(InOrEx, LOW);
	Delay_us(10);
	DS18B20_Mode_IPU(InOrEx);
	if( DS18B20_DATA_IN(InOrEx) == SET )
		dat = 1;
	else
		dat = 0;

	Delay_us(45);
	return dat;
}

 /**
  * @brief  ��DS18B20��һ���ֽ�����
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval ����������
  */
uint8_t DS18B20_Read_Byte(int InOrEx)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit(InOrEx);		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

 /**
  * @brief  ��ȡ�¶�
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval �¶�ֵ
  */
float DS18B20_Get_Temp(int InOrEx)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst(InOrEx);	   
	DS18B20_Presence(InOrEx);	 
	DS18B20_Write_Byte(InOrEx, 0XCC);				/* ���� ROM */
	DS18B20_Write_Byte(InOrEx, 0X44);				/* ��ʼת�� */
	
	DS18B20_Rst(InOrEx);
  DS18B20_Presence(InOrEx);
	DS18B20_Write_Byte(InOrEx, 0XCC);				/* ���� ROM */
  DS18B20_Write_Byte(InOrEx, 0XBE);				/* ���¶�ֵ */
	
	tplsb = DS18B20_Read_Byte(InOrEx);		 
	tpmsb = DS18B20_Read_Byte(InOrEx); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ���¶� */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}

 /**
  * @brief  ��ʼ��DS18B20����
  * @param  InOrEx��  DS18B20IN��ѡ�����DS18B20
  *                   DS18B20EX��ѡ���ⲿDS18B20
  * @retval 0�� ��ʼ���ɹ�
  *         1�� ��ʼ��ʧ��
  */
uint8_t DS18B20_Init (int InOrEx)
{
  if (InOrEx == DS18B20IN)    //��ʼ������DS18B20
  {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_DS18B20IN, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20IN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20IN, &GPIO_InitStructure);
    
    DS18B20_Rst(DS18B20IN);	
    return DS18B20_Presence(DS18B20IN);
  }
  else                         //��ʼ���ⲿDS18B20
  {
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx_DS18B20EX, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_DS18B20EX;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOx_DS18B20EX, &GPIO_InitStructure);
    
    DS18B20_Rst(DS18B20EX);	
    return DS18B20_Presence(DS18B20EX);
  }  
}
