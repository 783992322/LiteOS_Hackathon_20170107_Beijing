#include "get_sensor_data.h"
#include "adc.h"
#include "sbus.h"
#include "ds18b20.h"
#include "comm_struct.h"
#include "ult.h"
paddy_t paddy;
plant_protect_t plant_protect;
devid_t devid;
/*��������ֵ��ʼ��*/
void get_sensor_data_init(void)
{
	/*��ʼ������������ֵ*/
	paddy.silt_t = 0;
	paddy.water_do = 0;
	paddy.water_l = 0;
	paddy.water_ph = 0;
	paddy.water_t = 0;
	plant_protect.bugs_n = 0;
	devid.dev_id = 1;
	/*�ϴ�����*/
	sbus_set(DEVID_INFO_ADDR, DEVID_INFO_LENGTH, (const uint16_t*)(&devid));
	sbus_set(PADDY_INFO_ADDR, PADDY_INFO_LENGTH, (const uint16_t*)(&paddy));
	sbus_set(PLANT_PROTECT_INFO_ADDR, PLANT_PROTECT_INFO_LENGTH, (const uint16_t*)(&plant_protect));
	
	
}
/*��ȡˮ��ˮ��*/
void get_water_temperature(void)
{
	paddy.water_t = get_temperature_after_filter();
	
}
/*��ȡˮ��ˮλ*/
void get_water_depth(void)
{
	uint16_t l_val = adc_val(0);
	if(l_val < 750)
		l_val = 750;
	paddy.water_l = l_val*0.336 - 250;  /*ʵ��ֵ����10*/
}
/*��ȡˮ��PH*/
void get_paddy_ph(void)
{
	uint16_t ph_val = adc_val(1);
	if(ph_val < 750)
		ph_val = 750;
	paddy.water_ph = ph_val*0.47 - 350; /*ʵ��ֵ*100*/
}
/*��ȡˮ���ܽ���*/
void get_paddy_do(void)
{
	uint16_t do_val = adc_val(2);
	if(do_val < 750)
		do_val = 750;
	paddy.water_do = do_val*0.67 - 500;
	
}
/*��ȡˮ������*/
void get_paddy_silt_t(void)
{
	paddy.silt_t = 0;
}

/*��ȡɱ��Ƽ���*/
void get_plant_protect(void)
{
	
	//plant_protect.bugs_n = adc_val(3);
}
/*��ȡ�豸id*/
void get_devid(void)
{
	devid.dev_id = 17;
}
/*��ȡˮ�ﴫ����ֵ*/
void get_paddy_data(void)
{
	/*��ȡ�¶�ֵ*/
	get_water_temperature();
	/*��ȡˮ��ˮλ*/
	get_water_depth();
	/*��ȡˮ��PH*/
	get_paddy_ph();
	/*��ȡˮ���ܽ���*/
	get_paddy_do();
	/*��ȡˮ������*/
	get_paddy_silt_t();
	sbus_set(PADDY_INFO_ADDR, PADDY_INFO_LENGTH, (const uint16_t*)(&paddy));	
}
