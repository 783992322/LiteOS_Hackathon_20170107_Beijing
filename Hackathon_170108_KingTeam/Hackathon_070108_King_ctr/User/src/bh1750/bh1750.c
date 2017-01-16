#include "bh1750.h"
#include "i2c_hard.h"
#include "ult.h"
#include "xassert.h"
extern I2C_INIT_INFO I2C_Info;	///<I2C�����������Ϣ
uint8_t I2C_Data[10] ,ret;


/*BH1750��ʼ��*/
void bh1750_init(void)
{
    //����BH1750FVI����������I2C��ʼ������
	I2C_Info.controlMode = 1;//Ӳ������
	I2C_Info.speed = 100000;//100K
	I2C_Info.subAddrMode = 2;//���ӵ�ַģʽ
	I2C_Info.subAddrWidth = 0;//�ӵ�ַΪ0�ֽڿ��
	I2C_Info.slaveAddr = 0x46;//BH1750FVI������I2C��ַ
	I2C_Info.channel = I2C1_CHANNEL;//ʹ��I2C1, 
	VT_I2C_HardInit(&I2C_Info);
	I2C_Data[0] = 0x01;
	VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//������оƬ�ϵ�����
}

/**
 * ��bh1750�����ж�ȡ����ֵ
 * ����ֵ��OK 0��ERROR -1
 * ������ֵ����lux_val��
 *************************/
int8_t bh1750_get_lux_val(float *lux_val)
{
    I2C_Data[0] = 0x11;  /*�߷ֱ�ģʽ, 0.5lux*/
    ret = VT_I2C_HardWriteNByte(I2C1,0,I2C_Data,1);//���Ͳ�������
    if(ret)
    {
        xassert(ret == BH1750_WRITE_OK);
        return BH1750_WRITE_ERROR;     //����һ���ﲻ���Ĵ���ֵ
    }
    
    //delay_ms(120);     //�ȴ�����120ms������������ʱ�����򴫸���û׼�������ݻ���ֶ����ݴ���
    ret = VT_I2C_HardReadNByte(I2C1,0,I2C_Data,2);//��ȡ����ǿ��ֵ
    if(ret)
    {
        xassert(ret == BH1750_READ_OK);
        return BH1750_READ_ERROR;   //����һ���ﲻ���Ĵ���ֵ
    }
    *lux_val = ((I2C_Data[0]<<8)+I2C_Data[1])/(1.2*2);
    return BH1750_READ_OK;
    
    
}
