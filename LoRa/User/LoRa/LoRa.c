#include "stm8l15x.h"
#include "./sx1278/sx1278.h"
#include "LoRa.h"

unsigned int  SysTime = 0;
unsigned char mode = 0x01;//lora--1/FSK--0
unsigned char Freq_Sel = 0x00;//
unsigned char Power_Sel = 0x03;//
unsigned char Lora_Rate_Sel = 0x02;//0--fastest
unsigned char BandWide_Sel = 0x07;//
unsigned char Fsk_Rate_Sel = 0x00;//
unsigned char RxData[64];
extern unsigned char data;


/**********************************************
Function: Get sx1278 Interrupt state
Input:    none
Output:   none
**********************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

unsigned char Get_SX1278_IRQ_Status(void)
{
	unsigned char s = 0;
	
	s = SPIRead(LR_RegIrqFlags);
	return s;
}


/**********************************************************
**Parameter table define
**********************************************************/
unsigned char sx1276_7_8FreqTbl[1][3] = 
{ 
  {0x6C, 0x80, 0x00}, //434MHz
};

const unsigned char sx1276_7_8PowerTbl[4] =
{ 
  0xFF,                   //20dbm  
  0xFC,                   //17dbm
  0xF9,                   //14dbm
  0xF6,                   //11dbm 
};

const unsigned char sx1276_7_8SpreadFactorTbl[7] =
{
  6,7,8,9,10,11,12
};

const unsigned char sx1276_7_8LoRaBwTbl[10] =
{
//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
  0,1,2,3,4,5,6,7,8,9
};

const unsigned char  sx1276_7_8Data[] = {"HR_WT Lora sx1276_7_8"};



/**********************************************************
**Variable define
**********************************************************/


/**********************************************************
**Function: Entry standby mode
**Input:    None
**Output:   None
**********************************************************/
void sx1276_7_8_Standby(void)
{
  SPIWrite(LR_RegOpMode,0x89);                              		//Standby//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x01);                              	 //Standby//High Frequency Mode
  data = SPIRead(LR_RegOpMode);
}

/**********************************************************
**Function: Entry sleep mode
**Input:    None
**Output:   None
**********************************************************/
void sx1276_7_8_Sleep(void)
{
  SPIWrite(LR_RegOpMode,0x08);                          //Sleep//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x00);                  //Sleep//High Frequency Mode
  data = SPIRead(LR_RegOpMode);
}

/*********************************************************/
//LoRa mode
/*********************************************************/
/**********************************************************
**Function: Set RFM69 entry LoRa(LongRange) mode
**Input:    None
**Output:   None
**********************************************************/
void sx1276_7_8_EntryLoRa(void)
{
  SPIWrite(LR_RegOpMode,0x88);//Low Frequency Mode
	//SPIWrite(LR_RegOpMode,0x80);//High Frequency Mode
  data = SPIRead(LR_RegOpMode);
}

/**********************************************************
**Name:     sx1276_7_8_LoRaClearIrq
**Function: Clear all irq
**Input:    None
**Output:   None
**********************************************************/
void sx1276_7_8_LoRaClearIrq(void)
{
  SPIWrite(LR_RegIrqFlags,0xFF);
}

