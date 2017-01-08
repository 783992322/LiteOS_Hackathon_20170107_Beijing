/*************************************************************
 
  Description: Digital temperature and humidity sensor driver code
  
*************************************************************/
#include "sht10.h"
#include <math.h>
#include "ult.h"
#include "xassert.h"
#include "string.h"
/*CRC8У���, ������sht10��У���*/
const uint8_t crc8_code[] = {0, 49,
98, 83, 196, 245, 166, 151, 185, 136, 219,
234, 125, 76, 31, 46, 67, 114, 33, 16, 135,
182, 229, 212, 250, 203, 152, 169, 62, 15,
92, 109, 134, 183, 228, 213, 66, 115, 32,
17, 63, 14, 93,108, 251, 202, 153, 168,
197, 244, 167, 150, 1, 48, 99, 82, 124, 77,
30, 47, 184, 137, 218, 235, 61, 12, 95,
110, 249, 200, 155, 170, 132, 181, 230,
215, 64, 113, 34, 19, 126, 79, 28, 45, 186,
139, 216, 233, 199, 246, 165, 148, 3, 50,
97, 80, 187, 138, 217, 232, 127, 78, 29,
44, 2, 51, 96, 81, 198, 247, 164, 149, 248,
201, 154, 171, 60, 13, 94, 111, 65, 112,
35, 18, 133, 180, 231, 214, 122, 75, 24,
41, 190, 143, 220, 237, 195, 242, 161, 144,
7, 54, 101, 84, 57, 8, 91, 106, 253, 204,
159, 174, 128, 177, 226, 211, 68, 117, 38,
23, 252, 205, 158, 175, 56, 9, 90, 107, 69,
116, 39, 22, 129, 176, 227, 210, 191, 142,
221, 236, 123, 74, 25, 40, 6, 55, 100, 85,
194, 243, 160, 145, 71, 118, 37, 20, 131,
178, 225, 208, 254, 207, 156, 173, 58, 11,
88, 105, 4, 53, 102, 87, 192, 241, 162,
147, 189, 140, 223, 238, 121, 72, 27, 42,
193, 240, 163, 146, 5, 52, 103, 86, 120,
73, 26, 43, 188, 141, 222, 239, 130, 179,
224, 209, 70, 119, 36, 21, 59, 10, 89, 104,
255, 206, 157, 172};
/* CRCУ��, ����CRC8У��ֵ*/
uint8_t crc8_check(uint8_t *p, uint8_t counter)
{
    uint8_t crc8 = 0;

    for( ; counter > 0; counter--){
        crc8 = crc8_code[crc8^*p];
        p++;
    }
    return(crc8);

}
/*��λ���з�ת*/
uint8_t reverse_bit(uint8_t val)
{
    uint8_t val_bit[8];
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        if(val & 0x80)
            val_bit[i] = 1;
        else
            val_bit[i] = 0;
        val = (val << 1);
    }
    val = 0;
    for(i = 0; i < 8; i++)
    {
        if(val_bit[i])
            val |= (1 << i);
        else
            val &= ~(1 << i);
    }
    return val;
    
}
/*************************************************************
  Function   ��SHT10_Dly  
  Description��SHT10ʱ����Ҫ����ʱ
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_Dly(void)
{
	u16 i;
	for(i = 500; i > 0; i--);
    //delay_us(10);
}


/*************************************************************
  Function   ��SHT10_Config  
  Description����ʼ�� SHT10����
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	//��ʼ��SHT10����ʱ��
	RCC_APB2PeriphClockCmd(SHT10_AHB2_CLK ,ENABLE);
		
	// DATA �������	
	GPIO_InitStructure.GPIO_Pin = SHT10_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SHT10_DATA_PORT, &GPIO_InitStructure);
	// SCK �������
	GPIO_InitStructure.GPIO_Pin = SHT10_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SHT10_SCK_PORT, &GPIO_InitStructure);

	sht10_conreset();	//��λͨѶ
}


/*************************************************************
  Function   ��SHT10_DATAOut
  Description������DATA����Ϊ���
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_DATAOut(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA �������	
	GPIO_InitStructure.GPIO_Pin = SHT10_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 
	GPIO_Init(SHT10_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ��SHT10_DATAIn  
  Description������DATA����Ϊ����
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_DATAIn(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//PD0 DATA ��������	
	GPIO_InitStructure.GPIO_Pin = SHT10_DATA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SHT10_DATA_PORT, &GPIO_InitStructure);
}


/*************************************************************
  Function   ��SHT10_WriteByte  
  Description��д1�ֽ�
  Input      : value:Ҫд����ֽ�        
  return     : err: 0-��ȷ  1-����    
*************************************************************/
u8 SHT10_WriteByte(u8 value)
{
	u8 i, err = 0;
	
	SHT10_DATAOut();			  //����DATA������Ϊ���

	for(i = 0x80; i > 0; i /= 2)  //д1���ֽ�
	{
		if(i & value)
			SHT10_DATA_H();
		else
			SHT10_DATA_L();
		SHT10_Dly();
		SHT10_SCK_H();
		SHT10_Dly();
		SHT10_SCK_L();
		SHT10_Dly();
	}
	SHT10_DATAIn();				  //����DATA������Ϊ����,�ͷ�DATA��
	SHT10_SCK_H();
	err = SHT10_DATA_R();		  //��ȡSHT10��Ӧ��λ
	SHT10_SCK_L();

	return err;
}

