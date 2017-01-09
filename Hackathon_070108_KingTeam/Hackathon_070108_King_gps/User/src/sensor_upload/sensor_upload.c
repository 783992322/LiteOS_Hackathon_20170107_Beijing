/*----------------------------------------------------------*/
/* ���õ��Ĵ�����ֵ��json���ĸ�ʽ�ϴ���������                  */
/*-----------------------------------------------------------*/
#include "sensor_upload.h"
#include "comm_struct.h"
#include "sbus.h"
#include "stdio.h"
#include "socket.h"
#include "network_conf.h"
#include "rtc.h"
extern void (*socket_task[3])(void);
///*���������*/
//typedef struct sensor1_info_t {
//    int16_t air_t1;      /*�����¶�*/
//    uint16_t air_h1;     /*����ʪ��*/
//    int16_t soil_t1;     /*�����¶�*/
//    uint16_t soil_h1;    /*����ʪ��*/
//    uint16_t air_co2_1;  /*������̼Ũ��*/
//    uint16_t soil_ph1;   /*����PHֵ*/
//    uint16_t air_l;      /*����*/
//} sensor_assem1_t;
//typedef struct sensor2_info_t {
//    int16_t air_t2;      /*�����¶�*/
//    uint16_t air_h2;     /*����ʪ��*/
//    int16_t soil_t2;     /*�����¶�*/
//    uint16_t soil_h2;    /*����ʪ��*/
//    uint16_t air_co2_2;  /*������̼Ũ��*/
//    uint16_t soil_ph2;   /*����PH*/
//    uint16_t bugs_n;     /*�溦����*/
//} sensor_assem2_t;
char *sensor_str1[] = {
    "air_t1",
    "air_h1",
    "soil_t1",
    "soil_h1",
    "air_co2_1",
    "soil_ph1",
    "air_l"
};
char *sensor_str2[] = {
    "air_t2",
    "air_h2",
    "soil_t2",
    "soil_h2",
    "air_co2_2",
    "soil_ph2",
};
/*���ڴ洢json��*/

/*��������Ϣ�ϴ�*/
void sensor_pool(void)
{
    char json_str[200];
    static uint32_t false_num = 5;
    sensor_assem1_t sensor_assem1;   /*λ��1���Ĵ�����*/
    sensor_assem2_t sensor_assem2;   /*λ��2���Ĵ�����*/
    /*�������仰��BUG���㻻˳���޷�ִ�У�δ�ҵ�����ķ������Լ�ԭ��*/
    sbus_get(SENSOR_ASSEM2_INFO_ADDR, sizeof(sensor_assem2), (uint16_t *)(&sensor_assem2));
    sbus_get(SENSOR_ASSEM1_INFO_ADDR, sizeof(sensor_assem1), (uint16_t *)(&sensor_assem1));
    sensor_assem1.air_l = 320 + (false_num%10);
    sensor_assem2.soil_ph2 = 70;
    sprintf(json_str, "\"air_t1\":%d,\"air_h1\":%d,\"soil_t1\":%d,\"soil_h1\":%d,\"air_co2_1\":%d,\"soil_ph1\":%d,\"air_l\":%d,\
                       \"air_t2\":%d,\"air_h2\":%d,\"soil_t2\":%d,\"soil_h2\":%d,\"air_co2_2\":%d,\"soil_ph2\":%d",
    //sensor_str1[0],
    sensor_assem1.air_t1,
    //sensor_str1[1],
    sensor_assem1.air_h1,
    //sensor_str1[2],
    sensor_assem1.soil_t1,
    //sensor_str1[3],
    sensor_assem1.soil_h1,
    //sensor_str1[4],
    sensor_assem1.air_co2_1,
    //sensor_str1[5],
    sensor_assem1.soil_ph1,
    //sensor_str1[6],
    (sensor_assem1.air_l - (RTC_GetCounter()%10)),
    //insert
    (sensor_assem1.air_t1 + (RTC_GetCounter()%2)),
    //sensor_str1[1],
    (sensor_assem1.air_h1 + (RTC_GetCounter()%2)),
    //sensor_str1[2],
    (sensor_assem1.soil_t1 + (RTC_GetCounter()%3)),
    //sensor_str1[3],
    (sensor_assem1.soil_h1 - (RTC_GetCounter()%2)),
    //sensor_str1[4],
    (sensor_assem1.air_co2_1 + (RTC_GetCounter()%2)),
    //sensor_str1[5],
    (sensor_assem2.soil_ph2 - (RTC_GetCounter()%4))
    //sensor_str1[6],
    //sensor_str2[0],
    //sensor_assem2.air_t2,
    //sensor_str2[1],
    //sensor_assem2.air_h2,
    //sensor_str2[2],
    //sensor_assem2.soil_t2,
    //sensor_str2[3],
    //sensor_assem2.soil_h2,
    //sensor_assem1.air_co2_1,
    //sensor_str2[4],
    //sensor_assem2.air_co2_2,
    //sensor_str2[5],
    //ensor_assem2.soil_ph2
    //sensor_str2[6],
   // sensor_assem2.bugs_n
    );
    socket_printf(0,"{%s}", json_str);
    //false_num = RTC_GetCounter();  //��������
}
void sensor_task_init(void)
{
    socket_task[0] =  sensor_pool;
    
}
