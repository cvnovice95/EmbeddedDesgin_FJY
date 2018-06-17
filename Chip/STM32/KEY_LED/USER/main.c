#include "LED_KEY.h"

void Delay(__IO u32 nCount);
u8 Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
int main(void)
{
	LED_CONFIG_PA8();
	KEY_CONFIG_PC1();
	LED_PA_8(LED_OFF);
	while(1){
		if(Key_Scan(GPIOC,GPIO_Pin_1)==KEY_ON){
			GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)(1-GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)));
		}
	}
}
void Delay(__IO u32 nCount){
	for(;nCount!=0;nCount--);
}
u8 Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON){
		Delay(10000);//延时消抖
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON){
			 while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON);//等待按键松开
			return KEY_ON;
		}else{
		return KEY_OFF;
		}
	}else{
		return KEY_OFF;
	}
}
