#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "types.h"

void Usart_Control(ST_COMMAND *my_command,uint8_t rx[]);
void DC_Montor(uint16_t des,uint16_t fb);

#endif //__BSP_USART_H__