/**********************************************************
**Function: Entry Rx mode
**Input:    None
**Output:   None
**********************************************************/
unsigned char sx1276_7_8_LoRaEntryRx(void)
{
  unsigned char addr; 
        
  sx1276_7_8_Config();                                  //setting base parameter
  
  SPIWrite(REG_LR_PADAC,0x84);                          //Normal and Rx
  SPIWrite(LR_RegHopPeriod,0xFF);                       //RegHopPeriod NO FHSS
  SPIWrite(REG_LR_DIOMAPPING1,0x01);                    //DIO0=00, DIO1=00, DIO2=00, DIO3=01
      
  SPIWrite(LR_RegIrqFlagsMask,0x3F);                    //Open RxDone interrupt & Timeout
  sx1276_7_8_LoRaClearIrq();   
  
  SPIWrite(LR_RegPayloadLength,21);                     //RegPayloadLength  21byte(this register must difine when the data long of one byte in SF is 6)
    
  addr = SPIRead(LR_RegFifoRxBaseAddr);           	//Read RxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                     //RxBaseAddr -> FiFoAddrPtr
  SPIWrite(LR_RegOpMode,0x8d);                        	//Continuous Rx Mode//Low Frequency Mode
  data = SPIRead(LR_RegOpMode);
  //SPIWrite(LR_RegOpMode,0x05);     			//Continuous Rx Mode//High Frequency Mode
  SysTime = 0;
  while(1)
  {
    if((SPIRead(LR_RegModemStat)&0x04)==0x04)           //Rx-on going RegModemStat
      break;
    if(SysTime>=3)	
      return 0;                                        //over time for error
  }
  return 1;
}
/**********************************************************
**Name:     sx1276_7_8_LoRaReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
unsigned char sx1276_7_8_LoRaReadRSSI(void)
{
  uint16_t temp=10;
  temp=SPIRead(LR_RegRssiValue);                        //Read RegRssiValue,Rssi value
  temp=temp+127-137;                                    //127:Max RSSI, 137:RSSI offset
  return (unsigned char)temp;
}

/**********************************************************
**Name:     sx1276_7_8_LoRaRxPacket
**Function: Receive data in LoRa mode
**Input:    None
**Output:   1- Success
            0- Fail
**********************************************************/
unsigned char sx1276_7_8_LoRaRxPacket(void)
{
  unsigned char i; 
  unsigned char addr;
  unsigned char packet_size;
  unsigned char RxData[64];
 
  if( (Get_SX1278_IRQ_Status() & 0x40) == 0x40)
  {
    addr = SPIRead(LR_RegFifoRxCurrentaddr);            //last packet addr
    SPIWrite(LR_RegFifoAddrPtr,addr);                   //RxBaseAddr -> FiFoAddrPtr    
    packet_size = SPIRead(LR_RegRxNbBytes);     //Number for received bytes
    USART_SendData8(USART1,packet_size);    
    SPIBurstRead(0x00, RxData, packet_size);
    for(i=0;i<packet_size;i++)
    {
      USART_SendData8(USART1,RxData[i]);
      Delay(50000);
    }
    sx1276_7_8_LoRaClearIrq();
    return(packet_size);
  }
  else
    return 0;
}

