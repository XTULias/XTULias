#include "led.h"

void LED_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    LED1_Off();
    LED2_Off();
}

void LED1_On(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
void LED1_Off(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}
void LED2_On(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_3);
}
void LED2_Off(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_3);
}
void LED1_Blink(int8_t k)
{
    if (k != 0)
    {
        while (--k)
        {
            LED1_On();
            delay_ms(300);
            LED1_Off();
            delay_ms(300);
        }
    }
    else
    {
        LED1_On();
        delay_ms(1000);
        LED1_Off();
        delay_ms(1000);
    }
}

void LED_Trigger(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2)));
}
