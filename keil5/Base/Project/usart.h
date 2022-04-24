#ifndef _USART_H_
#define _USART_H_

#include "main.h"
// extern u8 dat_S1_Receive[8];
// extern u8 dat_S1_Transmit[8];
extern u8 dat_S2_Receive[8];
extern u8 dat_S2_Transmit[8];
// extern u8 flag_S1_Receive;
extern u8 flag_S2_Receive;
void USART1_Configure(void);
void USART2_Configure(void);
void SendData(USART_TypeDef *USARTx, u8 dat);
#endif
