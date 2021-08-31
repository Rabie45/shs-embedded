#ifndef SPI_H
#define SPI_H
#include "config.h"
#include <stdint.h>
void SPI_Master_Inti();
uint8_t SPI_Write(uint8_t);
uint8_t SPI_Read();
void SPI_Slave_Init();
uint8_t SPI_Read();
void SPI_Read_String(uint8_t *, uint16_t);

#endif