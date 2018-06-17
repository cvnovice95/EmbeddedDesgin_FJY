#ifndef  __LED_INT_H
#define  __LED_INT_H
#include "stm32f10x.h"
#define ON 0
#define OFF 1
#define LED(a) if(a) \
						GPIO_SetBits(GPIOA,GPIO_Pin_8); \
					else	\
						GPIO_ResetBits(GPIOA,GPIO_Pin_8)
void Config_Led(void);
void Config_NVIC(void);
void Config_EXIT1_PC1(void);
#endif