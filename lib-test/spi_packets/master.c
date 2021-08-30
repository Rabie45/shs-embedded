#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)

#define _XTAL_FREQ 16000000
#define SPI_PACKETS_GAP 5000
#define SPI_PACKETS_BYTES_GAP 80
#define SPI_PACKETS_AS_MASTER
#define DEVICE_0 RD0


#include <xc.h>
#include <proc/pic16f877.h>
#include <stdint.h>
#include "../lib/spi_packets.h"

uint8_t buffer[29] = {5, 4, 0x0c, 0x0d};
uint8_t responsePacket[29];


int main()
{
    TRISD0 = 0; // configure RD0 as SS
    SPIPackets_initialize();
    __delay_ms(1000); // wait for slave to come up
    while (1)
    {

        sendPacket(DEVICE_0, 4, 0x01, buffer);   // send 5 + 4
        receivePacket(DEVICE_0, responsePacket); // receive result
        __delay_ms(20);
        sendPacket(DEVICE_0, 4, 0x02, buffer);   // send 5 - 4
        receivePacket(DEVICE_0, responsePacket); // receive result
        __delay_ms(20);
    }
}


