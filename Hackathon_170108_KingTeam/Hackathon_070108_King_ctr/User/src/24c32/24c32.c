/*-------------------------------------------------*/
/* ����ģ���I2Cͨ��Э���дeeprom 24c32            */
/*-------------------------------------------------*/
#include "24c32.h"
#include "ult.h"
#include "config.h"
#include "xassert.h"
/* eerrom at24c32��ʼ�� */
void at24c32_init(void)
{
      GPIO_InitTypeDef GPIO_InitStructure;
      //Port B bidirection SDA
      GPIO_InitStructure.GPIO_Pin = I2C_SDA;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
      GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);
        
      //Port B output
      GPIO_InitStructure.GPIO_Pin = I2C_SCK;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_Init(EEPROM_PORT, &GPIO_InitStructure);
}
/*������ʼ�ź�*/
void at24c32_start(void)
{
      GPIO_SetBits(EEPROM_PORT, I2C_SDA);
      GPIO_SetBits(EEPROM_PORT, I2C_SCK);
      delay_us(1);
      GPIO_ResetBits(EEPROM_PORT, I2C_SDA);
      GPIO_ResetBits(EEPROM_PORT, I2C_SCK);
}
/*����ֹͣ�ź�*/
void at24c32_stop(void)
{
      GPIO_ResetBits(EEPROM_PORT, I2C_SDA);
      GPIO_SetBits(EEPROM_PORT, I2C_SCK);
      delay_us(1);
      GPIO_SetBits(EEPROM_PORT, I2C_SDA);
}
/*д��һ���ֽ�*/
void write_1byte(unsigned char val)
{
      unsigned char i, tmp;
      tmp=val;
      
      for (i=0;i<8;i++)
      {
            GPIO_ResetBits(EEPROM_PORT, I2C_SCK);
            delay_us(1);
            if (((tmp<<i) & 0x80)==0x80)
                GPIO_SetBits(EEPROM_PORT, I2C_SDA);
            else
                GPIO_ResetBits(EEPROM_PORT, I2C_SDA);
            delay_us(1);
            GPIO_SetBits(EEPROM_PORT, I2C_SCK);
            delay_us(1);
      }
      GPIO_ResetBits(EEPROM_PORT, I2C_SCK);
      delay_us(1);
      GPIO_SetBits(EEPROM_PORT, I2C_SDA);
      delay_us(1);
}

/*����һ���ֽ�*/
unsigned char read_1byte(void)
{
      unsigned char i,j,k=0;
      
      GPIO_ResetBits(EEPROM_PORT, I2C_SCK);delay_us(1);GPIO_SetBits(EEPROM_PORT, I2C_SDA);
      for (i=0;i<8;i++)
      {
            delay_us(1);
            GPIO_SetBits(EEPROM_PORT, I2C_SCK);
            delay_us(1);
            
            if(GPIO_ReadInputDataBit(EEPROM_PORT, I2C_SDA)==1) j=1; //???SDA becoms inout pin from output pin;
            else j=0;
            k=(k<<1) | j; 
            GPIO_ResetBits(EEPROM_PORT, I2C_SCK);
      }
      delay_us(1);
      return (k);
}
/*ʱ��*/
void clock(void)
{
      uint16 i=0;
      
      GPIO_SetBits(EEPROM_PORT, I2C_SCK);
      delay_us(1);
      
      while((GPIO_ReadInputDataBit(EEPROM_PORT, I2C_SDA)==1) && (i<255))
            i++;
      GPIO_ResetBits(EEPROM_PORT, I2C_SCK);
      delay_us(1);
}
/*��addr��ַЭ��һ���ֽڵ�val*/
void at24c32_write(uint16 addr, uint8 val)
{
      uint8 l_addr;

      __disable_irq();   /*��ֹ�ж�*/
      at24c32_start();
      //write_1byte(0xa0);
      //clock();
      //uint8 h_addr=addr>>8;
      //write_1byte(h_addr);
      //write_1byte(0xa0 | ((addr>>7 & 0xfe)));
      write_1byte(0xa0);  
      clock();
      write_1byte(addr >> 8);  
      clock();
      l_addr=(addr%256);
      write_1byte(l_addr);
      clock();
      write_1byte(val);
      clock();
      at24c32_stop();
      __enable_irq();
      delay_ms(5);
}
/* ��eeprom��ַ�ж�ȡһ���ֽڵ����� */
unsigned char at24c32_read(uint16 addr)
{
      //uint8 high,low;
      uint8 i;
      uint8 low;
      low = addr & 0x00ff;
      //high=(addr & 0xff00)>>8;

      __disable_irq();
      at24c32_start();
      //write_1byte(0xa0);
      //clock();
      //write_1byte(high);
      //write_1byte(0xa0 | ((addr>>7 & 0xfe)));
      write_1byte(0xa0);  
      clock();
      write_1byte(addr >> 8);  
      clock();
      write_1byte(low);
      clock();
      at24c32_start();
      write_1byte(0xa1);
      clock();
      i=read_1byte();
      at24c32_stop();
      //Delay_us(5);
      __enable_irq();
      return(i);
}
//eep block write
//eepAddr: eeprom start address
//dat: data array to be saved to eeprom
//index: data array start index
//len: how long to be write
/* ��eeprom�д�addr��ַ��ʼд�볤��Ϊlen���ֽ�����*/
void eeprom_block_write(uint16_t addr, uint16_t len, const uint8 *buf)
{
        /* ��ַ�����Ⱦ�����Ϊ0 */
        xassert(addr != 0);
        xassert(len  != 0);

        do
                at24c32_write(addr++, *buf++);
        while(--len);
}
/*eeprom�ж������ݴ��뻺����buf��*/
void eeprom_block_read(uint16_t addr, uint16_t len, uint8_t *buf)
{
        /* ��ַ�����Ⱦ�����Ϊ0 */
        xassert(addr != 0);
        xassert(len  != 0);

        do
            *buf++ = at24c32_read(addr++);       
        while(--len);
}
/* ����eeprom�е����ݣ���дΪ0xff */
void erase_eeprom(uint16 startAddr, uint16 len)
{	  
      uint16 i;
    for(i=startAddr;i<startAddr+len;i++)
  {
    at24c32_write(i,0xff);
  }
}
