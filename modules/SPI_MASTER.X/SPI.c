/*
 * File:   SPI.c
 * Author: abdal
 *
 * Created on August 24, 2021, 11:59 AM
 */


#include <xc.h>
#include <proc/pic16f877a.h>
#include"SPI.h"
#include "config.h"

void SPI_Master_Inti()
{
  // Set Spi Mode To Master + Set SCK Rate To Fosc/4
  SSPM0 = 0;
  SSPM1 = 1;
  SSPM2 = 0;
  SSPM3 = 0;
  // Enable The Synchronous Serial Port
  SSPEN = 1;
    __delay_ms(1);

  // Configure The Clock Polarity & Phase (SPI Mode Num. 1)
  CKP = 0;
  CKE = 0;
  // Configure The Sampling Time (Let's make it at middle)
  SMP = 0;
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 0; // SCK -> Output
   __delay_ms(1);
}

uint8_t SPI_Write(uint8_t Data)
{
  SSPBUF = Data; // Transfer The Data To The Buffer Register
  while(!SSPIF){
      ;
  }
  SSPIF=0;
  return SSPBUF;

}
void SPI_Slave_Init()
{
  // Set Spi Mode To Slave + SS Enabled
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 1;
  SSPM3 = 0;
  // Enable The Synchronous Serial Port
  SSPEN = 1;
  // Configure The Clock Polarity & Phase (SPI Mode Num. 1)
  CKP = 0;
  CKE = 0;
  // Clear The SMP Bit
  SMP = 0;
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 1; // SCK -> Intput
  PCFG3 = 0; // Set SS (RA5/AN4) To Be Digital IO
  PCFG2 = 1;
  PCFG1 = 0;
  PCFG0 = 0;
  TRISA5 = 1; // SS -> Input
  // Enable Interrupts. Comment Them To Disable interrupts(NOT Recommended)
  SSPIE = 1;
  PEIE = 1;
  GIE = 1;
}
 
uint8_t SPI_Read() // Not Recommended Method To Read SPI Data
{
  uint8_t Data;
  if(BF) // Check If Any New Data Is Received
  {
    Data = SSPBUF; // Read The Buffer
    BF = 0; // Clear The Buffer-Filled Indicator Bit
    SSPIF = 0; // Clear The Interrupt Flag Bit
    SSPOV = 0; // Clear The Overflow Indicator Bit
    return Data;
  }
}
void sendPacket_RouterPacket(uint8_t* size,uint8_t type,uint8_t *payload){
    
}
void sendPacket_customPacket(uint){
    
}



 