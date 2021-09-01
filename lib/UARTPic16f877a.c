/*
 * File:   UARTPic16f877a.c
 * Author: abdal
 *
 * Created on August 31, 2021, 5:00 PM
 */


#include <xc.h>
#include "UARTPic16f877a.h"
void UART_TInit(uint32_t BaudRate)
{
  uint16_t x;
  x = (_XTAL_FREQ - BaudRate*64)/(BaudRate*64); // Low Baud Rate
  if(x>255)  
  {
    x = (_XTAL_FREQ - BaudRate*16)/(BaudRate*16); // High Baud Rate
    BRGH = 1;
  }
  if (x<=255)
  {
    SPBRG = x; //Writing SPBRG Register
  }
  //--[ Enable The Ascynchronous Serial Port ]--
  SYNC = 0;
  SPEN = 1;
  //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
  TRISC6 = 1;  // As stated in the datasheet
  TRISC7 = 1;  // As stated in the datasheet
  TXEN = 1; // Enable UART Transmission
}
 
void UART_Write(uint8_t data)
{
  while(!TRMT);
  TXREG = data;
}
void newLine()
{
  TXREG = '\0';
}
 
void UART_Write_String(char *text)
{
  uint16_t i;
  for(i=0;text[i]!='\0';i++)
    UART_Write(text[i]);
}



void UART_RInit(uint32_t BaudRate)
{
  uint16_t x;
  x = (_XTAL_FREQ - BaudRate*64)/(BaudRate*64); // Low Baud Rate
  if(x>255)
  {
    x = (_XTAL_FREQ - BaudRate*16)/(BaudRate*16); // High Baud Rate
    BRGH = 1;
  }
  if (x<=255)
  {
    SPBRG = x; //Writing SPBRG Register
  }
  // Enable The Ascynchronous Serial Port
  SYNC = 0;
  SPEN = 1;
  // Set The RX-TX Pins to be in UART mode (not io)
  TRISC6 = 1; // As stated in the datasheet
  TRISC7 = 1; // As stated in the datasheet
  //--[ Enable UART Receiving Interrupts ]--
  RCIE = 1; // UART Receving Interrupt Enable Bit
  PEIE = 1; // Peripherals Interrupt Enable Bit
  GIE = 1; // Global Interrupt Enable Bit
  //------------------
  CREN = 1; // Enable Data Continous Reception
}
 
uint8_t UART_Read()
{
  while(!RCIF); // Wait Untill a Data Frame is Received
  return RCREG;
}
 
void UART_Read_String(uint8_t *Output, uint16_t length)
{
  uint16_t i;
  for(int i=0;i<length;i++)
    Output[i] = UART_Read();
}