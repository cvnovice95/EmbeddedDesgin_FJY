#ifndef __UART_H
#define __UART_H
#include "stm32f10x.h"
#include <stdio.h>

void NVIC_config(void);
void UART_config(void);
int fputc(int ch ,FILE* f);

#endif
