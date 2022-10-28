#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timespec.h"

void timespec_cpy(struct timespec * target, 
                  struct timespec * src)
{
    target->tv_sec  = src->tv_sec;
    target->tv_nsec = src->tv_nsec;
}


int timespec_eq(struct timespec * a, 
                struct timespec * b)
{
    return (a->tv_sec  == b->tv_sec && 
            a->tv_nsec == b->tv_nsec);
}

int test_pretty(struct timespec * strt, 
                struct timespec * expc,
                struct timespec * rest)
{
    printf("\n");
    printf("               START    EXPECTED      RESULT\n");
    printf("          ----------  ----------  ----------\n");
    // printf("tv_sec   %010l %010l %010l\n", strt->tv_sec, expc->tv_sec, rest->tv_sec);
    printf("tv_sec    %10d  %10d  %10d\n",    strt->tv_sec,  expc->tv_sec,  rest->tv_sec);
    printf("tv_nsec   %010d  %010d  %010d\n", strt->tv_nsec, expc->tv_nsec, rest->tv_nsec);

}


void ts_test_1()
{
    int test_num    = 1;
    long us_to_add  = 1;
    printf("\nts_test_%d(): \n", test_num);
    printf("        Adding %d microseconds \n", us_to_add);
    printf("        the starting time.\n");

    struct timespec ts_start, ts_expect, ts_result;

    ts_start.tv_sec  = 0;
    ts_start.tv_nsec = 0;

    ts_expect.tv_sec  = 0;
    ts_expect.tv_nsec = 1000;
    
    timespec_cpy(&ts_result, &ts_start);

    timespec_add_usec(&ts_result, us_to_add);

    test_pretty(&ts_start, &ts_expect, &ts_result);

    if (!timespec_eq(&ts_result, &ts_expect))
    { 
        printf("Failed at test %d, exiting...\n", test_num);
        exit(EXIT_FAILURE); 
    }
}

void ts_test_2()
{
    int test_num    = 2;
    long us_to_add  = 1;
    printf("\nts_test_%d(): \n", test_num);
    printf("        Adding %d microseconds \n", us_to_add);
    printf("        the starting time (999999 usec).\n");

    struct timespec ts_start, ts_expect, ts_result;

    ts_start.tv_sec   = 0;
    ts_start.tv_nsec  = 999999000;

    ts_expect.tv_sec  = 1;
    ts_expect.tv_nsec = 0;
    
    timespec_cpy(&ts_result, &ts_start);

    timespec_add_usec(&ts_result, us_to_add);

    test_pretty(&ts_start, &ts_expect, &ts_result);

    if (!timespec_eq(&ts_result, &ts_expect))
    { 
        printf("Failed at test %d, exiting...\n", test_num);
        exit(EXIT_FAILURE); 
    }
}

void ts_test_3()
{
    int test_num    = 3;
    long us_to_add  = 2000000;
    printf("\nts_test_%d(): \n", test_num);
    printf("        Adding %d microseconds \n", us_to_add);
    printf("        (2 second) the starting time.\n");

    struct timespec ts_start, ts_expect, ts_result;

    ts_start.tv_sec  = 1;
    ts_start.tv_nsec = 999999000;

    ts_expect.tv_sec  = 3;
    ts_expect.tv_nsec = 999999000;
    
    timespec_cpy(&ts_result, &ts_start);

    timespec_add_usec(&ts_result, us_to_add);

    test_pretty(&ts_start, &ts_expect, &ts_result);

    if (!timespec_eq(&ts_result, &ts_expect))
    { 
        printf("Failed at test %d, exiting...\n", test_num);
        exit(EXIT_FAILURE); 
    }
}


void ts_test_4()
{
    int test_num    = 4;
    long second     = 1000000;
    long minute     = 60 * second;
    long hour       = 60 * minute;
    long half_day   = 12 * hour;
    long three_days = 6 * half_day;

    printf("half_day_us = %u\n", half_day);
    long us_to_add = three_days + half_day;
    printf("\nts_test_%d(): \n", test_num);
    printf("        Adding %d microseconds \n", us_to_add);
    printf("        (3.5 days) to the starting \n");
    printf("        time (JTH openenig in 1994).\n");

    struct timespec ts_start, ts_expect, ts_result;

    ts_start.tv_sec  = 773013600;
    ts_start.tv_nsec = 0;

    ts_expect.tv_sec  = 773316000;
    ts_expect.tv_nsec = 0;
    
    timespec_cpy(&ts_result, &ts_start);

    timespec_add_usec(&ts_result, us_to_add);

    test_pretty(&ts_start, &ts_expect, &ts_result);

    if (!timespec_eq(&ts_result, &ts_expect))
    { 
        printf("Failed at test %d, exiting...\n", test_num);
        exit(EXIT_FAILURE); 
    }

    printf("\n\n");
    printf("              $ tldr date\n");
    printf("       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("       Will help you print your tv_sec\n");
    printf("       (which is a UNIX timestamp)\n");
    printf("       value in human readable form.\n\n");
}



int main(void)
{

    ts_test_1();
    ts_test_2();
    ts_test_3();
    ts_test_4();

    return EXIT_SUCCESS;
}

