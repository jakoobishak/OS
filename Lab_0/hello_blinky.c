#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>
#define LED_PIN 29 // where was it connected?

void * fun_3(void * arg)
{
    useconds_t sleep_prd = 1000 * 50; // 50 millisec
    int running = 1;
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
    /* calls to pthread_create() and pthread_join() */
    // printf("\nBoth threads are done!\n");
    printf("\nAll three threads are done!\n");
}
