#include "usart.h"
void USART1_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // APB2外设时钟使能 还有一个APB1外设 不同外设时钟不一样
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
    USART_DeInit(USART1);
    //端口设置在P85
    // TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    // USART1的发送引脚和PA9复用，因此推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            // PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入 -RX

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600; //波特率9600bit/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //帧结尾传输1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;    //奇偶失能 emm应该是不需要检验的意思  P83
    //置奇偶校验属性后，必须要修改数据位为9位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制模式使能还是失能
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //定义了奇偶模式，Rx接收使能 Tx发送使能
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
    USART_GetFlagStatus(USART1, USART_FLAG_TC); //解决第一个字符发送不出去
}

void USART2_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    // emm USART2是APB1的时钟
    USART_DeInit(USART2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    // TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //中断使能没有打开
    USART_Cmd(USART2, ENABLE);
    USART_GetFlagStatus(USART2, USART_FLAG_TC); //解决第一个字符发送不出去
}

void SendData(USART_TypeDef *USARTx, u8 dat)
{
    USART_SendData(USARTx, dat);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET)
        ;
}

// void USRTA1_IBQHandler(void)
// {
//     u8 i;
//     if (USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
//     {
//         for (i = 0; i < 8; ++i)
//         {
//             dat_S1_Receive[i] = USART_ReceiveData(USART1);
//         }
//         flag_S1_Receive = 1;
//         USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//     }
// }
