#include "common.h"

u8 DataRead[con_size];

void AD_delay()						//220ns����ʱ
{
	__nop();__nop();__nop();__nop();__nop();  __nop();__nop();__nop();__nop();__nop();   
}
float PH = 0.0;

uint16_t GetPH (void)
{
  uint16_t AD_Result = 0;
  
  
  AD_Write(AD_ModeReg);
  AD_Write(0x20);
  AD_Write(0x0A);
  
  //
  AD_Write(AD_ConfReg);
  AD_Write(0x10);
  AD_Write(0x80);
  //
  
  AD_Write(AD_SrReg);
  AD_Read(1);	
  while((DataRead[0]&0x80)==0x80)
  { 
  AD_Write(0x40); 
  AD_Read(1);	
  }

  AD_Write(AD_DataReg);
  AD_Read(2);	
  AD_Result = (DataRead[0] << 8) + DataRead[1];
  return AD_Result;
//  if (AD_Result< 32750)   //��  33350
//  {
//    PH = (-0.004 * AD_Result) + 139.12;
//    return PH;
//  }
//  else    //��
//  {
//    PH = (-0.000633579725449 * AD_Result) + 27.75923970433;
//    return PH;
//  }
}

void AD_IoInit()
{
	RCC->APB2ENR|=1<<2;     //����a��ʱ��
	GPIOA->CRL&=0x0000ffff;
	GPIOA->CRL|=0x34330000;	//A6�������� A7��5��4���
	AD_Cs=1;								//Ƭѡǯס
	AD_Dout=1;							//outĬ��Ϊ��
}

void AD_Init()
{
	int ResetTime;
	AD_IoInit();
	ResetTime=32;
	AD_Sclk=1;

 	AD_Cs=0;		  
 	AD_Din=1;
 	while(ResetTime--)
	{
		AD_delay();
		AD_Sclk=0;
		AD_delay();
 		AD_Sclk=1;
	}
 	AD_Cs=1;	
		
	AD_Write(AD_ConfReg); //д���üĴ���
	AD_Write(0x00); //δʹ���Ǳ�Ŵ���  1.25 V
	AD_Write(0x91); //�����ڲ�Ϊ��׼��Դ
//	WriteToReg(0x0d); //�����ⲿΪ��׼��Դ
//	WriteToReg(AD_IoReg);
//	WriteToReg(0x00);
}

void AD_Write(u8 ByteData) //ByteData���͵����ݣ�nByte���ͼ����ֽ�
{
	unsigned char temp;
	int i;	
	AD_Cs=0;
	temp=0x80;
	for(i=0;i<8;i++)
	{
 		if((temp & ByteData)==0)
		{		
      AD_Din=0;
		}	
 		else
		{
			 AD_Din=1;
    }
		AD_Sclk=0;
		AD_delay();
	  AD_Sclk=1;
		AD_delay();
 		temp=temp>>1;
	}
	AD_Cs=1;
}


void AD_Read(unsigned char nByte) // nByte ��������д���ֽ���
{
	int i,j;
  u8		temp;
  AD_Din=1;
 	AD_Cs=0;
  temp=0;
	AD_Dout=1;

	for(i=0; i<nByte; i++)
	{
		for(j=0; j<8; j++)
		{
			AD_Sclk=0;
			if(AD_Dout==0)
			{
			temp=temp<<1;
		}else
		{
			temp=temp<<1;
			temp=temp+0x01;
		}
		AD_delay();
		AD_Sclk=1;
		AD_delay();
		}
		DataRead[i]=temp;
		temp=0;
	}
   AD_Cs=1;
}


