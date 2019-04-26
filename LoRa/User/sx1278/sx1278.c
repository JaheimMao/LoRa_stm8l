#include "stm8l15x.h"
#include "sx1278.h"


/**********************************************************
**Name:     SPIInit
**Function: SPI Initiation
**Input:    none
**Output:   none
**note:     
**********************************************************/

extern unsigned char data;

void SPIInit()
{
/* 初始化SPI */
  SPI_DeInit(SPI1);
  SPI_Init(SPI1,SPI_FirstBit_MSB, SPI_BaudRatePrescaler_64, SPI_Mode_Master,\
              SPI_CPOL_Low, SPI_CPHA_1Edge, \
              SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
  SPI_Cmd(SPI1,ENABLE);	/* 使能SPI */
/* 配置CS管脚 */
  GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_Mode_Out_PP_High_Slow);
  SPI_CS_High;		/* 不使能外部SPI设备 */
}

/**********************************************************
**Name:     SPICmd8bit
**Function: SPI Write one byte
**Input:    WrPara
**Output:   none
**note:     use for burst mode
**********************************************************/
unsigned char SPICmd8bit(unsigned char WrPara)
{ 
  unsigned char RdPara;
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);
  SPI_SendData(SPI1,WrPara);
  while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
  RdPara=SPI_ReceiveData(SPI1);
  return RdPara;
}

/**********************************************************
**Name:     SPIRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
unsigned char SPIRead(unsigned char adr)
{
  unsigned char data;
  SPI_CS_Low;
  data = SPICmd8bit(adr);              //Send address first
  data = SPICmd8bit(0xFF);  
  SPI_CS_High;
  return data;
}

/**********************************************************
**Name:     SPIWrite
**Function: SPI Write CMD
**Input:    unsigned char address & unsigned char data
**Output:   None
**********************************************************/
void SPIWrite(unsigned char adr, unsigned char WrPara)  
{
  unsigned char tmp,data;
  SPI_CS_Low;						
  data = SPICmd8bit(adr|0x80);		//写入寄存器地址
  data = SPICmd8bit(WrPara);           //写入数据
  SPI_CS_High;
}

/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead(unsigned char adr, unsigned char *ptr, unsigned char length)
{
  unsigned char i;
  unsigned char tmp,data;
  
  SPI_CS_Low;
  data = SPICmd8bit(adr&0x7F);		//写入寄存器地址
  if(length<=1)                                            //length must more than one
    return;
  else
  {
    for(i=0;i<length;i++)
    {
      ptr[i] = SPICmd8bit(0x00);
    }
    SPI_CS_High;
  }
}

/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite(unsigned char adr, unsigned char *ptr, unsigned char length)
{ 
  unsigned char i;
  unsigned char tmp,data;

  SPI_CS_Low;						
  data = SPICmd8bit(adr|0x80);		//写入寄存器地址
  if(length<=1)                                            //length must more than one
    return;
  else  
  {
    for(i=0;i<length;i++)
    {
      data = SPICmd8bit(ptr[i]);
    }
    SPI_CS_High;
  }
}