#include "LED_INT.h"
void Delay(u32 nDelay){
	u32 i=0;
	for(;i<nDelay;i++);
}
int main(){
	Config_Led();
	LED(ON);
	Config_EXIT1_PC1();
	while(1){
		//产生中断 跳到中断处理函数
		// stm32f10x_it.c
	}
	return 0;
}