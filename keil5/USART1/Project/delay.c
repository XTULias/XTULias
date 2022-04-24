#include "delay.h"
void delay_ms(int32_t ms)
{
    int32_t i;
    while (--ms)
    {
        i = 7500;
        while (--i)
            ;
    }
}

void delay(void)
{
    int i = 0xffff;
    while (i--)
        ;
}