/*************************************************************
  Function   ��SHT10_ReadByte  
  Description����1�ֽ�����
  Input      : Ack: 0-��Ӧ��  1-Ӧ��        
  return     : err: 0-��ȷ 1-����    
*************************************************************/
u8 SHT10_ReadByte(u8 Ack)
{
	u8 i, val = 0;

	SHT10_DATAIn();				  //����DATA������Ϊ����
	for(i = 0x80; i > 0; i /= 2)  //��ȡ1�ֽڵ�����
	{
		SHT10_Dly();
		SHT10_SCK_H();
		SHT10_Dly();
		if(SHT10_DATA_R())
			val = (val | i);
		SHT10_SCK_L();
	}
	SHT10_DATAOut();			  //����DATA������Ϊ���
	if(Ack)
		SHT10_DATA_L();			  //Ӧ��������ȥ������ȥ������(У������)
	else
		SHT10_DATA_H();			  //��Ӧ���������˽���
	SHT10_Dly();
	SHT10_SCK_H();
	SHT10_Dly();
	SHT10_SCK_L();
	SHT10_Dly();

	return val;					  //���ض�����ֵ
}


/*************************************************************
  Function   ��SHT10_TransStart  
  Description����ʼ�����źţ�ʱ�����£�
                     _____         ________
               DATA:      |_______|
                         ___     ___
               SCK : ___|   |___|   |______	
  Input      : none        
  return     : none    
*************************************************************/
void SHT10_TransStart(void)
{
	SHT10_DATAOut();			  //����DATA������Ϊ���

	SHT10_DATA_H();
	SHT10_SCK_L();
	SHT10_Dly();
	SHT10_SCK_H();
	SHT10_Dly();
	SHT10_DATA_L();
	SHT10_Dly();
	SHT10_SCK_L();
	SHT10_Dly();
	SHT10_SCK_H();
	SHT10_Dly();
	SHT10_DATA_H();
	SHT10_Dly();
	SHT10_SCK_L();

}


/*************************************************************
  Function   ��SHT10_ConReset  
  Description��ͨѶ��λ��ʱ�����£�
                     _____________________________________________________         ________
               DATA:                                                      |_______|
                        _    _    _    _    _    _    _    _    _        ___     ___
               SCK : __| |__| |__| |__| |__| |__| |__| |__| |__| |______|   |___|   |______
  Input      : none        
  return     : none    
*************************************************************/
void sht10_conreset(void)
{
	u8 i;

	SHT10_DATAOut();

	SHT10_DATA_H();
	SHT10_SCK_L();

	for(i = 0; i < 9; i++)		  //����SCKʱ��9c��
	{
		SHT10_SCK_H();
		SHT10_Dly();
		SHT10_SCK_L();
		SHT10_Dly();
	}
	SHT10_TransStart();			  //��������
}



/*************************************************************
  Function   ��SHT10_SoftReset  
  Description����λ
  Input      : none        
  return     : err: 0-��ȷ  1-����    
*************************************************************/
u8 SHT10_SoftReset(void)
{
	u8 err = 0;

	sht10_conreset();			      //ͨѶ��λ
	err += SHT10_WriteByte(SOFTRESET);//дRESET��λ����

	return err;
}


/*************************************************************
  Function   ��SHT10_ReadStatusReg  
  Description����״̬�Ĵ���
  Input      : p_value-���������ݣ�p_checksun-������У������       
  return     : err: 0-��ȷ  0-����    
*************************************************************/
u8 SHT10_ReadStatusReg(u8 *p_value, u8 *p_checksum)
{
	u8 err = 0;

	SHT10_TransStart();					//��ʼ����
	err = SHT10_WriteByte(STATUS_REG_R);//дSTATUS_REG_R��ȡ״̬�Ĵ�������
	*p_value = SHT10_ReadByte(ACK);		//��ȡ״̬����
	*p_checksum = SHT10_ReadByte(noACK);//��ȡ���������
	
	return err;
}



/*************************************************************
  Function   ��SHT10_WriteStatusReg  
  Description��д״̬�Ĵ���
  Input      : p_value-Ҫд�������ֵ       
  return     : err: 0-��ȷ  1-����    
*************************************************************/
u8 SHT10_WriteStatusReg(u8 *p_value)
{
	u8 err = 0;

	SHT10_TransStart();					 //��ʼ����
	err += SHT10_WriteByte(STATUS_REG_W);//дSTATUS_REG_Wд״̬�Ĵ�������
	err += SHT10_WriteByte(*p_value);	 //д������ֵ

	return err;
}



