#ifndef _LED_H_
#define _LED_H_
#include "main.h"

void LED_Configure(void);
void LED2_On(void);
void LED1_On(void);
void LED1_Off(void);
void LED2_Off(void);
void LED1_Blink(int8_t);
void LED_Trigger(void);
#endif
