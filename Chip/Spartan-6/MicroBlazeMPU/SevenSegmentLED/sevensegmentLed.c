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
XGpio DATA_L_GPIO;
XGpio LED_L_GPIO;
XGpio LED_BUS;
void Delay(unsigned int nDelay)
{
   unsigned int i,j,k;
   for ( i=0;i<nDelay;i++ ){
	   for ( j=0;j<110;j++ ){
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
unsigned char table[8]={ D0,D1,D2,D3,D4,D5,D6,D7};
unsigned char StudentNumber[8]={ D2,D0,D1,D4,D1,D7,D4,D6};
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

