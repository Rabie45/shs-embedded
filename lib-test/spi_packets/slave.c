#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ 16000000
#define SPI_PACKETS_HANDLER spiHandler
#define SPI_PACKETS_AS_SLAVE

#include <xc.h>
#include <proc/pic16f877.h>
#include "../lib/spi_packets.h"
#include <stdint.h>
uint8_t counter = 0;
int main()
{
    SPIPackets_initialize();
    while (1)
    {
        SPIPackets_listen();
    }
}

void spiHandler(uint8_t *incomingPacket, uint8_t *responsePacket)
{
    uint8_t size = incomingPacket[0];
    uint8_t type = incomingPacket[1];
    switch (type)
    {
    case 0x01: // addition
        responsePacket[0] = 3;
        responsePacket[1] = 0x20;
        responsePacket[2] = incomingPacket[2] + incomingPacket[3];
        break;
    case 0x02: // subtraction
        responsePacket[0] = 3;
        responsePacket[1] = 0x30;
        responsePacket[2] = incomingPacket[2] - incomingPacket[3];
        break;
    default: // default respond with fail status packet
        responsePacket[0] = 3;
        responsePacket[1] = 0x40;
        responsePacket[2] = 0x03; // bad request
        break;
    }
}

void __interrupt() isr(void)
{
    SPIPackets_resolveISR();
}