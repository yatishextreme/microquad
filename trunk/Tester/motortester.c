#include "lcd6100.h"
#include "delay.h"
#include "timers.h"
#include "msp430x22x2.h"
#include "stdlib.h"
#include "stdio.h"
#include <io.h>
#include <msp430/iostructures.h> // serve para acessar pinos individuais de uma port

/* output1 e output2 from 2000 to 4000 */

#define UP BIT3
#define DOWN BIT4

/* tentativa frustrada de acessar bits de um registrador
typedef union{
	struct{
		unsigned char bit0:1;
		unsigned char bit1:1;
		unsigned char bit2:1;
		unsigned char bit3:1;
		unsigned char bit4:1;
		unsigned char bit5:1;
		unsigned char bit6:1;
		unsigned char bit7:1;
	}Byte;
	unsigned char uChar;
}REGISTER;
*/

int main(){
	unsigned int periodo = 2000;

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT        
	int i;
	
	// config io
	P3DIR = 0x0F;
	P1DIR = 0x00;
	//P2DIR = 0xFF;
	port2.dir.pin0 = 1;
	
	//config clock
    BCSCTL1 |= XTS;	// talvez nao seja muito bom dividir por 8 aqui
    BCSCTL3 |= LFXT1S1;                       // 3 – 16MHz crystal or resonator
    do{
    	IFG1 &= ~OFIFG;                       // Clear OSCFault flag
	    for (i = 0xFF; i > 0; i--);           // Time for flag to set
    }
    while (IFG1 & OFIFG);                     // OSCFault flag still set?
    BCSCTL2 |= SELM_3 + SELS;                 // MCLK = LFXT1 (safe), SMCLK = 16MHz
	
    // periodo de 1ms
    TMRB_init(periodo);
    
	for(;;){
		TBCCR1 = 0;
		delayms(1000);
		
		port2.out.pin0 ^= 1;
		
		for(i = 0; i < periodo; i++){
			TBCCR1 = i;
			delayms(5);
		}
	}
}
