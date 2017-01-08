#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_
#include "types.h"
#include "stdint.h"
#include "network_conf.h"
/* ��Ʒ�İ汾��Ϣ*/
typedef struct device_ver_t_{
    uint8_t tpye_str[6]; /*�豸����*/
    uint8_t ver[2];  /*ver[0]:���汾�ţ�ver[1]:���汾��*/
    uint8_t serial_num[3]; /*�豸���к�*/
    uint8_t build_date[3]; /*�豸��������*/
    
} device_ver_t;
/*�豸�ϴ�ʱ������*/
typedef struct _upload_delay_time_t{
    
   uint8_t delay_times_h;
   uint8_t delay_times_l;
   // uint16_t delay_times;    /*Ĭ����*/
}upload_delay_time_t;
/*����һ���豸�Ļ�����Ϣ���ã�����eeprom��*/
typedef struct device_config_t_{
    device_ver_t device_ver;                       /* �豸�汾 */
    net_config_t net_config;                       /*�����豸��������*/
    server_config_t server_config;                 /*����������*/
    upload_delay_time_t upload_delay_time;          /*�ϴ�ʱ������ */
} device_config_t;

/*������EEPROM�е�λ��*/

/*����eeprom�е��׵�ַ*/
#define BASE_ADDR                       (0x10)
/*�豸�汾��Ϣ�׵�ַ������*/
#define DEVICE_VER_INFO_ADDR            (BASE_ADDR)
#define DEVICE_VER_INFO_LENGTH          (sizeof(device_ver_t) / sizeof(uint8_t))
/*�������������׵�ַ������*/
#define NET_CONFIG_INFO_ADDR            (BASE_ADDR + DEVICE_VER_INFO_LENGTH)
#define NET_CONFIG_INFO_LENGTH          (sizeof(net_config_t) / sizeof(uint8_t))
/*����������׵�ַ������*/  
#define SERVER_CONFIG_INFO_ADDR         (NET_CONFIG_INFO_ADDR + NET_CONFIG_INFO_LENGTH)
#define SERVER_CONFIG_INFO_LENGTH       (sizeof(server_config_t) / sizeof(uint8_t))
/*�ϴ�ʱ���������׵�ַ������*/
#define UPLOAD_DELAY_TIME_INFO_ADDR     (SERVER_CONFIG_INFO_ADDR + SERVER_CONFIG_INFO_LENGTH)
#define UPLOAD_DELAY_TIME_INFO_LENGTH   (sizeof(upload_delay_time_t) / sizeof(uint8_t))
//�û��Զ���ͨ��ͷ�ļ�
//#define NDEBUG


#endif


