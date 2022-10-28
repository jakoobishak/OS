#define _GNU_SOURCE

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>

#include "lab_1.h"
#include "thread_help.h"



void halting_function()
{
    int ttl_ms = milliseconds_to_run();
    
    if (ttl_ms > 0) // run for ttl_ms milliseconds
    {
        // 1000 millisecond = 1 second
        printf("Running for %d ms.\n", ttl_ms);
        usleep(ttl_ms* 1000); // because us
        kill_led_threads();
    }
    else if (ttl_ms == 0) // 0 = want keyboard instead
    {
        printf("Press RETURN to end the program.\n");
        getchar(); // will block waiting for ENTER
        kill_led_threads();
    }
    else // less than 0 --> do nothing
    {
    }
}

void time_test(int millisec_attempt)
{
    struct timespec t0;
    struct timespec t1;

    clock_gettime(CLOCK_REALTIME, &t0);

    cpu_stress(millisec_attempt);

    clock_gettime(CLOCK_REALTIME, &t1);

    if (t1.tv_nsec <= t0.tv_nsec)
    {
        t1.tv_sec  -= 1;
        t1.tv_nsec += 1000000000;
    }
    long   diff_nsec = t1.tv_nsec - t0.tv_nsec;
    time_t diff_sec  = t1.tv_sec  - t0.tv_sec;
    printf("   %d.%09d seconds\n", diff_sec, diff_nsec);
}


int main(void)
{
    if ( 0 ) /* code for cpu_stress() calibration */
    {
        // stick_this_thread_to_core( highest_core_number() );
        for (int i = 0; i < 10; i++)
        {
            time_test(   6);
        }
        exit(EXIT_SUCCESS);
    }

    wiringPiSetup();

    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    user_init();     // user code

    pthread_t           blink_thread_r;
    pthread_t           blink_thread_g;
    pthread_t           blink_thread_b;

    pthread_attr_t      blink_attr_r;
    pthread_attr_t      blink_attr_g;
    pthread_attr_t      blink_attr_b;

    pthread_attr_init(&blink_attr_r); // fill attr with default values
    pthread_attr_init(&blink_attr_g);
    pthread_attr_init(&blink_attr_b);

    if ( want_realtime() )    // change attribute to start RT threads
    {
        if ( getuid() != 0 ) // Are you root? (needed for RT threads)
        {
            printf("To run this program with RT threads you need admin ");
            printf("privileges. Please try running again, but with 'sudo'.\n");
            printf("Exiting...\n");
            exit(EXIT_FAILURE);
        }
        struct sched_param param_r;
        struct sched_param param_g;
        struct sched_param param_b;

        int           prio_r,  prio_g,  prio_b;
        get_rt_prios(&prio_r, &prio_g, &prio_b);

        param_r.sched_priority = prio_r;
        param_g.sched_priority = prio_g;
        param_b.sched_priority = prio_b;

        pthread_attr_setinheritsched(&blink_attr_r, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setinheritsched(&blink_attr_g, PTHREAD_EXPLICIT_SCHED);
        pthread_attr_setinheritsched(&blink_attr_b, PTHREAD_EXPLICIT_SCHED);

        pthread_attr_setschedpolicy(&blink_attr_r, SCHED_FIFO);
        pthread_attr_setschedpolicy(&blink_attr_g, SCHED_FIFO);
        pthread_attr_setschedpolicy(&blink_attr_b, SCHED_FIFO);

        pthread_attr_setschedparam(&blink_attr_r, &param_r);
        pthread_attr_setschedparam(&blink_attr_g, &param_g);
        pthread_attr_setschedparam(&blink_attr_b, &param_b);
    }

    //               os_thread_var,          attr, thread_fun, arg
    pthread_create(&blink_thread_r, &blink_attr_r, run_red,    NULL);
    pthread_create(&blink_thread_g, &blink_attr_g, run_green,  NULL);
    pthread_create(&blink_thread_b, &blink_attr_b, run_blue,   NULL);

    pthread_setname_np(blink_thread_r, "RED_thread");
    pthread_setname_np(blink_thread_g, "GRN_thread");
    pthread_setname_np(blink_thread_b, "BLU_thread");

    // printf("thread R pid: %u\n", blink_thread_r);
    // printf("thread G pid: %u\n", blink_thread_g);
    // printf("thread B pid: %u\n", blink_thread_b);

    halting_function();

    pthread_join(blink_thread_r, NULL);
    pthread_join(blink_thread_g, NULL);
    pthread_join(blink_thread_b, NULL);

    // Less nice but faster method (same as kill -9)
    // pthread_kill(blink_thread_r, SIGKILL);
    // pthread_kill(blink_thread_g, SIGKILL);
    // pthread_kill(blink_thread_b, SIGKILL);

    exit(EXIT_SUCCESS);
}

