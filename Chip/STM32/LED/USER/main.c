
#include "stm32f10x.h"
#include "led.h"
void Delay(__IO u32 nCount);
int main(void)
{
	//SystemInit();
  LED_GPIO_Config();
	LED_GPIO_Config_A();
  while(1)
	 {
			//GPIO_SetBits(GPIOD,GPIO_Pin_2);
		 LED1(ON);
		  Delay(0x0FFFEF);
		 LED1(OFF);
			//GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		 
		  //GPIO_SetBits(GPIOA,GPIO_Pin_8);
		 LED2(ON);
		  Delay(0x0FFFEF);
		 LED2(OFF);
			//GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	 }
}
void Delay(__IO u32 nCount){
	for(;nCount!=0;nCount--);
}
