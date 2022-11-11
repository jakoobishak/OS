// $ gcc phil.c -lpthread -o phil.bin

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "millisleep.h" // Lets us use human readable numbers

int phil_count;

pthread_mutex_t * fork_arr;

void * phil(void * arg)
{
    int nbr = *((int *) arg); // copy the arg from the heap to the stack
    free(arg);

    int servings_wanted = 4 + (rand() % 6);
    int t_ms = 500; // time to sleep, milliseconds

    int left = nbr;
    int right = (nbr+1) % phil_count;

    int first;
    int second;

    if (nbr % 2 == 0)
    {
        first = right;
        second = left;
    }
    else
    {
        first = left;
        second = right;
    }

    printf("p%d was seated. Wants %d portions.\n", nbr, servings_wanted);

    while (servings_wanted)
    {

        printf("p%d grabbing fork 1.\n", nbr);
        pthread_mutex_lock(&fork_arr[first]);
        printf("p%d acquired fork 1.\n", nbr);
        msleep(t_ms);

        printf("p%d grabbing fork 2.\n", nbr);
        pthread_mutex_lock(&fork_arr[second]);
        printf("p%d acquired fork 2.\n", nbr);
        msleep(t_ms);

        printf("p%d eats and thinks.\n", nbr);
        msleep(4 * t_ms);
        servings_wanted--;

        printf("p%d  returns fork 1.\n", nbr);
        pthread_mutex_unlock(&fork_arr[first]);
        printf("p%d  returns fork 2.\n", nbr);
        pthread_mutex_unlock(&fork_arr[second]);

        // wait a while until next bite
        printf("p%d thinks some more.\n", nbr);
        msleep(3 * t_ms);
        if (servings_wanted)
        {
            printf("p%d wants %d more servings.\n", nbr, servings_wanted);
        }
        else
        {
            printf("p%d is full and leaves the table.\n", nbr);
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    printf("To kill a program that's stuck, press CTRL+C\n\n");
    phil_count = 5;
    pthread_t phil_arr[phil_count];
    fork_arr = malloc( phil_count * sizeof(pthread_mutex_t));

    srand(time(NULL));


    /***** Set the table *****/
    for (int i = 0; i < phil_count; i++)
    {
        pthread_mutex_init(&fork_arr[i], NULL);
    }


    /***** Seat the guests *****/
    for (int i = 0; i < phil_count; i++)
    {
        // Since C is an amazing language:
        // 'i' is on the stack. We need to place a copy of it on the heap.
        int * p_nbr = malloc(sizeof(int));
        *p_nbr = i; // A '*' eliminates a 'p'

        //            (  *pthread_t, attr,  fun,           *arg)
        pthread_create(&phil_arr[i], NULL, phil, (void *) p_nbr);
    }


    /***** Wait for the guests to finish *****/
    for (int i = 0; i < phil_count; i++)
    {
        pthread_join(phil_arr[i], NULL);
    }


    /***** Clear the table *****/
    for (int i = 0; i < phil_count; i++)
    {
        pthread_mutex_destroy(&fork_arr[i]);
    }


    printf("Dinner is over!\n");
    return EXIT_SUCCESS;
}


