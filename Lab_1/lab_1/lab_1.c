#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <lttng/tracef.h>

#include "lab_1.h"
#include "thread_help.h"

/*************************************************************
 *                                                           *
 * RUNTIME_MILLSEC can be one of three things:               *
 *                                                           *
 *      -> If it's greater than 0 then it's how many         *
 *          milliseconds the program will run for. When the  *
 *          time is over kill_led_threads() will be called.  *
 *                                                           *
 *      -> If it's 0 then the program will run until         *
 *          the user presses RETURN in the terminal, at      *
 *          which time kill_led_threads() will be called.    *
 *                                                           *
 *      -> If it's less than 0 then kill_led_threads() won't *
 *          be called. Instead the threads should terminate  *
 *          themselves.                                      *
 *                                                           *
 *                                                           *
 * main.c:halting_function() for more details.               *
 *                                                           *
 *************************************************************/
#define RUNTIME_MILLSEC     8000
#define REALTIME_THREADS    0

// run `chrt -m` to learn allowed values (SCHED_FIFO)
#define RT_PRIO_R           3
#define RT_PRIO_G           2
#define RT_PRIO_B           1

#define EXERCISE_TO_RUN     9

#define NANOSECOND 1000000000
#define MILISECOND 1000

int running;  // leave this alone
struct timespec ex5_ts;
pthread_mutex_t ex8_mut, ex9_mut_r, ex9_mut_g, ex9_mut_b;


/*************************************************************
 *************************************************************
 **               PREPARATORY EXERCISE                      **
 *************************************************************
 *************************************************************/
void timespec_add_usec(struct timespec *ts, long microseconds);

void timespec_add_usec(struct timespec *ts, long us)
{
    const long start_nsec = ts->tv_nsec + us * MILISECOND;

    if (start_nsec >= NANOSECOND)
    {
        int i;

        for (i = 0; i < (start_nsec / NANOSECOND); ++i)
        {
            ts->tv_sec++;
        }

        ts->tv_nsec -= NANOSECOND * (start_nsec / NANOSECOND) - (us * MILISECOND);
    }
    else
    {
        ts->tv_nsec = start_nsec;
    }
}



/*************************************************************
 *************************************************************
 **               EXERCISE 1 - Blinking                     **
 *************************************************************
 *************************************************************/

void   ex1_init()
{
}

int pin_invert(int pin_state) {
    if (pin_state == LOW)
        return HIGH;

    return LOW;
}

void * ex1_red(void * arg)
{
    useconds_t red_sleep_us = 1000 * 1000; // 1s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_R, v);
        usleep(red_sleep_us);
    }
    return NULL;
}

void * ex1_green(void * arg)
{
    useconds_t green_sleep_us = 500 * 1000; // 0.5s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_G, v);
        usleep(green_sleep_us);
    }
    return NULL;
}

void * ex1_blue(void * arg)
{
    useconds_t blue_sleep_us = 200 * 1000; // 0.2s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_B, v);
        usleep(blue_sleep_us);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 2 - Adding tracef()              **
 *************************************************************
 *************************************************************/

void   ex2_init()
{
}

void * ex2_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex2_green(void * arg)
{
    useconds_t g_period_us = 500 * 1000; // 0.5s
    int v = LOW;
    while(running)
    {
        v = pin_invert(v);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        tracef("sleep(GREEN, %u usec)", g_period_us);
        usleep(g_period_us);
    }
    return NULL;
}

void * ex2_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 3 - Adding CPU Load              **
 *************************************************************
 *************************************************************/

void   ex3_init()
{
}

void * ex3_red(void * arg)
{
    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t)(0.2 * (float) r_period_ms);
    int v = LOW;

    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        usleep(r_period_us);
    }
    return NULL;
}

void * ex3_green(void * arg)
{
    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t)(0.2 * (float) g_period_ms);
    int v = LOW;

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        usleep(g_period_us);
    }
}

void * ex3_blue(void * arg)
{
    uint32_t b_period_ms = 200;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t)(0.2 * (float) b_period_ms);
    int v = LOW;

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        usleep(b_period_us);
    }
}




/*************************************************************
 *************************************************************
 **               EXERCISE 4 - Many Ways of Sleeping        **
 *************************************************************
 *************************************************************/

void   ex4_init()
{
}

void * ex4_red(void * arg)
{
    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = 200;
    int v = LOW;

    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        usleep(r_period_us);
    }
    return NULL;
}

void * ex4_green(void * arg)
{
    uint32_t g_period_ms = 1000;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = 200;
    int v = LOW;
    struct timespec required_time;

    timespec_add_usec(&required_time, g_period_us);

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        nanosleep(&required_time, NULL);
    }
}

void * ex4_blue(void * arg)
{
    uint32_t b_period_ms = 1000;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = 200;
    int v = LOW;
    struct timespec required_time;

    required_time.tv_sec = 0;
    required_time.tv_nsec = 0;

    clock_gettime(CLOCK_REALTIME, &required_time);

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        timespec_add_usec(&required_time, b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &required_time, NULL);
    }
}



