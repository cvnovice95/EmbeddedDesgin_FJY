#ifndef __LED_KEY_H
#define __LED_KEY_H

#include "stm32f10x.h"

#define LED_ON 0
#define LED_OFF 1
#define KEY_ON 0
#define KEY_OFF 1

#define LED_PA_8(a) if(a) \
							GPIO_SetBits(GPIOA,GPIO_Pin_8); \
										else \
							GPIO_ResetBits(GPIOA,GPIO_Pin_8)
void LED_CONFIG_PA8(void);
void KEY_CONFIG_PC1(void);											
               
						 
	














#endif

