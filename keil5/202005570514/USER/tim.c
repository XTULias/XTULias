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

