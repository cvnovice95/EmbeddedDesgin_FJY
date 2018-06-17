/*
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * 
 *
 * This file is a generated sample test application.
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * SDK application project when you run the "Generate Libraries" menu item.
 *
 */


#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xintc.h"
//#include "xil_exception.h"
#include "mb_interface.h"
#define D0 0x3f
#define D1 0x06
#define D2 0x5b
#define D3 0x4f
#define D4 0x66
#define D5 0x6d
#define D6 0x7d
#define D7 0x07
#define D8 0x7f
#define D9 0x6f
#define DELAY 50
#define LED_BUS_CHANNEL 1
#define DATA_L_CHANNEL 1
#define LED_L_CHANNEL 1
#define U_CHANNEL 1
#define _OUT 0
#define _IN 1
#define POSTION_SEL(a) XGpio_DiscreteWrite(&LED_BUS,LED_BUS_CHANNEL,a)
#define SEND_DATA(a) XGpio_DiscreteWrite(&LED_BUS,LED_BUS_CHANNEL,a)
#define ENABLE_SIGNAL(a) XGpio_DiscreteWrite(&a,U_CHANNEL,1)
#define LATCH_SIGNAL(a)  XGpio_DiscreteWrite(&a,U_CHANNEL,0)
#define SET_DIRECTION(a,b) XGpio_SetDataDirection(&a,U_CHANNEL,b)

#define ONE_SECOND	 	0x02FAF080
#define HALF_SECOND		0x017D7840
#define SCAN_50HZ       0xFFFE17B7
#define ONE_S 			0xFD050F7F
#define THREE_S			0xF4143DFF
#define TIMER_CNTR_0	0
//#define LED_SEVEN
#define ENABLE_TIMER
#ifdef ENABLE_TIMER

#define TIMER_DEVICE_ID XPAR_TMRCTR_0_DEVICE_ID
#define TIMER_INT_VECTOR_ID XPAR_INTC_0_TMRCTR_0_VEC_ID
#define INT_DEVICE_ID XPAR_INTC_0_DEVICE_ID
#define TIMER_CONTROLLER_NUMBER 0
#define INT_CONTROLLER XIntc
#define TIMER_CONTROLLER XTmrCtr

#endif
XGpio DATA_L_GPIO;
XGpio LED_L_GPIO;
XGpio LED_BUS;
void Delay(unsigned int nDelay)
{
   unsigned int i,j,k;
   for ( i = 0;i<nDelay;i++ ){
	   for ( j = 0;j<110;j++ ){
		   k++;
	   }
   }
}
int Init_GPIO(){
	int STATUS =  XGpio_Initialize(&DATA_L_GPIO,XPAR_DATA_L_GPIO_DEVICE_ID)|
			      XGpio_Initialize(&LED_L_GPIO,XPAR_LED_L_GPIO_DEVICE_ID)|
			      XGpio_Initialize(&LED_BUS,XPAR_LED_SEVENSEGMENT_DEVICE_ID);
	return ((STATUS==XST_SUCCESS)?TRUE:FALSE);
}
unsigned char table[8] = { D0,D1,D2,D3,D4,D5,D6,D7};
unsigned char StudentNumber[8] = { D2,D0,D1,D4,D1,D7,D4,D6};
#ifdef LED_SEVEN
int main() 
{

	unsigned char LED_BUS_SEND = 0xff;
	unsigned char i;
	if(!Init_GPIO()==TRUE){
		return -1;
	}
	SET_DIRECTION(DATA_L_GPIO,_OUT);
	SET_DIRECTION(LED_L_GPIO,_OUT);
	SET_DIRECTION(LED_BUS,_OUT);
    while(1){
    	for(i=0;i<8;i++){
    		SEND_DATA(LED_BUS_SEND);
    		ENABLE_SIGNAL(DATA_L_GPIO);
    		ENABLE_SIGNAL(LED_L_GPIO);

    		LATCH_SIGNAL(DATA_L_GPIO);
    		LATCH_SIGNAL(LED_L_GPIO);

      	    LED_BUS_SEND = 0x01<<i;
      	    LED_BUS_SEND = ~LED_BUS_SEND;
      	    POSTION_SEL(LED_BUS_SEND);
      	    ENABLE_SIGNAL(LED_L_GPIO);
      	    LATCH_SIGNAL(LED_L_GPIO);

      	    SEND_DATA( table[i]);
      	    ENABLE_SIGNAL(DATA_L_GPIO);
      	    LATCH_SIGNAL(DATA_L_GPIO);
      	    Delay(DELAY);
    	}
    }
   return 0;
}
#endif
#ifdef ENABLE_TIMER
INT_CONTROLLER InterruptController;
TIMER_CONTROLLER TimerController;
unsigned char i = 0;
void Timer0InterruptHandler(void *CallbackRef, u8 TmrCtrNumber){
	unsigned char LED_BUS_SEND = 0xff;
	SEND_DATA(LED_BUS_SEND);
	ENABLE_SIGNAL(DATA_L_GPIO);
	ENABLE_SIGNAL(LED_L_GPIO);

	LATCH_SIGNAL(DATA_L_GPIO);
	LATCH_SIGNAL(LED_L_GPIO);

	LED_BUS_SEND = 0x01<<i;
    LED_BUS_SEND = ~LED_BUS_SEND;
    POSTION_SEL(LED_BUS_SEND);
    ENABLE_SIGNAL(LED_L_GPIO);
    LATCH_SIGNAL(LED_L_GPIO);

    SEND_DATA( table[i]);
	ENABLE_SIGNAL(DATA_L_GPIO);
    LATCH_SIGNAL(DATA_L_GPIO);

    i=(i+1)%8;
	XTmrCtr_Reset(&TimerController,TIMER_CNTR_0);
}
int main(){
	microblaze_enable_interrupts();
	if(!Init_GPIO()==TRUE){
			return -1;
	}
	SET_DIRECTION(DATA_L_GPIO,_OUT);
	SET_DIRECTION(LED_L_GPIO,_OUT);
	SET_DIRECTION(LED_BUS,_OUT);
	int Status = 0;

	Status = XTmrCtr_Initialize(&TimerController,XPAR_TMRCTR_0_DEVICE_ID);
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}
	//初始化中断控制器
	Status = XIntc_Initialize(&InterruptController,XPAR_MICROBLAZE_0_INTC_DEVICE_ID);
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}
	//中断控制器连接 时钟  设置中断服务函数
	Status =  XIntc_Connect(&InterruptController,XPAR_INTC_0_TMRCTR_0_VEC_ID,(XInterruptHandler)Timer0InterruptHandler,TIMER_CNTR_0);
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}
	//中断使能
	XIntc_Enable(&InterruptController,XPAR_INTC_0_TMRCTR_0_VEC_ID);
    //设置时钟参数  中断后重新载入
    XTmrCtr_SetOptions(&TimerController,TIMER_CNTR_0,XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);
    //设置时钟计数值
    XTmrCtr_SetResetValue(&TimerController,TIMER_CNTR_0,SCAN_50HZ);
    //开启时钟
    XTmrCtr_Start(&TimerController,TIMER_CNTR_0);
    //以实模式开启中断
    Status = XIntc_Start(&InterruptController,XIN_REAL_MODE);
    if(Status != XST_SUCCESS){
    	return XST_FAILURE;
    }
    while(1){

    }
    //关闭中断
    //XIntc_Disable(&InterruptController,TIMER_INT_VECTOR_ID);
    XTmrCtr_Stop(&TimerController,TIMER_CNTR_0);
	return 0;
}
#endif

