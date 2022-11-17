#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "wait_period.h"

/* initialize the period by setting the timer statet to the current time */
void wait_period_initialize( struct timespec * timer_state )
{
    clock_gettime( CLOCK_REALTIME, timer_state );

    return;
}

/* wait the specified amount of time relative to the previous period's end point */
void wait_period( struct timespec * timer_state, unsigned long milliseconds )
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

    /* increment the period */
    timer_state->tv_sec  += milliseconds/1000U;
    timer_state->tv_nsec += (milliseconds%1000U) * 1000000;
    timer_state->tv_sec  += timer_state->tv_nsec/1000000000;
    timer_state->tv_nsec  = timer_state->tv_nsec%1000000000;

    /* pthread_cond_timedwait waits until the specified absolute time expires */
    pthread_mutex_lock( &mutex );
    pthread_cond_timedwait( &cond, &mutex, timer_state );
    pthread_mutex_unlock( &mutex );

    return;
}
