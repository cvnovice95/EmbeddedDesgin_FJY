#include "uart.h"

void UART_config(){
	//GPIO Structure
	GPIO_InitTypeDef GPIO_InitStructure;
	//USART Structure
	USART_InitTypeDef USART1_InitStructure;
	//enable rcc
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	//Configuration GPIO
	//define TX PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//define RX PA10 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//Config USART
	USART1_InitStructure.USART_BaudRate = 115200;
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Mode =  USART_Mode_Tx |USART_Mode_Rx;
	USART_Init(USART1,&USART1_InitStructure);
	//INT RX
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);  //RX Data no empty
	
	//enable usart
	USART_Cmd(USART1,ENABLE);
}
void NVIC_config(){
	//NVIC Structure
	
	NVIC_InitTypeDef NVIC_InitStruture;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruture.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruture.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruture);
	
}
int fputc(int ch ,FILE* f){
	USART_SendData(USART1,(unsigned char)ch);
	// 等待发送完成
	while(!(USART1->SR & USART_FLAG_TXE));
  //while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=SET);
	return (ch);
}