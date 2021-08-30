#ifndef SPI_PACKETS_H
#define SPI_PACKETS_H

#include <stdint.h>
#include <xc.h>
/////// BEGINING OF SLAVE DECLARATIONS /////////
#ifdef SPI_PACKETS_AS_SLAVE

#define SPI_PACKETS_STATE_RECEIVING 0x00 // during the transmission of a packet from the master to the slave
#define SPI_PACKETS_STATE_BUSY 0x7f  // during the processing of a received packet
#define SPI_PACKETS_STATE_SENDING 0xff // during the transmission of a packet from the slave to the master

#ifndef SPI_PACKETS_IN_PACKET_SIZE
#define SPI_PACKETS_IN_PACKET_SIZE 32 
#endif

#ifndef SPI_PACKETS_OUT_PACKET_SIZE
#define SPI_PACKETS_OUT_PACKET_SIZE 32
#endif
// macro instead of a function to reduce stack levels
#define SPIPackets_initialize()        \
    SSPEN = 1;                         \
    TRISC5 = 0;                        \
    TRISC4 = 1;                        \
    TRISC3 = 1;                        \
    TRISA5 = 1;                        \
    SSPCON = (SSPCON & 0xf0) | 0b0100; \
    SSPIE = 1;                         \
    GIE = 1;                           \
    SSPBUF = 0;                        \
    PEIE = 1;

// macro instead of a function to reduce stack levels
void SPIPackets_resolveISR();

#ifdef SPI_PACKETS_HANDLER
void SPI_PACKETS_HANDLER(uint8_t *incomingPacket, uint8_t *responsePacket);
#endif
void SPIPackets_listen();
void SPIPackets_exchange(uint8_t byte);
///////// END OF SLAVE DECLARATIONS ////////////
/////// BEGINING OF MASTER DECLARATIONS ////////
#elif defined(SPI_PACKETS_AS_MASTER)
#ifndef SPI_PACKETS_GAP
#define SPI_PACKETS_GAP 2000
#endif
#ifndef SPI_PACKETS_GAP
#define SPI_PACKETS_GAP 50
#endif
#define SPIPackets_initialize() \
    SSPEN = 1;                  \
    TRISC5 = 0;                 \
    TRISC4 = 1;                 \
    TRISC3 = 0;                 \
    SSPCON = (SSPCON & 0xf0) | 0b0010;
uint8_t SPIPackets_sendByte(uint8_t byte);
void SPIPackets_sendPacket(uint8_t size, uint8_t type, uint8_t *payload);
void SPIPackets_readPacket(uint8_t *packet);
#endif
///////// END OF MASTER DECLARATIONS //////////

// include c file
#include "spi_packets.c"
#endif