/*************************************************************
 *************************************************************
 **               EXERCISE 5 - Properly Periodical          **
 *************************************************************
 *************************************************************/

void   ex5_init()
{
    clock_gettime(CLOCK_REALTIME, &ex5_ts);
}

void * ex5_red(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t)(0.2 * (float) r_period_ms);
    int v = LOW;

    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        timespec_add_usec(&wakeup, r_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);

    }
    return NULL;
}

void * ex5_green(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t)(0.2 * (float) g_period_ms);
    int v = LOW;

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        timespec_add_usec(&wakeup, g_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}

void * ex5_blue(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t b_period_ms = 200;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t)(0.2 * (float) b_period_ms);
    int v = LOW;
    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        timespec_add_usec(&wakeup, b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}




/*************************************************************
 *************************************************************
 **               EXERCISE 6 - Real-Time Scheduling         **
 *************************************************************
 *************************************************************/

void   ex6_init()
{
    clock_gettime(CLOCK_REALTIME, &ex5_ts);
}

void * ex6_red(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t)(0.2 * (float) r_period_ms);
    int v = LOW;

    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        timespec_add_usec(&wakeup, r_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);

    }
    return NULL;
}

void * ex6_green(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t)(0.2 * (float) g_period_ms);
    int v = LOW;

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        timespec_add_usec(&wakeup, g_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}

void * ex6_blue(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t b_period_ms = 200;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t)(0.2 * (float) b_period_ms);
    int v = LOW;
    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        timespec_add_usec(&wakeup, b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}


/*************************************************************
 *************************************************************
 **               EXERCISE 7 - CPU Overloading              **
 *************************************************************
 *************************************************************/

void   ex7_init()
{
    clock_gettime(CLOCK_REALTIME, &ex5_ts);
}

void * ex7_red(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t r_period_ms = 100;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = (uint32_t)(0.4 * (float) r_period_ms);
    int v = LOW;

    while(running) {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", r_stress_ms);
        cpu_stress(r_stress_ms);
        tracef("RED LED = %d", v);
        digitalWrite(LED_R, v);
        timespec_add_usec(&wakeup, r_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);

    }
    return NULL;
}

