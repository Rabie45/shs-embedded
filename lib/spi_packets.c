/////// BEGINING OF SLAVE DECLARATIONS /////////
#ifdef SPI_PACKETS_AS_SLAVE

uint8_t _internals_SPIPackets_state = SPI_PACKETS_STATE_RECEIVING;
uint8_t _internals_SPIPackets_incomingPacket[SPI_PACKETS_IN_PACKET_SIZE];
uint8_t _internals_SPIPackets_outgoingPacket[SPI_PACKETS_OUT_PACKET_SIZE];
uint8_t _internals_SPIPackets_sentBytesCount = 0;
uint8_t _internals_SPIPackets_receivedBytesCount = 0;
uint8_t _internals_SPIPackets_ignoredByte = 0;

void SPIPackets_listen()
{
    if (_internals_SPIPackets_state == SPI_PACKETS_STATE_BUSY)
    {

#ifdef SPI_PACKETS_HANDLER
        SPI_PACKETS_HANDLER(_internals_SPIPackets_incomingPacket, _internals_SPIPackets_outgoingPacket);
#endif

        // reset all cursors
        _internals_SPIPackets_sentBytesCount = 0;
        _internals_SPIPackets_receivedBytesCount = 0;
        _internals_SPIPackets_state = SPI_PACKETS_STATE_SENDING;
    }
}

void SPIPackets_resolveISR()
{
    if (SSPIF)
    {
        SSPIF = 0;
        if (_internals_SPIPackets_state == SPI_PACKETS_STATE_BUSY)
        {
            _internals_SPIPackets_ignoredByte = SSPBUF; // dummy read
            SSPBUF = 0x00;
        }
        else if (_internals_SPIPackets_state == SPI_PACKETS_STATE_RECEIVING)
        {

            _internals_SPIPackets_incomingPacket[_internals_SPIPackets_receivedBytesCount++] = SSPBUF;
            if (_internals_SPIPackets_receivedBytesCount >= _internals_SPIPackets_incomingPacket[0])
            {
                SSPBUF = 0;
                _internals_SPIPackets_state = SPI_PACKETS_STATE_BUSY; // process the received packet
            }
        }
        else if (_internals_SPIPackets_state == SPI_PACKETS_STATE_SENDING)
        {
            _internals_SPIPackets_ignoredByte = SSPBUF; // dummy read
            SSPBUF = _internals_SPIPackets_outgoingPacket[_internals_SPIPackets_sentBytesCount++];

            if (_internals_SPIPackets_sentBytesCount > _internals_SPIPackets_outgoingPacket[0])
            {
                _internals_SPIPackets_state = SPI_PACKETS_STATE_RECEIVING;
            }
        }
    }
}

////////// END OF SLAVE DECLARATIONS ///////////
/////// BEGINING OF MASTER DECLARATIONS ////////
#elif defined(SPI_PACKETS_AS_MASTER)
/**
 * Sends a packet with the provided params
 */
void SPIPackets_sendPacket(uint8_t size, uint8_t type, uint8_t *payload)
{
    SPIPackets_sendByte(size > 2 ? size : 2);
    SPIPackets_sendByte(type);
    size -= 2;
    for (uint8_t i = 0; i < size; i++)
    {
        SPIPackets_sendByte(payload[i]);
    }
#if SPI_PACKETS_GAP > 0
    _delay(SPI_PACKETS_GAP);
#endif
}

uint8_t SPIPackets_sendByte(uint8_t byte)
{
    SSPBUF = byte;
    while (SSPIF == 0)
        ; // wait for the byte to be sent
    SSPIF = 0;
#if SPI_PACKETS_BYTES_GAP > 0
    _delay(SPI_PACKETS_BYTES_GAP);
#endif
    return SSPBUF;
}

void SPIPackets_readPacket(uint8_t *packet)
{
    while ((packet[0] = SPIPackets_sendByte(0x00)) == 0)
    {
#if SPI_PACKETS_GAP > 0
        _delay(SPI_PACKETS_GAP);
#endif
    }
    // once the size is recieved, the packet is collected into the buffer
    for (uint8_t i = 1; i < packet[0]; i++)
    {
        packet[i] = SPIPackets_sendByte(0x00);
    }
#if SPI_PACKETS_GAP > 0
    _delay(SPI_PACKETS_GAP);
#endif
}
#endif
///////// END OF MASTER DECLARATIONS ///////////