/**********************************************************
**Name:     sx1276_7_8_LoRaEntryTx
**Function: Entry Tx mode
**Input:    None
**Output:   None
**********************************************************/
unsigned char sx1276_7_8_LoRaEntryTx(unsigned char  len)
{
  unsigned char addr,temp;
    
  sx1276_7_8_Config();                                     //setting base parameter
    
  SPIWrite(REG_LR_PADAC,0x87);                             //Tx for 20dBm
  data = SPIRead(REG_LR_PADAC);
  SPIWrite(LR_RegHopPeriod,0x00);                          //RegHopPeriod NO FHSS
  data = SPIRead(LR_RegHopPeriod);
  SPIWrite(REG_LR_DIOMAPPING1,0x41);                       //DIO0=01, DIO1=00, DIO2=00, DIO3=01
  data = SPIRead(REG_LR_DIOMAPPING1);
  
  sx1276_7_8_LoRaClearIrq();
  SPIWrite(LR_RegIrqFlagsMask,0xF7);                       //Open TxDone interrupt
  data = SPIRead(LR_RegIrqFlagsMask);
  SPIWrite(LR_RegPayloadLength,len);                       //RegPayloadLength  21byte
  data = SPIRead(LR_RegPayloadLength);
  
  addr = SPIRead(LR_RegFifoTxBaseAddr);           //RegFiFoTxBaseAddr
  SPIWrite(LR_RegFifoAddrPtr,addr);                        //RegFifoAddrPtr
  SysTime = 0;
  while(1)
  {
    temp=SPIRead(LR_RegPayloadLength);
    if(temp==len)
    {
      break; 
    }
    if(SysTime>=3)	
      return 0;
  }
  return 1;
}
/**********************************************************
**Name:     sx1276_7_8_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
unsigned char sx1276_7_8_LoRaTxPacketbak(void)
{ 
//  unsigned char TxFlag=0;
//  unsigned char addr;
  
	BurstWrite(0x00, (unsigned char *)sx1276_7_8Data, 21);
	SPIWrite(LR_RegOpMode,0x8b);                    //Tx Mode           
	while(1)
	{
		if(Get_SX1278_IRQ_Status() == 0x08)                      //Packet send over
		{      
			SPIRead(LR_RegIrqFlags);
			sx1276_7_8_LoRaClearIrq();                                //Clear irq
				
			sx1276_7_8_Standby();                                     //Entry Standby mode      
			
			break;
		}
	} 
	
	return 1;
}
/**********************************************************
**Name:     sx1276_7_8_LoRaTxPacket
**Function: Send data in LoRa mode
**Input:    None
**Output:   1- Send over
**********************************************************/
unsigned char sx1276_7_8_LoRaTxPacket(unsigned char len , unsigned char *buf)
{ 
//  unsigned char TxFlag=0;
//  unsigned char addr;
  unsigned char i;
  
	sx1276_7_8_LoRaEntryTx(len);
	BurstWrite(0x00, (unsigned char *)buf, len);
	SPIWrite(LR_RegOpMode,0x8b);                    //Tx Mode
        data = SPIRead(LR_RegOpMode);
	
	while(1)
	{
		if(Get_SX1278_IRQ_Status() == 0x08)                      //Packet send over
		{      
			data = SPIRead(LR_RegIrqFlags);
			sx1276_7_8_LoRaClearIrq();                                //Clear irq
				
			sx1276_7_8_Standby();                                     //Entry Standby mode      
			
			break;
		}
	} 
	
	return 1;
}
/**********************************************************
**Name:     sx1276_7_8_ReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
unsigned char sx1276_7_8_ReadRSSI(void)
{
  unsigned char temp=0xff;
	
  temp=SPIRead(0x11);
  temp>>=1;
  temp=127-temp;                                           //127:Max RSSI
  return temp;
}
/**********************************************************
**Name:     sx1276_7_8_Config
**Function: sx1276_7_8 base config
**Input:    mode
**Output:   None
**********************************************************/
unsigned char sx1276_7_8_Config(void)
{
  unsigned char s = 0; 
  //entry sleep mode
  sx1276_7_8_Sleep();                                      
  
  //LORA Mode
  sx1276_7_8_EntryLoRa();  
  
  //32000000*0x6c8000/2^19 = 434M Load Frequency
  BurstWrite(LR_RegFrMsb,sx1276_7_8FreqTbl[0],3); 
  data = SPIRead(LR_RegFrMsb);
  data = SPIRead(LR_RegFrMsb+1);
  data = SPIRead(LR_RegFrMsb+2);
  //11dB Output power
  SPIWrite(LR_RegPaConfig,sx1276_7_8PowerTbl[Power_Sel]);          
  data = SPIRead(LR_RegPaConfig);
  
  //Overcurrent protection off
  SPIWrite(LR_RegOcp,0x0B); 
  data = SPIRead(LR_RegOcp);
  //¡ä?¨°¨¦ LNA¡Á?¡ä¨®??¨°?1?¡À?
  SPIWrite(LR_RegLna,0xE0);
  data = SPIRead(LR_RegLna); 
  
  if(sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]==6)           //SFactor=6
  {
    unsigned char tmp;
    SPIWrite(LR_RegModemConfig1,((sx1276_7_8LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x01));//Implicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
    SPIWrite(LR_RegModemConfig2,((sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]<<4)+(CRC<<2)+0x03));
      
    tmp = SPIRead(0x31);
    tmp &= 0xF8;
    tmp |= 0x05;
    SPIWrite(0x31,tmp);
    data = SPIRead(0x31);
    
    SPIWrite(0x37,0x0C);
    data = SPIRead(0x37);
    
  } 
  else
  {
    //¡ä?¨°¨¦ ¡ä??¨ª?¡ã¡¤¡é¨¦??¨´?¨º CRC?a?? ?¨®¨º???¨°2D¨¨?a??
    SPIWrite(LR_RegModemConfig1,((sx1276_7_8LoRaBwTbl[BandWide_Sel]<<4)+(CR<<1)+0x00));//Explicit Enable CRC Enable(0x02) & Error Coding rate 4/5(0x01), 4/6(0x02), 4/7(0x03), 4/8(0x04)
    data = SPIRead(LR_RegModemConfig1);
    
    SPIWrite(LR_RegModemConfig2,((sx1276_7_8SpreadFactorTbl[Lora_Rate_Sel]<<4)+(CRC<<2)+0x03));  //SFactor &  LNA gain set by the internal AGC loop 
    data = SPIRead(LR_RegModemConfig2);
    
  }
  //RX3?¨º¡À¨º¡À??
  SPIWrite(LR_RegSymbTimeoutLsb,0xFF);                   //RegSymbTimeoutLsb Timeout = 0x3FF(Max) 
  data = SPIRead(LR_RegSymbTimeoutLsb);
  
  //?¡ã¦Ì??? ???D??
  SPIWrite(LR_RegPreambleMsb,0x00);                       //RegPreambleMsb 
  data = SPIRead(LR_RegPreambleMsb);
  
  SPIWrite(LR_RegPreambleLsb,12);                      //RegPreambleLsb 8+4=12byte Preamble
//data = SPIRead(LR_RegPreambleLsb);
  
  s = SPIRead(LR_RegPreambleLsb);
    
  SPIWrite(REG_LR_DIOMAPPING2,0x01);                     //RegDioMapping2 DIO5=00, DIO4=01
  data = SPIRead(REG_LR_DIOMAPPING2);

  //¡äy?¨²
  sx1276_7_8_Standby();                                         //Entry standby mode
	
  //¨¦¨¨??3¨¦1|
  return s == 12;
}

