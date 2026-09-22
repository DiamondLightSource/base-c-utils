/* Helper functions for lock support. */

#ifndef _LOCKING_H
#define _LOCKING_H 1


/* Condition waiting and signalling. */
#define WAIT(mutex, signal) \
    ASSERT_PTHREAD(pthread_cond_wait(&(signal), &(mutex)))
#define SIGNAL(signal)      ASSERT_PTHREAD(pthread_cond_signal(&(signal)))
#define BROADCAST(signal)   ASSERT_PTHREAD(pthread_cond_broadcast(&(signal)))


/* A handy macro for timeouts in ns. */
#define NSECS   1000000000      // 1e9

/* pthread condition timed wait.  To be completely safe against clock
 * misbehaviour we also have to initialise the condition specially. */
void pwait_initialise(pthread_cond_t *signal);

/* Performs timed wait on given (mutex,signal) pair.  Must be called as usual
 * with the mutex held, returns true if the signal was received, false if a
 * timeout intervened. */
bool pwait_timeout(
    pthread_mutex_t *mutex, pthread_cond_t *signal,
    const struct timespec *timeout, clockid_t clock_id);

/* As for pwait_timeout, but in this case the timeout is an absolute time. */
bool pwait_deadline(
    pthread_mutex_t *mutex, pthread_cond_t *signal,
    const struct timespec *deadline);

/* Computes deadline from timeout. */
void compute_deadline(
    const struct timespec *timeout, clockid_t clock_id, struct timespec *deadline);

#endif 
