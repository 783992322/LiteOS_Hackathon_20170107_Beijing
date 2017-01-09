/*------------------------------------------------------------------------/
/  W5500�������ó�ʼ���ӿں���
/-------------------------------------------------------------------------/
/
/  Copyright (C) 2015, Zg, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------*/
#include "mcu_init.h"
#include "stm32f10x.h"
#include "ult.h"
#include <stdio.h>
#include "usart.h"
#include "usart3.h"
#include "xassert.h"
#include "config.h"
#include "network_conf.h"
#include "socket.h"
#include "w5500.h"
#include "md5.h"
#include "spi2.h"
#include "sensor_upload.h"
#include "stdlib.h"
#include "dns.h"
//extern uint8 txsize[];
//extern uint8 rxsize[];
//extern char json_pack[64]; /*json���ݰ�������json_parser*/
char *json_pack;
uint8_t recv_complete_flag = 0;
uint16_t socket_recv_len = 0;   /*socket�������ݵĳ���*/
CONFIG_MSG  ConfigMsg, RecvMsg;
server_config_t server_config;  /*���������*/
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint16_t anyport = 6000;
uint16_t local_port[3] = { 54581, 54582, 54583};  /*���ؿ��Ŷ˿�*/
void (*socket_task[3])(void);
void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOB, WIZ_RESET);
  delay_us(2);  
  GPIO_SetBits(GPIOB, WIZ_RESET);
  delay_ms(1600);
}
/*---------------------------------------------*/
/* socket ѭ���أ����ա���������                 */
/* SOCKET0, 192.168.1.120, 2015                */
/*---------------------------------------------*/
void socket_poll(SOCKET s, uint8_t *server_ip, uint16_t server_port)
{
    //uint8_t dns_retry_cnt=0;
    //uint8_t dns_ok=0;
        switch(getSn_SR(s))       /*��ȡsocketx��״̬*/
         {
               case SOCK_INIT:    /*socket��ʼ�����*/
                    connect(s, server_ip ,server_port);/*��TCPģʽ���������������������*/
                    break;
               case SOCK_ESTABLISHED:/*socket���ӽ���*/
                    if(getSn_IR(s) & Sn_IR_CON)
                    {
                        setSn_IR(s, Sn_IR_CON);/*Sn_IR�ĵ�0λ��1*/
                    }
                    if(s == SOCKET0)   /*socket0 ���д�����ֵ����*/
                    {
                        socket_task[0]();
                        
                    }
                    
                    if(s == SOCKET1)
                    {
                        socket_recv_len = getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
                        /*���յ����ݣ�����������ݱ�������������´ν���*/
                        if(socket_recv_len > 0 && (recv_complete_flag == 0))
                        {
                            json_pack = (char *)malloc(sizeof(char)*(socket_recv_len+1));  /* ������һ���ֽڣ������洢'\0'*/
                            recv(s, (uint8_t *)json_pack, socket_recv_len); 
                            json_pack[socket_recv_len] = '\0';
                            /*�����ݷ��أ���֪�յ����ݣ����ﲻ����Ӧ���ڽ���������֮�󷵻�*/
                            send(s, (uint8_t *)json_pack, socket_recv_len);/*just for test*/
                            recv_complete_flag = 1;
                        }
                    }
                    /*W5500��Server���Ͳ�������*/
                    /*�������ݣ����Է������ݡ�Ҳ���Խ�������*/
                    break;
               case SOCK_CLOSE_WAIT:/*socket�ȴ��ر�״̬*/
                    disconnect(s);
                    break;
               case SOCK_CLOSED:    /*socket�ر�*/
                    socket(s,Sn_MR_TCP,anyport++,Sn_MR_ND);/*��socket0��һ���˿�*/
                     break;
         }
  
}
/*-------------------------------------------------------*/
/* �����������ã�mac��ip���������롢����                   */
/*-------------------------------------------------------*/
void network_init(net_config_t local_net_config)
{
      uint8 ip[4];
      //net_config_t local_net_config;                /*���屾����������*/
      //uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11}; /*����Mac����*/
      //uint8 lip[4]={192,168,1,110};                 /*����lp����*/
      //uint8 sub[4]={255,255,255,0};                 /*����subnet����*/
      //uint8 gw[4]={192,168,1,1};                    /*����gateway����*/
      uint8 dns[4]={8,8,8,8};  
      Reset_W5500();            /*Ӳ����W5500*/
      WIZ_SPI_Init();           /*��ʼ��SPI�ӿ�*/ 
      setSHAR(local_net_config.mac);             /*����Mac��ַ*/
      setSUBR(local_net_config.sub);             /*������������*/
      setGAR(local_net_config.gw);               /*����Ĭ������*/
      setSIPR(local_net_config.lip);             /*����Ip��ַ*/
        //Init. TX & RX Memory size of w5500
      sysinit(txsize, rxsize); /*��ʼ��8��socket*/
      setRTR(2000);             /*�������ʱ��ֵ*/
      setRCR(3);                /*����������·��ʹ���*/
      getSIPR (ip);
      getSUBR(ip);
      getGAR(ip);
      memcpy(ConfigMsg.dns,dns,4);  /*�����DNS����*/

      sprintf((char*)ConfigMsg.domain,"%s", "m.zhuogan.com.cn"); 
      
}

/*����������*/
void setkeepalive(SOCKET s)

{

    //IINCHIP_WRITE(Sn_KPALVTR(s), KEEP_ALIVE_TIME);  /*30s*/
}
