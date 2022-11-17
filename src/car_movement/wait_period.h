#ifndef WAIT_PERIOD
#define WAIT_PERIOD

void wait_period_initialize( struct timespec * timer_state );
void wait_period( struct timespec * timer_state, unsigned long milliseconds );

#endif  /* WAIT_PERIOD */
