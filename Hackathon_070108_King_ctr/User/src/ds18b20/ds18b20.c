#include "ds18b20.h"
#include "ult.h"
#include "xassert.h"
#include "usart3.h"
/*�����жϣ�DS18B20��ȡ��Ҫʱ�䣬���ܴ��*/
#define EnableINT()  
#define DisableINT()


/*�Ի�ȡ�����¶�ֵ�����˲�,ԭ����ȥ�����ֵ����Сֵ��ȡƽ��ֵ*/
/* �˲����г��� */
#define __FILTER_LEN    8

/* �˲����ݽṹ */
struct _filter_t {
    uint16_t buf[__FILTER_LEN];
    uint16_t index;
};

struct _filter_t  _filter;  //�����洢�¶ȴ�������ֵ
 
 
static unsigned char TempX_TAB[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};
/*CRC8У���*/
const uint8_t crc8_code[]={
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
  190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
  219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
  87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};
/*���¶�ֵ�����˲�, ��ֵ�����з��ŵ�*/
static  int16_t filter(struct _filter_t *filter_p, int16_t value)
{

    uint16_t i;
    uint16_t max, min;
    uint32_t sum = 0;
 
    filter_p->buf[filter_p->index] = value;
    if(++filter_p->index == __FILTER_LEN)
        filter_p->index = 0;
    /* ��Ͳ���������Сֵ */
    max =  filter_p->buf[0];
    min =  filter_p->buf[0];
    for(i = 0; i < __FILTER_LEN; ++i) {
        sum += filter_p->buf[i];
        if(filter_p->buf[i] > max)
            max = filter_p->buf[i];
        if(filter_p->buf[i] < min)
            min = filter_p->buf[i];
    }
    /* ȥ�������С */
    sum -= max;
    sum -= min;
    /* ���ؽ�� */
    return sum / (__FILTER_LEN - 2);
}

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


/*��λDS18B20*/
/*ͬʱ�����豸�Ƿ����ߣ���������ߣ��򷵻�ERROR_NO_DS18B20,���߷���0*/
int16_t ResetDS18B20(void)
{
    
    uint8_t us_delay = 0;
    SetDQ();
    delay_us(50);
    ResetDQ();
    delay_us(500);  //500us (???????????480?960??)
    SetDQ();
    delay_us(40);  //40us
    while(GetDQ() && (us_delay < 240))
    {
        us_delay++;
        delay_us(1);
    }   
    //xassert(us_delay < 240);
    if(us_delay >= 240)
    {
     //����
       
        return ERROR_NO_DS18B20;   //ds18b20������
    }
    delay_us(500);
    SetDQ();
    return OK;
 
}
/*one wire д��һ���ֽ�����*/
void DS18B20WriteByte(unsigned char Dat)
{
    unsigned char i;
    for(i=8;i>0;i--)
    {
           ResetDQ();     //?15u????????,DS18B20?15-60u??
           delay_us(5);    //5us
           if(Dat & 0x01)
                SetDQ();
           else
                ResetDQ();
           delay_us(65);    //65us
           SetDQ();
           delay_us(2);    //????????1us
           Dat >>= 1; 
    } 
}

/*one wire ����һ���ֽ�����*/
unsigned char DS18B20ReadByte(void)
{
     unsigned char i,Dat;
     SetDQ();
     delay_us(5);
     for(i=8;i>0;i--)
     {
            Dat >>= 1;
            ResetDQ();     //???????????????15u?,????????15u???
            delay_us(5);   //5us
            SetDQ();
            delay_us(5);   //5us
            if(GetDQ())
                Dat|=0x80;
            else
                Dat&=0x7f;  
            delay_us(65);   //65us
            SetDQ();
     }
     return Dat;
}

/*��ROM����*/
void ReadRom(unsigned char *Read_Addr)
{
    unsigned char i;
    
    DS18B20WriteByte(ReadROM);
  
     for(i=8;i>0;i--)
     {
          *Read_Addr=DS18B20ReadByte();
          Read_Addr++;
     }
}

/*�����߳�ʼ��*/
void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)
{
     DisableINT();
     ResetDS18B20();
     DS18B20WriteByte(SkipROM); 
     DS18B20WriteByte(WriteScratchpad);
     DS18B20WriteByte(AlarmTL);
     DS18B20WriteByte(AlarmTH);
     DS18B20WriteByte(Precision);

     ResetDS18B20();
     DS18B20WriteByte(SkipROM); 
     DS18B20WriteByte(CopyScratchpad);
     EnableINT();

     while(!GetDQ());  //?????? ///////////
}
/*����һ��ת��*/
void DS18B20StartConvert(void)
{
     DisableINT();
     ResetDS18B20();
     DS18B20WriteByte(SkipROM); 
     DS18B20WriteByte(StartConvert); 
     EnableINT();
}

