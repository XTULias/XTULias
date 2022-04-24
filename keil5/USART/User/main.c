#include "stm32f10x.h"
#include <stdio.h>

#define BufferSize 32

typedef enum
{
	FAILED = 0,
	PASSED = !FAILED
} TestStatus;
__IO uint32_t CurrDataCounterBegin = 0;	 //开始传输前，通道数据量
__IO uint32_t CurrDataCounterEnd = 0x01; //传输结束前，通道数据量

 uint32_t SRC_Const_Buffer[BufferSize] = {
	//位于Flash，作为DMA传送源地址
	0x01020304,
	0x05060708,
	0x090A0B0C,
	0x0D0E0F10,
	0x11121314,
	0x15161718,
	0x191A1B1C,
	0x1D1E1F20,
	0x21222324,
	0x25262728,
	0x292A2B2C,
	0x2D2E2F30,
	0x31323334,
	0x35363738,
	0x393A3B3C,
	0x3D3E3F40,
	0x41424344,
	0x45464748,
	0x494A4B4C,
	0x4D4E4F50,
	0x51525354,
	0x55565758,
	0x595A5B5C,
	0x5D5E5F60,
	0x61626364,
	0x65666768,
	0x696A6B6C,
	0x6D6E6F70,
	0x71727374,
	0x75767778,
	0x797A7B7C,
	0x7D7E7F80,
};

uint32_t DST_Buffer[BufferSize]; //位于RAM，作为DMA传送目的的地址
void NVIC_Configure(void);
void DMA_Configure(void);
void USART_Configure(void);
void USART_GPIO_Configure(void);
TestStatus Buffercmp(const uint32_t *Buffer_SRC, uint32_t *Buffer_DST, uint16_t BufferLength);
void delay_ms(int32_t ms);
int main(void)
{
	uint32_t count;						//数据计数
	TestStatus TransferStatus = FAILED; //发送状态标志
	NVIC_Configure();
	USART_Configure();
	DMA_Configure();
	delay_ms(1000);
	printf("\n--------------------DMA Test--------------------\n");
	printf("\n--------------------Complete Initializatino--------------------\n");
	while (CurrDataCounterEnd != 0)
		;
	printf("\n--Complete Transmission-----\n");
	printf("\n---Contents of SRC_Const_Buffer:");
	for (count = 0; count < BufferSize; ++count)
	{
		if (count % 4 == 0)
			printf("\n");
		printf("0x%08x", DST_Buffer[count]); // 16进制，8位，左补0
	}
	TransferStatus = Buffercmp(SRC_Const_Buffer, DST_Buffer, BufferSize);
	if (TransferStatus == FAILED)
		printf("\nDMA test fail!\n");
	else
		printf("\n\nDMA test success!\n");
	SRC_Const_Buffer[0]=0;
	DMA_Configure();
	while (1)
		;
}
void NVIC_Configure(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void DMA_Configure(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel4);
	// DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;	// FLASH外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;			// RAM外设基地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//传输方向，外设为源地址，即FLASH -> RAM SRC-Source DST-Destination
	DMA_InitStructure.DMA_BufferSize = BufferSize+2;							//缓冲区大小，即一次传输的数据量，0-65535
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//外设地址自增1
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// RAM地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据宽度，32-字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;			// RAM数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//传输模式 一次传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//优先级 高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;								//内存到内存传输
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	CurrDataCounterBegin = DMA_GetCurrDataCounter(DMA1_Channel4); //开始传输前数据量
	DMA_Cmd(DMA1_Channel4, ENABLE);
}
void USART_Configure(void)
{
	
	USART_InitTypeDef USART_InitStructure;
	
	USART_DeInit(USART1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_GPIO_Configure();
	USART_InitStructure.USART_BaudRate = 9600; //波特率9600bit/s
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//奇偶失能 emm应该是不需要检验的意思  P83
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式使能还是失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//定义了奇偶模式，Rx接收使能 Tx发送使能
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //接收中断
	USART_GetFlagStatus(USART1, USART_FLAG_TC);	   //解决第一个字符发送不出去
	USART_Cmd(USART1, ENABLE);
}
void USART_GPIO_Configure(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	// TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  // PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入 -RX
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (u8)ch);								  //串口发送数据
	while (!(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)) //等待发送完成
		;
	return ch;
}
TestStatus Buffercmp(const uint32_t *Buffer_SRC, uint32_t *Buffer_DST, uint16_t BufferLength)
{
	while (--BufferLength)
	{
		if (*Buffer_SRC != *Buffer_DST)
			return FAILED;
		++Buffer_DST;
		++Buffer_SRC;
	}
	return PASSED;
}
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
void DMA1_Channel4_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC4)) //传输完成中断屏蔽  HT半中断 TE传输错误中断
	{
		CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel4); //传输完成后得到当前计数
		DMA_ClearITPendingBit(DMA1_IT_GL4);							//清除中断 GLx通道x全局标志位
	}
}
