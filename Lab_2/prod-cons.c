
#include <assert.h> 
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <semaphore.h>

#include "millisleep.h"
#include "circular_buffer.h" // this is NOT a thread safe library

#define     BTN_PIN     24

int running           = 1;

cbuf_handle_t           cbuf;
pthread_mutex_t         cbuf_mut;
pthread_cond_t          cbuf_no_longer_full;
pthread_cond_t          cbuf_no_longer_empty;
sem_t                   semaphore;


/***********************************************************
 *************** BUTTON INTERRUPT SUBROUTINE ***************
 ***********************************************************/
void btn_isr()
{
    sem_post(&semaphore);
}


/***********************************************************
 **************** PRODUCER THREAD FUNCTIONS ****************
 ***********************************************************/
void * prod_button_thread(void * arg)
{
    while (running)
    {
        sem_wait(&semaphore);

        pthread_mutex_lock(&cbuf_mut);

        while (circular_buf_full(cbuf))
            pthread_cond_wait(&cbuf_no_longer_full, &cbuf_mut);

        circular_buf_put(cbuf, 'B');
        pthread_cond_signal(&cbuf_no_longer_empty);

        pthread_mutex_unlock(&cbuf_mut);
    }
    return arg;
}


void * prod_timed_thread(void * arg)
{
    /* 
     * Let's generate our sleep times so that they're exponentially
     * distributed with an average time of T_avg.
     */
    float T_avg = 1500.0; // this is (1 / lambda)
    float U, T;

    while (running) 
    {
        pthread_mutex_lock(&cbuf_mut);

        while (circular_buf_full(cbuf))
            pthread_cond_wait(&cbuf_no_longer_full, &cbuf_mut);

        circular_buf_put(cbuf, 'T');
        pthread_cond_signal(&cbuf_no_longer_empty);

        pthread_mutex_unlock(&cbuf_mut);

        // Get a random exponentially distributed delay
        U = (float)rand()/(float)RAND_MAX;
        T = -log(U) * T_avg;
        // Use this to switch to constant delay:
        //T = T_avg;
        msleep(round(T));



    }
    return arg;
}


/**********************************************************
 *************** CONSUMER THREAD FUNCTIONS **************** 
 **********************************************************/
void * cons_timed_thread(void * arg)
{
    uint8_t read_val; // to use with circular_buf_get() 

    float T_avg = 2000.0; // (1 / lambda)
    float U, T;

    while (running)
    {
        pthread_mutex_lock(&cbuf_mut);

        while (circular_buf_empty(cbuf))
            pthread_cond_wait(&cbuf_no_longer_empty, &cbuf_mut);

        circular_buf_get(cbuf, &read_val);

        pthread_cond_signal(&cbuf_no_longer_full);

        pthread_mutex_unlock(&cbuf_mut);

        // Get a random exponentially distributed delay
        U = (float)rand()/(float)RAND_MAX;
        T = -log(U) * T_avg;
        // Use this to switch to constant delay:
        //T = T_avg;
        msleep(round(T));
    }
    return arg;
}


/**********************************************************
 **************** PRINTING THREAD FUNCTION **************** 
 **********************************************************/
void * cbuf_print_fun(void * arg)
{
    circular_buf_print(cbuf);
    while (running)
    {
        printf("\033[3A");
        circular_buf_print(cbuf);
        msleep(100); // 10 Hz refresh rate
    }
    return arg;
}



int main(int argc, char *argv[])
{
    /***** Variable declarations *****/
    pthread_t  pthread_prod_button;
    pthread_t  pthread_prod_timed;
    pthread_t  pthread_cons_timed;
    pthread_t  pthread_cbuf_print;

    sem_init(&semaphore, 0, 0);

    /***** Initialize the circular buffer *****/
    size_t      cb_size  = 16; // feel free to change this
    uint8_t *   cb_array = malloc(cb_size * sizeof(uint8_t));
    cbuf                 = circular_buf_init(cb_array, cb_size);

    /***** Set up pthread variables *****/
    pthread_mutex_init(&cbuf_mut, NULL);
    pthread_cond_init(&cbuf_no_longer_empty, NULL);
    pthread_cond_init(&cbuf_no_longer_full, NULL);

    /***** Set up wiringPi with ISR for buton *****/
    wiringPiSetup();
    pinMode(BTN_PIN, INPUT);
    pullUpDnControl(BTN_PIN, PUD_DOWN);
    wiringPiISR(BTN_PIN, INT_EDGE_RISING, &btn_isr);

    /***** Spawn threads *****/
    pthread_create(&pthread_prod_button, NULL, prod_button_thread, NULL);
    pthread_create(&pthread_prod_timed,  NULL, prod_timed_thread,  NULL);
    pthread_create(&pthread_cons_timed,  NULL, cons_timed_thread,  NULL);
    pthread_create(&pthread_cbuf_print,  NULL, cbuf_print_fun,     NULL);

    /***** Run for 20 seconds *****/
    msleep(20000);

    /***** Stop threads, then join() or kill() them *****/
    running = 0;
    pthread_join(pthread_prod_button, NULL);
    pthread_join(pthread_prod_timed,  NULL);
    pthread_join(pthread_cons_timed,  NULL);
    pthread_join(pthread_cbuf_print,  NULL);

    /***** Return used resources and exit *****/
    pthread_mutex_destroy(&cbuf_mut);
    pthread_cond_destroy(&cbuf_no_longer_empty);
    pthread_cond_destroy(&cbuf_no_longer_full);
    free(cb_array);
    circular_buf_free(cbuf);
    return EXIT_SUCCESS;
}


