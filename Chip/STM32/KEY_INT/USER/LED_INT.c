#include "LED_INT.h"

void Config_Led(){
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
void Config_NVIC(){
	//配置NVIC 中断控制器
	//定义NVIC结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	//设置 抢占优先级 和 响应优先级 分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//初始化结构体
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//  这是中断向量  查找文件stm32f10x.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;   //中断使能
	
	NVIC_Init(&NVIC_InitStructure);
}
void Config_EXIT1_PC1(){
	//定义GPIO 初始化结构体 
	//定义外部中断EXIT1 初始化结构体
	EXTI_InitTypeDef EXIT_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// 开启时钟   GPIO 功能复用 必须开启 AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	// 初始化 NVIC控制器
	Config_NVIC();
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//连接GPIO PC1至外部中断
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	
	EXIT_InitStructure.EXTI_Line = EXTI_Line1;   //设置中断线
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿出发
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXIT_InitStructure);
}