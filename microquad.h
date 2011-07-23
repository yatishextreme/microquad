#define PPM_P1MASK 0xFF

#define YAW_CH 4
#define THROTTLE_CH 3
#define PITCH_CH 2
#define ROLL_CH 0
#define CH5_CH 5
#define CH6_CH 6

void clock_init(void);
void p1_init();
void timer_a3_init(void);
void calibrate_radio(void);
void load_transmitter_values(void);
void draw_rc_inputs();