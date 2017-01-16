/*-----------------------------------------------------------------------*/
/* ����ƽ̨�õ������ݽ�����֮������ӻ������ɼ���������                    */
/* �ӻ�01 �����豸1 �ӻ�02 �����豸2  �ӻ��豸3 �����豸3 �ӻ��豸4 �����豸4*/
/* ----------------------------------------------------------------------*/
/* ��ʽ���£�                                                             */
/* �ӻ���ַ ͷ֡  ���� ָ��   ָ��    ָ��     ָ��    ָ��   CRC-8         */
/*  00 01  55 AA      00 01  00 02  00 03     00 04  00 05  HEX          */
#include "msg_lower_set.h"
#include "sbus.h"
#include "comm_struct.h"
#include "stdint.h"
#include "usart.h"

#define set_msg_lower(c)    USART1_Putc((c))
/*���� �������豸��������*/
void msg_set_sensor_limit_info(void)
{
        sensor_limit_t sensor_limit;   /*����������������*/
        static uint8_t roll_flag = 0;  /*ѭ�����ʹ�������ֵ*/
        uint16_t sensor_limit_val_region[12];
        /*�ӵ�ǰ�����л�ȡ����*/
        sbus_get(SENSOR_LIMIT_INFO_ADDR, SENSOR_LIMIT_INFO_LENGTH, (uint16_t *)(&sensor_limit));
        /*�����¶� ������*/
        sensor_limit_val_region[0] = sensor_limit.air_t_h;
        sensor_limit_val_region[1] = sensor_limit.air_t_l;
        /*����ʪ�� ������*/
        sensor_limit_val_region[2] = sensor_limit.air_h_h;
        sensor_limit_val_region[3] = sensor_limit.air_h_l;
        /*�����¶� ������*/
        sensor_limit_val_region[4] = sensor_limit.soil_t_h;
        sensor_limit_val_region[5] = sensor_limit.soil_t_l;
        /*����ʪ�� ������*/
        sensor_limit_val_region[6] = sensor_limit.soil_h_h;
        sensor_limit_val_region[7] = sensor_limit.soil_h_l;
        /*������̼    ������*/
        sensor_limit_val_region[8] = sensor_limit.air_co2_h;
        sensor_limit_val_region[9] = sensor_limit.air_co2_l;
        /*���� ������*/
        sensor_limit_val_region[10] = sensor_limit.air_l_h;
        sensor_limit_val_region[11] = sensor_limit.air_l_l;
        
        /*�����ݷ��͵�����----->�����豸*/
        set_msg_lower(0x00);
        set_msg_lower(0x03);   /*�豸��ַ�������zigbee����ģʽ*/
        set_msg_lower(0x55);
        set_msg_lower(0xAA);
        set_msg_lower(0x01);   /*�� 0x01 ��ʾ�����������*/
        set_msg_lower(0x05);
        /*δ����У��*/
        /*ͨ����ѯ������ÿ�η��͵ĳ��ȣ���6�δ���*/
        switch(roll_flag)
        {
            case 0:
                /*�����¶�������*/
                set_msg_lower(0x01);
                set_msg_lower((int8_t)(sensor_limit_val_region[0] >> 8));
                set_msg_lower((int8_t)(sensor_limit_val_region[0] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[1] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[1] & 0x00ff));
                
                roll_flag = 1;
                break;
            case 1:
                /*����ʪ��������*/
                set_msg_lower(0x02);
                set_msg_lower((uint8_t)(sensor_limit_val_region[2] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[2] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[3] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[3] & 0x00ff));
            
                roll_flag = 2;
                break;
            case 2:
                /*�����¶�������*/
                set_msg_lower(0x03);
                set_msg_lower((int8_t)(sensor_limit_val_region[4] >> 8));
                set_msg_lower((int8_t)(sensor_limit_val_region[4] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[5] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[5] & 0x00ff));
                roll_flag = 3;
                break;
            case 3:
                /*����ʪ��������*/
                set_msg_lower(0x04);
                set_msg_lower((uint8_t)(sensor_limit_val_region[6] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[6] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[7] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[7] & 0x00ff));
                roll_flag = 4;
                break;
            case 4:
                /*������̼Ũ��������*/
                set_msg_lower(0x05);
                set_msg_lower((uint8_t)(sensor_limit_val_region[8] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[8] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[9] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[9] & 0x00ff));
                roll_flag = 5;
                break;
            case 5:
                /*����������*/
                set_msg_lower(0x06);
                set_msg_lower((uint8_t)(sensor_limit_val_region[10] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[10] & 0x00ff));
                set_msg_lower((uint8_t)(sensor_limit_val_region[11] >> 8));
                set_msg_lower((uint8_t)(sensor_limit_val_region[11] & 0x00ff));
                roll_flag = 0;
                break;
            default:
                roll_flag = 0;
                break;
            
        }
   
}
/*�������ָ��*/
void msg_set_ctr_info(void)
{
        ctr_cmd_t ctr_cmd;
        uint16_t ctr_val_region[4];
        sbus_get(CTR_CMD_INFO_ADDR, CTR_CMD_INFO_LENGTH, (uint16_t *)(&ctr_cmd));
        
        /*��� ��Ĥ ���� �����*/
        ctr_val_region[0] = (uint8_t)(ctr_cmd.water_ctr & 0x00ff);
        ctr_val_region[1] = (uint8_t)(ctr_cmd.roll_ctr & 0x00ff);
        ctr_val_region[2] = (uint8_t)(ctr_cmd.shutter_ctr & 0x00ff);
        ctr_val_region[3] = (uint8_t)(ctr_cmd.light_ctr & 0x00ff);
        /*����ָ��----->�����豸*/
        set_msg_lower(0x00);
        set_msg_lower(0x03);   /*�豸��ַ�������zigbee����ģʽ*/
        set_msg_lower(0x55);
        set_msg_lower(0xAA);
        //set_msg_lower(0x02);   /*��0x02��ʾ�������ָ��*/
        set_msg_lower(0x04);
        set_msg_lower(ctr_val_region[0]);  /*���*/
        set_msg_lower(ctr_val_region[1]);  /*��Ĥ*/
        set_msg_lower(ctr_val_region[2]);  /*����*/
        set_msg_lower(ctr_val_region[3]);  /*�����*/   
        /*δ����У��*/
}
