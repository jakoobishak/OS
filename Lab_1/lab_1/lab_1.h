#include <wiringPi.h>

// Pin numbers you get from running
// $ gpio readall
// and then testing with
// $ gpio blink X
#define     LED_R               29
#define     LED_G               28
#define     LED_B               27


// These are functions that extracts values from
// lab_mutex.c so that they become available in main.c
void      kill_led_threads    ();
int       want_realtime       ();
void      get_rt_prios        (int * p_r, int * p_g, int * p_b);
int       milliseconds_to_run ();


// Functions that will run your written code
void      user_init           ();
void *    run_red             (void * arg);
void *    run_green           (void * arg);
void *    run_blue            (void * arg);

