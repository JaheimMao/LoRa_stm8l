#ifndef _SX1278_H_
#define _SX1278_H_

#define SPI_CS_PORT             GPIOB
#define SPI_CS_PIN              GPIO_Pin_4
/* 使能SPI外部设备，拉低CS引脚电平 */
#define SPI_CS_Low              GPIO_ResetBits(SPI_CS_PORT , SPI_CS_PIN)
/* 禁止SPI外部设备，拉高CS引脚电平 */
#define SPI_CS_High             GPIO_SetBits(SPI_CS_PORT, SPI_CS_PIN)

extern void SPIInit();
extern unsigned char SPICmd8bit(unsigned char WrPara);
extern unsigned char SPIRead(unsigned char adr);
extern void SPIWrite(unsigned char adr, unsigned char WrPara);
extern void SPIBurstRead(unsigned char adr, unsigned char *ptr, unsigned char length);
extern void BurstWrite(unsigned char adr, unsigned char *ptr, unsigned char length);


#endif