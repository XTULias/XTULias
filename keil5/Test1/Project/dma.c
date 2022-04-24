#include "dma.h"
void DMA_Configure(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	// DMA1时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;	// FLASH外设基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;			// RAM外设基地址
    
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//传输方向，外设为源地址，即FLASH -> RAM SRC-Source DST-Destination
	DMA_InitStructure.DMA_BufferSize = BufferSize;							//缓冲区大小，即一次传输的数据量，0-65535
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//外设地址自增1
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					// RAM地址自增1
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; //外设数据宽度，32-字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;			// RAM数据宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//传输模式 一次传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;						//优先级 高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;								//内存到内存传输
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
	CurrDataCounterBegin = DMA_GetCurrDataCounter(DMA1_Channel6); //开始传输前数据量
	DMA_Cmd(DMA1_Channel6, ENABLE);
}


void DMA1_Channel6_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC6)) //传输完成中断屏蔽  HT半中断 TE传输错误中断
	{
		CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel6); //传输完成后得到当前计数
		DMA_ClearITPendingBit(DMA1_IT_GL6);							//清除中断 GLx通道x全局标志位
	}
}