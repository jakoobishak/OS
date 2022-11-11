
#include <time.h>

/* This is a bad solution, so don't steal it! */
void timespec_add_usec(struct timespec *ts, long microseconds)
{
    const long NS_MAX = 1000000000;
    long nanosec_addition = microseconds * 1000;
    ts->tv_nsec += nanosec_addition;
    while (ts->tv_nsec >= NS_MAX)
    {
        ts->tv_sec  += 1;
        ts->tv_nsec -= NS_MAX;
    }
    return;
}


void msleep(long milliseconds);

void msleep(long milliseconds)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    timespec_add_usec(&t, 1000 * milliseconds); // bcuz adding usec
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &t, NULL);
}



