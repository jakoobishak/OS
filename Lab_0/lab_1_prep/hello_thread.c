#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "timespec.c"

int running = 1;

void * fun_1(void * arg)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
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

int main()
{
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, fun_1, NULL);
    pthread_create(&t2, NULL, fun_2, NULL);
    usleep(500000); // 500 milliseconds
    running = 0; // stop the while loops
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("\nBoth threads are done!\n");
    return EXIT_SUCCESS;
}
