#include "tim.h"
void TIM3_Configure(void)
{
    //定义定时器时基结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
    //打开TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStruct.TIM_Prescaler = (7200 - 1 );           //预分频 72MHz/(7200 - 1 + 1)
    TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseStruct.TIM_Period = 10000 - 1 ;                    // Maxvalue:0xFFFF 计数值
    // 1/(72Mhz / (7200 - 1 +1 ) / (9999 + 1)) = 1 s
    //完成时基设置
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct);
    //允许TIM3中断
    TIM_ITConfig(TIM3, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
    //使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
	u8 i;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		LED_Trigger();
		// LED1
		dat_S2_Transmit[5] = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2);
		// LED2
		dat_S2_Transmit[6] = GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3);
		for (i = 0; i < 8; ++i)
		{
			// SendData(USART2, dat_S2_Receive[i]);
			SendData(USART2, dat_S2_Transmit[i]);
		}
	}
}