/*������IOӲ������*/
void DS18B20_Configuration(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;
     
     RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);

     GPIO_InitStructure.GPIO_Pin = DS_DQIO;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; //????
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //2M????
     GPIO_Init(DS_PORT, &GPIO_InitStructure);
    }

/*ds18b20��ʼ��*/
void ds18b20_init(void)
{
     DS18B20_Configuration();
     DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
     DS18B20StartConvert();
}

/*��ȡ�¶�ֵ��9�ֽڵ�ram�� data[0], data[1]���¶ȣ� data[8] ��CRCУ��*/
/*����ֵ*/
signed short ds18b20_read(void)
{
     unsigned char TemperatureL,TemperatureH;
     unsigned int  Temperature;
     unsigned  char data[9];  //����CRCУ��ֵ
     unsigned char crc_val = 0;
     unsigned char i;
     DisableINT();
     //ResetDS18B20();
     xassert(!(ResetDS18B20() == ERROR_NO_DS18B20));  //�˴��������
     if(ResetDS18B20() == ERROR_NO_DS18B20)           //����豸�����ߣ���ô�ͷ���һ������ֵ
          return ERROR_NO_DS18B20;
     DS18B20WriteByte(SkipROM); 
     DS18B20WriteByte(ReadScratchpad);
     for(i = 0 ; i < 9; i++)
        data[i] = DS18B20ReadByte();
     //xassert(!(ResetDS18B20() == ERROR_NO_DS18B20));  //�˴��������
     //if(ResetDS18B20() == ERROR_NO_DS18B20)           //����豸�����ߣ���ô�ͷ���һ������ֵ
     //     return ERROR_NO_DS18B20;
     ResetDS18B20();  
     EnableINT();
     crc_val = crc8_check(data, 8);
     xassert(crc_val == data[8]);
    if(crc_val == data[8]) //У���ֵ��ȷ����ȡ�¶�*/
    {
         TemperatureL = data[0];
         TemperatureH = data[1]; 
    }
    else
         return ERROR_CRC_DS18B20; //���У�鲻�Ծͷ���
    if(TemperatureH & 0x80)
    {
        TemperatureH=(~TemperatureH) | 0x08;
        TemperatureL=~TemperatureL+1;
        if(TemperatureL==0)
            TemperatureH+=1;
    }
    TemperatureH=(TemperatureH<<4)+((TemperatureL&0xf0)>>4);
    TemperatureL=TempX_TAB[TemperatureL&0x0f];
    //bit0-bit7????,bit8-bit14????,bit15????
    Temperature=TemperatureH;
    Temperature=(Temperature<<8) | TemperatureL; 
    DS18B20StartConvert();
    return  Temperature;
 //??16???  bit0-bit7????,bit8-bit14????,bit15????
}

/*�¶�ֵת����ֱ�ӷ��ش����ŵ�ֵ*/
int16_t get_temperature(int16_t hex_val)
{
	int16_t t1;
        uint8_t t2;	
	t1 = (hex_val&(0x7f00))>>8; //����λ
	t2 = (uint8_t)(hex_val&(0x00ff)); //С��?
	if(hex_val & (0x8000)) //�¶ȵ���0���϶�
    {
		hex_val = hex_val & ~(0x8000); // ȥ������λ
        hex_val = t1 * 10 + t2;  //ֻȡ��������ȡ���ţ����ŷ���
        hex_val = -hex_val;       //ȡ��ֵ
        return hex_val;
    }
	else
    {
        hex_val = t1* 10 + t2;     //����
        return hex_val;
    }
        
}

/*��ȡ�˲�����¶�ֵ,�¶�ֵ���з���*/
/*����ֵ��************************
*******1���豸�����ߣ�����850��85�棩
*******2����ȡֵ���󣬷���1000��100�棩
*******3������������ʵ���˲����ֵ
***********************************/
int16_t get_temperature_after_filter(void)
{
    volatile static int16_t t_val = 0;
    static int16_t t_val_filter = 0;
    t_val = ds18b20_read();
    xassert(!(t_val  == ERROR_CRC_DS18B20));
    xassert(!(t_val  == ERROR_NO_DS18B20));
    if((t_val == ERROR_CRC_DS18B20) || (t_val == ERROR_NO_DS18B20))   //���CRCУ����󣬷���100��,����豸�����ߣ��ͷ���85��
    {
        return t_val*10;  //ʵ��Ӧ�÷���850�� 1000
    }
        
    else
    {
         t_val = get_temperature(t_val);
         t_val_filter = filter(&_filter, t_val);
    }
    return t_val_filter;
}
