#ifndef __ANALOG
#define __ANALOG

#define ANALOG_ENABLE 0xFF

#define BATTERY_CH 7 
#define GYROY_CH 1
#define GYROX_CH 0
#define GYROZ_CH 2
#define ACCELX_CH 5
#define ACCELY_CH 4
#define ACCELZ_CH 3
#define ADC_EXTRA_CH 6

extern int AnalogOffset[8];
extern int AnalogValue[8];

void analog_init(void);
void analog_calibrate_channel(int channel);
void analog_refresh_all(void);
void analog_refresh_channel(int channel);
     
#endif /* __ANALOG */