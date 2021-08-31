/*
 * File:   newmain.c
 * Author: abdal
 *
 * Created on August 24, 2021, 12:05 PM
 */


#include <xc.h>
#include <proc/pic16f877a.h>
#include "SPI.h"
#include <stdint.h>

uint8_t data=0;
uint8_t DataRead=0xff;
//master 
void main(void) {
   SPI_Master_Inti();
   TRISB=0xf;
   TRISD=0x00;
   PORTD=0x00;
   while(1){
       if(RB1==1){
          
           uint8_t buf = SPI_Write(0xf0);
            __delay_ms(200);
       }
      
   }
}

void __interrupt () ISR(void)
{
  if(SSPIF)
  {
    DataRead = SSPBUF; // Read The Buffer
    SSPIF = 0; // Clear The Interrupt Flag
  }
}
