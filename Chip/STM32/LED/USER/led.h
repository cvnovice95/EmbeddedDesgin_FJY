#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

#define ON 0
#define OFF 1

#define LED1(a) if(a) \
							 GPIO_SetBits(GPIOD,GPIO_Pin_2);\
							else \
							 GPIO_ResetBits(GPIOD,GPIO_Pin_2)
							
#define LED2(a) if(a) \
							 GPIO_SetBits(GPIOA,GPIO_Pin_8);\
							else \
							 GPIO_ResetBits(GPIOA,GPIO_Pin_8)

void LED_GPIO_Config(void);
void LED_GPIO_Config_A(void);					
							
#endif 