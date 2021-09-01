#include <xc.h> 
#include "config.h"
void UART_TInit(uint32_t);
void UART_Write(uint8_t);
void UART_Write_String(char *);
void UART_RInit(uint32_t);
uint8_t UART_Read(void);
void UART_Read_String(uint8_t *Output, uint16_t length);
void newLine();