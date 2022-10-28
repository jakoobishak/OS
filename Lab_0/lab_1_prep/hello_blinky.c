#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include "timespec.c"

#define LED_PIN 29

int running = 1;

void * fun_1(void * arg)
{
    struct timespec ts;
    while (running)
    {
        printf("X"); // letter X
        timespec_add_usec(&ts, 500);
    }
    return NULL;
}

void * fun_2(void * arg)
{
    while (running)
    {
        printf(" "); // a space char
        usleep(500);
    }
    return NULL;
}

void * fun_3(void * arg)
{
    useconds_t sleep_prd = 1000 * 50; // 50 millisec
    while (running) {
        digitalWrite(LED_PIN, HIGH);
        usleep(sleep_prd);
        digitalWrite(LED_PIN, LOW);
        usleep(sleep_prd);
    }
    return arg;
}

int main()
{
    wiringPiSetup();
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_create(&t1, NULL, fun_1, NULL);
    pthread_create(&t2, NULL, fun_2, NULL);
    pthread_create(&t3, NULL, fun_3, NULL);
    usleep(500000); // 500 milliseconds
    running = 0; // stop the while loops
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printf("\nAll three threads are done!\n");
    return EXIT_SUCCESS;
}
