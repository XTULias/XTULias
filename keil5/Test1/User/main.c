#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "main.h"
#include <stdio.h>
// 0 八进制 0x 十六进制 0b 二进制

u8 dat_S2_Receive[8];
u8 dat_S2_Transmit[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0x01, 0x00, 0x00, 0x00};
u8 flag_S2_Receive;
u8 n = 0;
int main(void)
{
	u8 i;
	flag_S2_Receive = 0;
	NVIC_Configure();
	USART1_Configure();
	USART2_Configure();
	LED_Configure();
	KEY_Configure();
	TIM3_Configure();
	while (1)
	{
		if (flag_S2_Receive)
		{ //下位机 接收到指令
			if ((dat_S2_Receive[0] == 0xAA) && (dat_S2_Receive[1] == 0xBB) && (dat_S2_Receive[2] == 0xCC) && (dat_S2_Receive[3] == 0xDD))
			{
				if (dat_S2_Receive[5] == 0x00)
					LED1_Off();
				else
					LED1_On();
			}
			else
			{
				for (i = 0; i < 8; ++i)
				{
					SendData(USART2, 0x00);
				}
			}
			flag_S2_Receive = 0;
			n = 0;
		}
	}
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

void USART2_IRQHandler(void)
{

	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		dat_S2_Receive[n] = USART_ReceiveData(USART2);
		n++;
		if (n >= 8)
		{
			flag_S2_Receive = 1;
			n = 0;
		}
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (u8)ch);								  //串口发送数据
	while (!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)) //等待发送完成
		;
	return ch;
}