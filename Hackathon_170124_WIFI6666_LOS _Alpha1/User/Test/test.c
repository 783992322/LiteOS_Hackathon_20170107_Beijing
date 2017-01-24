#include "test.h"
#include "bsp_esp8266.h"
//#include "bsp_SysTick.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>
#include "bsp_dht11.h"

#include "bsp_key.h" 
#include "bsp_hall.h" 
#include "bsp_led.h" 
volatile uint8_t ucTcpClosedFlag = 0;



/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	int roll_count =0;
	
	DHT11_Data_TypeDef DHT11_Data;
	
//  printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( STA );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n���� ESP8266 ���\r\n" );
	
	LED2_ON;
	
	while ( 1 )
	{
		if( HALL_Scan(HALL1_GPIO_PORT,HALL1_GPIO_PIN) == HALL_ON  )
		{//��⵽����
			roll_count ++;
			/*LED2��ת*/
			LED2_TOGGLE;
			/*��������*/
			//ESP8266_SendString ( ENABLE, "HEAD / HTTP/1.1\r\nConnection: Close\r\n\r\n", 0, Single_ID_0 );   
			if ( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS )       //��ȡ DHT11 ��ʪ����Ϣ
				sprintf ( cStr, "%d.%d,%d.%d,%d,%s\n", 
									DHT11_Data .humi_int, DHT11_Data .humi_deci, DHT11_Data .temp_int, DHT11_Data.temp_deci, roll_count , "A000001");
			else
				printf ( cStr, "%d.%d,%d.%d,%d,%s\n", 
									0, 0, 0, roll_count, "A000001");
			printf ( "%s", cStr );                                             //��ӡ��ȡJSON��

			ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );               //����JSON��
			
//			LOS_TaskDelay(100000);
			
			if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
			{
				LED1_TOGGLE;
				ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
				
				do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
				while ( ! ucStatus );
				
				if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
				{
					printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
					
					while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
					
					while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
					
					printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );

				}
				
				while ( ! ESP8266_UnvarnishSend () );		
				
			}
		}
	}
	
		
}