/*************************************************************
  Function   ��SHT10_Measure  
  Description������ʪ�ȴ�������ȡ��ʪ��
  Input      : p_value-������ֵ��p_checksum-������У����        
  return     : err: 0-��ȷ 1������    -1 У�����  
*************************************************************/
int8_t  sht10_measure(u16 *p_value, u8 *p_checksum, u8 mode)
{
	u8 err = 0;
	u32 i;
	u8 value_H = 0;
	u8 value_L = 0;
    uint8_t crc_data[3];
    uint8_t crc = 0;
	SHT10_TransStart();						 //��ʼ����
	switch(mode)							 
	{
	case TEMP:								 //�����¶�
		err += SHT10_WriteByte(MEASURE_TEMP);//дMEASURE_TEMP�����¶�����
        crc_data[0] = MEASURE_TEMP;  //CRCУ�飬 ����+�¶ȸ�ֵ+�¶ȵ�ֵ
		break;
	case HUMI:
		err += SHT10_WriteByte(MEASURE_HUMI);//дMEASURE_HUMI����ʪ������
        crc_data[0] = MEASURE_HUMI;  //CRCУ�飬����+ʪ�ȸ�ֵ+ʪ�ȵ�ֵ
		break;
	default:
		break;
	}
	SHT10_DATAIn();
	for(i = 0; i < 72000000; i++)		     //�ȴ�DATA�źű�����
	{
		if(SHT10_DATA_R() == 0) break;	     //��⵽DATA�������ˣ�����ѭ��
	}
	if(SHT10_DATA_R() == 1)   			     //����ȴ���ʱ��
		err += 1;
    xassert(err == 0);                       //�豸������ err = 0
	value_H = SHT10_ReadByte(ACK);
	value_L = SHT10_ReadByte(ACK);
    //sht10CRCУ���������ֵ����һ����������¶ȡ���ʪ�ȣ����ڶ����Ƕ���ֵ�ĸ߰�λ������ǵͰ�λ
    crc_data[1] = value_H;
    crc_data[2] = value_L;
    //�õ�һ����λ��ֵ��Ҫ��ת����������CRCֵ��D7 - D7��......)
    crc = crc8_check(crc_data, sizeof(crc_data));
    crc = reverse_bit(crc);
	*p_checksum = SHT10_ReadByte(noACK);  	 //��ȡУ������
	*p_value = (value_H << 8) | value_L;
    xassert(crc == *p_checksum);   //���������ֵ��ȷ����ôcrcУ��ҲӦ����ȷ
    if(crc != *p_checksum)  //��ȡֵ����
        return ERROR_CRC;
	return err;
}


/*************************************************************
  Function   ��SHT10_Calculate  
  Description��������ʪ�ȵ�ֵ
  Input      : Temp-�Ӵ������������¶�ֵ��Humi-�Ӵ�����������ʪ��ֵ
               p_humidity-�������ʵ�ʵ�ʪ��ֵ��p_temperature-�������ʵ���¶�ֵ        
  return     : none    
*************************************************************/
void sht10_calculate(u16 t, u16 rh, float *p_temperature, float *p_humidity)
{
	const float d1 = -39.7;
	const float d2 = +0.01;
	const float C1 = -2.0468;
	const float	C2 = +0.0367;
	const float C3 = -0.0000015955;	
	const float T1 = +0.01;
	const float T2 = +0.00008;

	float RH_Lin;  										//RH����ֵ	
	float RH_Ture; 										//RH��ʵֵ
	float temp_C;

	temp_C = d1 + d2 * t;  							    //�����¶�ֵ	
	RH_Lin = C1 + C2 * rh + C3 * rh * rh;			    //����ʪ��ֵ
	RH_Ture = (temp_C -25) * (T1 + T2 * rh) + RH_Lin;	//ʪ�ȵ��¶Ȳ���������ʵ�ʵ�ʪ��ֵ

	if(RH_Ture > 100)									//����ʪ��ֵ����
		RH_Ture	= 100;
	if(RH_Ture < 0.1)
		RH_Ture = 0.1;									//����ʪ��ֵ����
    if(temp_C > 100)
        temp_C = 100;
	*p_humidity = RH_Ture;
	*p_temperature = temp_C;

}


/*************************************************************
  Function   ��SHT10_CalcuDewPoint  
  Description������¶��
  Input      : h-ʵ�ʵ�ʪ�ȣ�t-ʵ�ʵ��¶�        
  return     : dew_point-¶��    
*************************************************************/
float sht10_calcu_dewpoint(float t, float h)
{
	float logEx, dew_point;

	logEx = 0.66077 + 7.5 * t / (237.3 + t) + (log10(h) - 2);
	dew_point = ((0.66077 - logEx) * 237.3) / (logEx - 8.16077);

	return dew_point; 
}
/*sht10 ��ʼ��*/
void sht10_init(void)
{
    uint8_t error = 0;
    SHT10_Config();
    error = SHT10_SoftReset();
    xassert(error == 0);
    delay_ms(20);
}
