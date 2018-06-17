#include "LED_KEY.h"

void LED_CONFIG_PA8(void){
	//define GPIO structure
	GPIO_InitTypeDef GPIO_InitStructure;
	//set Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//set GPIO parameter
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	//Init GPIOA
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void KEY_CONFIG_PC1(void){
	//define GPIO structure
	GPIO_InitTypeDef GPIO_InitStructure;
	//set Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//set GPIO parameter
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	//Init GPIOC
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}


	