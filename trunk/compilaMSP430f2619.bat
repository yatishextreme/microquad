msp430-gcc -Os -mmcu=msp430x2232 -O -O2 -Wall -g %1.c -o %1.o lcd6100.o delay.o timers.o -lm
msp430-objdump -DS  %1.o > %1.lst
msp430-objcopy -O  ihex %1.o %1.hex