void * ex7_green(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t g_period_ms = 50;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = (uint32_t)(0.7 * (float) g_period_ms);
    int v = LOW;

    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", g_stress_ms);
        cpu_stress(g_stress_ms);
        tracef("GREEN LED = %d", v);
        digitalWrite(LED_G, v);
        timespec_add_usec(&wakeup, g_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}

void * ex7_blue(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t b_period_ms = 20;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = (uint32_t)(0.9 * (float) b_period_ms);
    int v = LOW;
    while (running)
    {
        v = pin_invert(v);
        tracef("Stressing for: %u ms", b_stress_ms);
        cpu_stress(b_stress_ms);
        tracef("BLUE LED = %d", v);
        digitalWrite(LED_B, v);
        timespec_add_usec(&wakeup, b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}



/*************************************************************
 *************************************************************
 **         EXERCISE 8 - [Bonus] Resource Protection        **
 *************************************************************
 *************************************************************/

void   ex8_init()
{
    clock_gettime(CLOCK_REALTIME, &ex5_ts);
    pthread_mutex_init(&ex8_mut, NULL);
}

void * ex8_red(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t r_period_ms = 1000;
    useconds_t r_period_us = r_period_ms * 1000;
    uint32_t r_stress_ms = 50;
    int v = LOW;

    while(running) {

        pthread_mutex_lock(&ex8_mut);
        v = pin_invert(v);
        cpu_stress(r_stress_ms);
        digitalWrite(LED_R, v);
        pthread_mutex_unlock(&ex8_mut);

        timespec_add_usec(&wakeup, r_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);

    }
    return NULL;
}

void * ex8_green(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    uint32_t g_stress_ms = 50;
    int v = LOW;

    while (running)
    {
        pthread_mutex_lock(&ex8_mut);
        v = pin_invert(v);
        cpu_stress(g_stress_ms);
        digitalWrite(LED_G, v);
        pthread_mutex_unlock(&ex8_mut);

        timespec_add_usec(&wakeup, g_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}

void * ex8_blue(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t b_period_ms = 200;
    useconds_t b_period_us = b_period_ms * 1000;
    uint32_t b_stress_ms = 50;
    int v = LOW;
    while (running)
    {
        pthread_mutex_lock(&ex8_mut);
        v = pin_invert(v);
        cpu_stress(b_stress_ms);
        digitalWrite(LED_B, v);
        pthread_mutex_unlock(&ex8_mut);

        timespec_add_usec(&wakeup, b_period_us);
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &wakeup, NULL);
    }
}


/*************************************************************
 *************************************************************
 **         EXERCISE 9 - [Bonus] Control Flow               **
 *************************************************************
 *************************************************************/

void   ex9_init()
{
    clock_gettime(CLOCK_REALTIME, &ex5_ts);
    pthread_mutex_init(&ex9_mut_r, NULL);
    pthread_mutex_init(&ex9_mut_g, NULL);
    pthread_mutex_init(&ex9_mut_b, NULL);
    pthread_mutex_lock(&ex9_mut_g);
    pthread_mutex_lock(&ex9_mut_b);
}

void * ex9_red(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t r_period_ms = 500;
    useconds_t r_period_us = r_period_ms * 1000;
    int v = LOW;

    while(running)
    {
        pthread_mutex_lock(&ex9_mut_r);

        v = pin_invert(v);
        digitalWrite(LED_R, v);

        timespec_add_usec(&wakeup, r_period_us);
        usleep(r_period_us);

        pthread_mutex_unlock(&ex9_mut_b);

        v = pin_invert(v);
        digitalWrite(LED_R, v);


    }
    return NULL;
}

void * ex9_green(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t g_period_ms = 500;
    useconds_t g_period_us = g_period_ms * 1000;
    int v = LOW;

    while (running)
    {
        pthread_mutex_lock(&ex9_mut_g);

        v = pin_invert(v);
        digitalWrite(LED_G, v);

        timespec_add_usec(&wakeup, g_period_us);
        usleep(g_period_us);

        pthread_mutex_unlock(&ex9_mut_r);

        v = pin_invert(v);
        digitalWrite(LED_G, v);
    }
}

void * ex9_blue(void * arg)
{
    struct timespec wakeup;
    wakeup.tv_sec = ex5_ts.tv_sec;
    wakeup.tv_nsec = ex5_ts.tv_nsec;

    uint32_t b_period_ms = 500;
    useconds_t b_period_us = b_period_ms * 1000;
    int v = LOW;

    while (running)
    {
        pthread_mutex_lock(&ex9_mut_b);

        v = pin_invert(v);
        digitalWrite(LED_B, v);

        timespec_add_usec(&wakeup, b_period_us);
        usleep(b_period_us);

        pthread_mutex_unlock(&ex9_mut_g);

        v = pin_invert(v);
        digitalWrite(LED_B, v);

    }
}



/*************************************************************
 *************************************************************
 **                                                         **
 **         EXPORTED FUNCTIONS BELOW!                       **
 **                                                         **
 *************************************************************
 *************************************************************/


/****** DON'T TOUCH THESE FUNCTIONS ******/
void  kill_led_threads()    { running = 0; }
int   want_realtime()       { return REALTIME_THREADS; }
int   milliseconds_to_run() { return RUNTIME_MILLSEC; }

void    get_rt_prios(int * p_r, int * p_g, int * p_b)
{
    if (p_r == NULL || p_g == NULL || p_b == NULL)
    {
        printf("get_rt_prios(): One or more arguments was NULL.\n");
    }
    *p_r = RT_PRIO_R;
    *p_g = RT_PRIO_G;
    *p_b = RT_PRIO_B;
}
/****** DON'T TOUCH THESE FUNCTIONS ******/


void user_init()
{
    running = 1;
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);

    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_init();         break;
    case 2:     ex2_init();         break;
    case 3:     ex3_init();         break;
    case 4:     ex4_init();         break;
    case 5:     ex5_init();         break;
    case 6:     ex6_init();         break;
    case 7:     ex7_init();         break;
    case 8:     ex8_init();         break;
    case 9:     ex9_init();         break;
    default:    fprintf(stderr,
                        "ERROR: Invalid exercise number: %d\n",
                        EXERCISE_TO_RUN);
                exit(EXIT_FAILURE);
                break;
    }
}

void * run_red(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_red(arg);       break;
    case 2:     ex2_red(arg);       break;
    case 3:     ex3_red(arg);       break;
    case 4:     ex4_red(arg);       break;
    case 5:     ex5_red(arg);       break;
    case 6:     ex6_red(arg);       break;
    case 7:     ex7_red(arg);       break;
    case 8:     ex8_red(arg);       break;
    case 9:     ex9_red(arg);       break;
    default:                        break;
    }
}

void * run_green(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_green(arg);     break;
    case 2:     ex2_green(arg);     break;
    case 3:     ex3_green(arg);     break;
    case 4:     ex4_green(arg);     break;
    case 5:     ex5_green(arg);     break;
    case 6:     ex6_green(arg);     break;
    case 7:     ex7_green(arg);     break;
    case 8:     ex8_green(arg);     break;
    case 9:     ex9_green(arg);     break;
    default:                        break;
    }
}

void * run_blue(void *arg)
{
    int target_core = highest_core_number();
    stick_this_thread_to_core(target_core);
    switch (EXERCISE_TO_RUN)
    {
    case 1:     ex1_blue(arg);      break;
    case 2:     ex2_blue(arg);      break;
    case 3:     ex3_blue(arg);      break;
    case 4:     ex4_blue(arg);      break;
    case 5:     ex5_blue(arg);      break;
    case 6:     ex6_blue(arg);      break;
    case 7:     ex7_blue(arg);      break;
    case 8:     ex8_blue(arg);      break;
    case 9:     ex9_blue(arg);      break;
    default:                        break;
    }
}

