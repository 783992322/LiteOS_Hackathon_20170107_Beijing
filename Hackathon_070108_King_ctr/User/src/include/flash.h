#ifndef __STMFLASH_H__
#define __STMFLASH_H__  
#include "stm32f10x.h"
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
//FLASH��ʼд���ַ
#define FLASH_START_ADDR  0x08030000
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
/*flashд��*/
void flash_write_nbytes(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);
/*flash����*/
void flash_read_nbytes(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);
//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);	

//ʹ�÷���
//flash_read_nbytes(FLASH_START_ADDR, (uint16_t*)read_buf, sizeof(TEXT_Buffer));
#endif

















