#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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
#define RUNTIME_MILLSEC     5000
#define REALTIME_THREADS    0

// run `chrt -m` to learn allowed values (SCHED_FIFO)
#define RT_PRIO_R           -1
#define RT_PRIO_G           -1
#define RT_PRIO_B           -1

#define EXERCISE_TO_RUN     1

int running;  // leave this alone


/*************************************************************
 *************************************************************
 **               PREPARATORY EXERCISE                      **
 *************************************************************
 *************************************************************/
void timespec_add_usec(struct timespec *ts, long microseconds);

void timespec_add_usec(struct timespec *ts, long us)
{
    /* Insert your solution here */
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
    useconds_t green_sleep_us = 1000 * 1000; // 1s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_R, v);
        usleep(green_sleep_us);
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
    useconds_t green_sleep_us = 200 * 1000; // 0.2s
    int v = LOW;
    while(running) {
        v = pin_invert(v);
        digitalWrite(LED_B, v);
        usleep(green_sleep_us);
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
    while (running)
    {
        sleep(1);
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
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex3_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex3_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
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
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex4_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex4_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **               EXERCISE 5 - Properly Periodical          **
 *************************************************************
 *************************************************************/

void   ex5_init()
{
}

void * ex5_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex5_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex5_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}




/*************************************************************
 *************************************************************
 **               EXERCISE 6 - Real-Time Scheduling         **
 *************************************************************
 *************************************************************/

void   ex6_init()
{
}

void * ex6_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex6_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex6_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}


/*************************************************************
 *************************************************************
 **               EXERCISE 7 - CPU Overloading              **
 *************************************************************
 *************************************************************/

void   ex7_init()
{
}

void * ex7_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex7_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex7_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}



/*************************************************************
 *************************************************************
 **         EXERCISE 8 - [Bonus] Resource Protection        **
 *************************************************************
 *************************************************************/

void   ex8_init()
{
}

void * ex8_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex8_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex8_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}


/*************************************************************
 *************************************************************
 **         EXERCISE 9 - [Bonus] Control Flow               **
 *************************************************************
 *************************************************************/

void   ex9_init()
{
}

void * ex9_red(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex9_green(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
}

void * ex9_blue(void * arg)
{
    while (running)
    {
        sleep(1);
    }
    return NULL;
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

