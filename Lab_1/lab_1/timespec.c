#include "timespec.h"
#include <stdio.h>

#define NANOSECOND 1000000000
#define MILISECOND 1000

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

    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, ts, NULL);
}
