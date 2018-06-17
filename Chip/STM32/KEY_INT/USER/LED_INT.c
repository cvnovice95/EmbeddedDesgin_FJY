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
	//����NVIC �жϿ�����
	//����NVIC�ṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//���� ��ռ���ȼ� �� ��Ӧ���ȼ� ����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//��ʼ���ṹ��
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//  �����ж�����  �����ļ�stm32f10x.h
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd  = ENABLE;   //�ж�ʹ��
	
	NVIC_Init(&NVIC_InitStructure);
}
void Config_EXIT1_PC1(){
	//����GPIO ��ʼ���ṹ�� 
	//�����ⲿ�ж�EXIT1 ��ʼ���ṹ��
	EXTI_InitTypeDef EXIT_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// ����ʱ��   GPIO ���ܸ��� ���뿪�� AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);
	// ��ʼ�� NVIC������
	Config_NVIC();
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//����GPIO PC1���ⲿ�ж�
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	
	EXIT_InitStructure.EXTI_Line = EXTI_Line1;   //�����ж���
	EXIT_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXIT_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��س���
	EXIT_InitStructure.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXIT_InitStructure);
}