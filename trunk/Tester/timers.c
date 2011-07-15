#include "timers.h"
#include "msp430x22x2.h"

/* TIMER B */
// 4020 = 2ms ; 2020 = 1ms
void TMRB_init(unsigned int period){
    /* 
     * TBCCTL1, Capture/Compare Control Register 1
     * 
     * CM_0 -- No Capture
     * CCIS_0 -- CCIxA
     * OUTMOD_7 -- PWM output mode: 7 - PWM reset/set
     */
	TBCCTL1 = CM_0 + CCIS_0 + CLLD_0 + OUTMOD_7;

    /* TBCCR0, Timer_B Capture/Compare Register 0 */
    TBCCR0 = period;

    /* TBCCR1, Timer_B Capture/Compare Register 1 */
    TBCCR1 = 0; 

    /* 
     * TBCTL, Timer_B3 Control Register
     * 
     * CNTL_0 -- 16-bit, TBR(max) = 0FFFFh
     * TBSSEL_1 -- SMCLK
     * ID_3 -- Divider - /1
     * MC_1 -- Up Mode
     */
    TBCTL = TBCLGRP_0 + CNTL_0 + TBSSEL_2 + ID_0 + MC_1;
    
    /* configura saidas do timerB - P4.1 e P4.2*/
    P4SEL = BIT1 | BIT2;
    P4DIR = BIT1 | BIT2;
}
