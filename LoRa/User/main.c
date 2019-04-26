#include "stm8l15x.h"
#include "sx1278/sx1278.h"
#include "LoRa/LoRa.h"

uint8_t Send_Data[12] = {0xff,0xff,0xff,0xff,0xff,0x01,0x01,0x00,0x00,0x00,0x00,0xae};
uint8_t data1,data,rdata,tmp;
unsigned char FreqTbl[1][3] = 
{ 
  {0x6C, 0x80, 0x00}, //434MHz
};

uint8_t *Status[] = {"Open", "Close"};

int main(void)
{
  CLK_HSICmd(ENABLE);//开始内部高频RC
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//HSI为系统时钟
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_4);//系统时钟设置
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,ENABLE);//开启SPI时钟门控
  CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//开启USART时钟
  //GPIO初始化
  GPIO_Init(GPIOB,GPIO_Pin_2,GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);  
  
  
  SPIInit();//LoRa初始化
  
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);  
  
  USART_Init(USART1,9600,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx|USART_Mode_Rx);//设置USART参数9600，8N1，接收/发送
  USART_ITConfig (USART1,USART_IT_RXNE,ENABLE);//使能接收中断
  USART_Cmd (USART1,ENABLE);//使能USART
  enableInterrupts(); 
  
  GPIO_WriteBit(GPIOB,GPIO_Pin_2,0);
  
//    USART_SendData8(USART1,0x00);
//    SPI_CS_Low;
//    while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);
//    SPI_SendData(SPI1,RegVersion);
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    data1 = SPI_ReceiveData(SPI1);
    
//    while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);
//    SPI_SendData(SPI1,0xFF);
//    while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
//    data = SPI_ReceiveData(SPI1);
    
//    data = SPIRead(RegVersion);
//    SPI_CS_High;
  
    while(0)
    {
      data = SPIRead(RegVersion);
    }
    USART_SendData8(USART1,data);
    rdata = 0x00;
    while(0)
    {
      SPIWrite(LR_RegOcp,0x0B); 
      data = SPIRead(LR_RegOcp);
    }
    
    while(0)
    {
      SPIWrite(LR_RegLna,0xE0);
      data = SPIRead(LR_RegLna); 
      USART_SendData8(USART1,data);
      Delay(5000);
    }
    
  while(0)
  {
    BurstWrite(LR_RegFrMsb,FreqTbl[0],3);  
    data = SPIRead(LR_RegFrMsb);
    data = SPIRead(LR_RegFrMsb+1);
    data = SPIRead(LR_RegFrMsb+2);
  }
    
    while(sx1276_7_8_Config() == 0);
    
    sx1276_7_8_Standby();
    Delay(5000);
    GPIO_WriteBit(GPIOB,GPIO_Pin_2,1);
//TX
    while(1)
    {
//      GPIO_WriteBit(GPIOB,GPIO_Pin_2,1);
     sx1276_7_8_LoRaTxPacket(5, Status[1]);
//      GPIO_WriteBit(GPIOB,GPIO_Pin_2,0);
//      Delay(100000);
//      GPIO_WriteBit(GPIOB,GPIO_Pin_2,1); 
    }
//RX    
//    sx1276_7_8_LoRaEntryRx();
    while(0)
    {
      GPIO_WriteBit(GPIOB,GPIO_Pin_2,0);
      Delay(50000);
      if(sx1276_7_8_LoRaRxPacket())		
      {
      }
      GPIO_WriteBit(GPIOB,GPIO_Pin_2,1);
      Delay(50000);
        
    }
    
}