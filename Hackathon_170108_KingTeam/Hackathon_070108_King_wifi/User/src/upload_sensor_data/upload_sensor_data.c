#include "upload_sensor_data.h"
#include "get_sensor_data.h"
#include "usart3.h"
#include "comm_struct.h"
#include "sbus.h"
#include "string.h"
#include "stdio.h"

typedef struct data_buff_t_ {
	char data_pack[100];
	uint8_t data_len;
} data_buff_t; //���ݻ�����
///void get_paddy_data(void);
void upload_sensor_dat(void)
{
	
	/*��json���ݰ���������*/
	data_buff_t data_buff;
	dev_time_t dev_time;
	paddy_t paddy;
	devid_t devid;
	float w_t, w_ph, w_do, w_l;
	uint8_t i;
	/*ÿ����֮ǰ��ջ���*/
	memset(data_buff.data_pack, '\0', 100*sizeof(char));
	data_buff.data_len = 0;
	sbus_get(PADDY_INFO_ADDR, PADDY_INFO_LENGTH, (uint16_t *)&paddy);
	sbus_get(DEVID_INFO_ADDR, DEVID_INFO_LENGTH, (uint16_t *)&devid);
	w_t = paddy.water_t / 10.0;  /*�任�ɸ�������*/
	w_ph = paddy.water_ph / 100.0; /*�任�ɸ�������*/
	w_do = paddy.water_do / 100.0;
	w_l = paddy.water_l / 10.0;
	sprintf(data_buff.data_pack,"\"id\":%d,\"water_l\":%.1f,\"water_t\":%.1f,\"water_ph\":%.2f,\"water_do\":%.2f",
			devid.dev_id, w_l, w_t, w_ph, w_do);
	data_buff.data_len = sizeof(data_buff.data_pack);
	/*��������*/
	usart3_printf("{");
	for(i = 0; i < data_buff.data_len; i++){
		usart3_printf("%c", data_buff.data_pack[i]);
	}
	sbus_get(DEV_TIME_INFO_ADDR, DEV_TIME_INFO_LENGTH, (uint16_t*)&dev_time);
	
	//����ʱ���
	usart3_printf(",\"dev_time\":\"20%x-%x-%x %x:%x:%x\"",
		dev_time.year, dev_time.month, dev_time.day, dev_time.hour, dev_time.min, dev_time.sec);
	usart3_printf("}");
	
}

/*�ϴ���������*/
void upload_bugs_num(void)
{
	plant_protect_t plant_protect;
	dev_time_t dev_time;
	devid_t devid;
	sbus_get(PLANT_PROTECT_INFO_ADDR, PLANT_PROTECT_INFO_LENGTH, (uint16_t*)&plant_protect);
    sbus_get(DEVID_INFO_ADDR, DEVID_INFO_LENGTH, (uint16_t *)&devid);
	usart3_printf("{");
	usart3_printf("\"id\":%d,\"bugs_n\":%d",devid.dev_id, plant_protect.bugs_n);
	sbus_get(DEV_TIME_INFO_ADDR, DEV_TIME_INFO_LENGTH, (uint16_t*)&dev_time);
	//����ʱ���
	usart3_printf(",\"dev_time\":\"20%x-%x-%x %x:%x:%x\"",
		dev_time.year, dev_time.month, dev_time.day, dev_time.hour, dev_time.min, dev_time.sec);
	usart3_printf("}");
	//���ڵ�ǰ�汾û�����ݴ洢��ÿ�η������֮�����㣬���ۻ�
	plant_protect.bugs_n = 0;
	sbus_set(PLANT_PROTECT_INFO_ADDR, PLANT_PROTECT_INFO_LENGTH, (const uint16_t*)&plant_protect);
}
