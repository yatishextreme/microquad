#ifndef __TIMERS
#define __TIMERS

#define OUTPUT1 TBCCR1
#define OUTPUT2 TBCCR2
// 4020 = 2ms ; 2020 = 1ms
void TMRB_init(unsigned int period); // envia o periodo de contagem do timer junto

#endif /* __TIMERS */
