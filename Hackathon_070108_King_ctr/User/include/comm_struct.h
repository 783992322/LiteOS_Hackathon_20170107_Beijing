/*----------------------------------------------------*/
/* �ļ���comm_struct.h
   ���ܣ�����������ݵĽṹ��
   �汾��V1.1 ��������һЩ�ṹ�嶨�壬�������״̬�������Ϣ
   ��ʷ�汾��V1.0 ����Ľṹ�����ڲɼ��˼��            */
/*----------------------------------------------------*/
#ifndef COMM_STRUCT_H_
#define COMM_STRUCT_H_
#include <stdint.h>
#include <stdio.h>
//ˮ�� paddy ���� glebe ˮ�� canal ���� greenhouse ����վ weather station ֲ��  plant protection

#ifdef	__cplusplus
extern "C" {
#endif
	
/*����ָ��cmd*/
typedef struct _cmd_info_t {
	uint16_t cmd_info;
} cmd_t;
/*״̬��Ϣstate*/
typedef struct _state_info_t {
	uint16_t sta_info;
} state_t;
/*���״̬*/
typedef struct _battery_info_t{
	uint16_t v_info;
	uint16_t i_info;
} battery_t;
/*�豸id*/
typedef struct _devid_info_t {
	uint16_t dev_id;
} devid_t;
/*���������*/
/*ˮ��*/
typedef struct _paddy_info_t {
    uint16_t water_l;   /*ˮλ*/
    int16_t  water_t;   /*ˮ��*/
    int16_t  silt_t;    /*����*/
    uint16_t water_ph;  /*PH*/
    uint16_t water_do;  /*�ܽ���*/
	uint16_t water_ec;  /*�絼��*/
} paddy_t;
/*����*/
typedef struct _glebe_info_t {
	int16_t  soil_t;    /*�����¶�*/
    int16_t  soil_5h;   /*5cm����ʪ��*/
    int16_t  soil_10h;  /*10cm����ʪ��*/
	int16_t  soil_15h; 	/*15cm����ʪ��*/
	uint16_t soil_ph;   /*����PH*/
	int16_t  air_t;  	/*�����¶�*/
	int16_t  air_h;  	/*����ʪ��*/
	uint16_t air_l;  	/*����*/
} glebe_t;
/*ˮ��*/
typedef struct _cannl_info_t {
    uint16_t water_l;   /*ˮλ*/
    int16_t  water_t;   /*ˮ��*/
    uint16_t water_ph;  /*PH*/
    uint16_t water_do;  /*�ܽ���*/
	uint16_t water_ec;  /*�絼��*/
	uint16_t water_ntu;  /*�Ƕ�*/
} canal_t;
/*����*/
typedef struct _greenhouse_info_t {
    int16_t soil_t;   	/*�����¶�*/
    int16_t soil_5h;   	/*5cm����ʪ��*/
    int16_t soil_10h;  	/*10cm����ʪ��*/
	int16_t air_t;  	/*�����¶�*/
	int16_t air_h;  	/*����ʪ��*/
	uint16_t air_co2;  	/*������̼*/
	uint16_t air_l;  	/*����*/
	uint16_t soil_ph;  	/*ph*/
	uint16_t air_nh;  	/*����*/
} greenhouse_t;
/*����վ*/
typedef struct _wea_station_info_t {
	int16_t air_t;  	/*�����¶�*/
	int16_t air_h;  	/*����ʪ��*/
	uint16_t air_l;  	/*����*/
	uint16_t air_co2;  	/*������̼*/
	uint16_t soil_ev;  	/*����*/
	uint16_t air_rain;  /*����*/
	uint16_t air_s;  	/*����*/
	uint16_t air_w;  	/*����*/
	uint16_t air_rs;  	/*��ѩ*/
} wea_station_t;
/*ֲ��*/
typedef struct _plant_protect_info_t {

	uint16_t bugs_n;  	/*����*/
} plant_protect_t;

/*�豸״̬*/
/*0���������У�1���豸���ߣ�2���豸�ػ���3���豸���ϣ�4���豸����*/
typedef struct _dev_status_info_t{
    uint16_t device_status;      /*�豸״̬*/
} dev_status_t;
/*ʱ��*/
typedef struct _dev_time_info_t{
	uint16_t sec;
	uint16_t min;
	uint16_t hour;
	uint16_t day;
	uint16_t month;
	uint16_t year;
} dev_time_t;

/* �գ�ռλ      #1     */
/* 0                    */
#define BASE_ADDRESS                      	(0x00)
/*����*/
#define CMD_INFO_ADDR						(BASE_ADDRESS)
#define CMD_INFO_LENGTH						(sizeof(cmd_t) / sizeof(uint16_t))
/*״̬*/
#define STATE_INFO_ADDR						(BASE_ADDRESS + CMD_INFO_LENGTH)
#define STATE_INFO_LENGTH				    (sizeof(state_t) / sizeof(uint16_t))
/*���*/
#define BATTERY_INFO_ADDR					(STATE_INFO_ADDR + STATE_INFO_LENGTH)
#define BATTERY_INFO_LENGTH					(sizeof(battery_t) / sizeof(uint16_t))
	
/*�豸ID*/
#define DEVID_INFO_ADDR           			(BATTERY_INFO_ADDR + BATTERY_INFO_LENGTH)
#define DEVID_INFO_LENGTH        			(sizeof(devid_t) / sizeof(uint16_t))

/* ˮ�����׵�ַ������ */
#define PADDY_INFO_ADDR           			(DEVID_INFO_ADDR + DEVID_INFO_LENGTH)
#define PADDY_INFO_LENGTH        			(sizeof(paddy_t) / sizeof(uint16_t))
/*�������׵�ַ������ */
#define GLEBE_INFO_ADDR           			(PADDY_INFO_ADDR + PADDY_INFO_LENGTH)
#define GLEBE_INFO_LENGTH        			(sizeof(glebe_t) / sizeof(uint16_t))
/*ˮ������׵�ַ������*/
#define CANAL_INFO_ADDR           			(GLEBE_INFO_ADDR + GLEBE_INFO_LENGTH)
#define CANAL_INFO_LENGTH        			(sizeof(canal_t) / sizeof(uint16_t))
/*���Ҽ���׵�ַ������*/
#define GREENHOUSE_INFO_ADDR           		(CANAL_INFO_ADDR + CANAL_INFO_LENGTH)
#define GREENHOUSE_INFO_LENGTH        		(sizeof(greenhouse_t) / sizeof(uint16_t))
/*����վ����׵�ַ������*/
#define WEA_STATION_INFO_ADDR           	(GREENHOUSE_INFO_ADDR + GREENHOUSE_INFO_LENGTH)
#define WEA_STATION_INFO_LENGTH        		(sizeof(wea_station_t) / sizeof(uint16_t))
/*ֲ������׵�ַ������*/
#define PLANT_PROTECT_INFO_ADDR           	(WEA_STATION_INFO_ADDR + WEA_STATION_INFO_LENGTH)
#define PLANT_PROTECT_INFO_LENGTH        	(sizeof(plant_protect_t) / sizeof(uint16_t))
/*�豸����״̬�׵�ַ������*/
#define DEV_STATUS_INFO_ADDR           		(PLANT_PROTECT_INFO_ADDR + PLANT_PROTECT_INFO_LENGTH)
#define DEV_STATUS_INFO_LENGTH        		(sizeof(dev_status_t) / sizeof(uint16_t))
/*�豸����ʱ���׵�ַ������*/
#define DEV_TIME_INFO_ADDR           		(DEV_STATUS_INFO_ADDR + DEV_STATUS_INFO_LENGTH)
#define DEV_TIME_INFO_LENGTH        		(sizeof(dev_time_t) / sizeof(uint16_t))
#ifdef	__cplusplus
}
#endif

#endif // COMM_STRUCT_H_
