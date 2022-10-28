#include <stdint.h>

// These are used for setting CPU affinitiy
// (meaning the CPUs that they get scheduled to)
int   highest_core_number         ();
int   stick_this_thread_to_core   (int core_id);

/***************************************************
 *  This function has been calibrated with a       *
 * Raspberry Pi 400. The parameter delay_factor    *
 * is very close to the amout of milliseconds that *
 * the function will run for.                      *
 ***************************************************/
void   cpu_stress                  (const uint32_t delay_factor_ms);

