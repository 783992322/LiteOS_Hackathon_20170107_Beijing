#ifndef COMM_STRUCT_H_
#define COMM_STRUCT_H_
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*����������family*/
typedef struct _air_sensor_info_t {
    int16_t air_t;      /*�����¶�*/
    uint16_t air_h;     /*����ʪ��*/
    uint16_t air_l;     /*����*/
    uint16_t air_c;     /*������̼*/
    uint16_t air_wd;    /*����*/
    uint16_t air_ws;    /*����*/
    uint16_t air_s1;    /*����*/
    uint16_t air_s2;    /*����*/
    uint16_t air_s3;    /*����*/
    uint16_t air_s4;    /*����*/
} air_sensor_t;
/*����������family*/
typedef struct _soil_sensor_info_t{
    int16_t soil_t;     /*�����¶�*/
    uint16_t soil_h5;   /*5cm����ʪ��*/
    uint16_t soil_h10;  /*10cm����ʪ��*/
    uint16_t soil_h15;  /*15cm����ʪ��*/
} soil_sensor_t;

/*ˮ�ʴ�����family*/
typedef struct _water_sensor_info_t{
     uint16_t water_l;   /*ˮλ*/
     uint16_t water_p;   /*PH*/
     uint16_t water_e;   /*�絼��*/
     uint16_t water_o;   /*�ܽ���*/
     uint16_t water_d;   /*�Ƕ�*/
} water_sensor_t;


/* �գ�ռλ      #1     */
/* 0                    */
/* �����������׵�ַ������ */
#define AIR_SENSOR_INFO_ADDR            (0x00)
#define AIR_SENSOR_INFO_LENGTH          (sizeof(air_sensor_t) / sizeof(uint16_t))
/* �����������׵�ַ������ */
#define SOIL_SENSOR_INFO_ADDR           (0x0B)
#define SOIL_SENSOR_INFO_LENGTH         (sizeof(soil_sensor_t) / sizeof(uint16_t))
/* ˮ�ʴ������׵�ַ������ */
#define WATER_SENSOR_INFO_ADDR          (0x0F)
#define WATER_SENSOR_INFO_LENGTH        (sizeof(water_sensor_t) / sizeof(uint16_t))
#ifdef	__cplusplus
}
#endif

#endif // COMM_STRUCT_H_
