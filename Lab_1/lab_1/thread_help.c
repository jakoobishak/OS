#define _GNU_SOURCE

#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


int highest_core_number()
  {
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    return (num_cores - 1);
}

int stick_this_thread_to_core(int core_id)
{
    int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (core_id < 0 || core_id >= num_cores)
    {
        return EINVAL;
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}


/*
 * These tests did NOT use Real Time.
 * They were done on an RPi 400
 *
 * delay_factor_ms =    1 ====>    1.0 ~    1.4 ms
 * delay_factor_ms =    5 ====>    5.1 ~    6.1 ms
 * delay_factor_ms =  100 ====>  102.0 ~  107.6 ms
 * delay_factor_ms = 2000 ====> 2013.2 ~ 2027.2 ms
 *
 * The RPi 4 has some errors (~50%) for 
 * 	1 <= delay_factor_ms <= 4
 * But for delay_factor_ms >= 6 it's very good.
 */

#define LOOP_MULT_RPI_400 900     // RPi 400 (1.8 GHz)
#define LOOP_MULT_RPI_4   750     // RPi 4   (1.5 GHz)

void cpu_stress(const uint32_t delay_factor_ms)
{
    uint32_t loop_count = delay_factor_ms * LOOP_MULT_RPI_400;
    for (int i = 0; i < loop_count; i++)
    {
        // let's fuckin goooooooooooooooooooooooooooooooooooo
        __asm volatile(
            "\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            nop\nnop\nnop\nnop\nnop\n nop\nnop\nnop\nnop\nnop\n\
            \
            \
            \
            "
        );
    }
}


