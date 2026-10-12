/* Helper functions for lock support. */

#ifndef _LOCKING_H
#define _LOCKING_H 1

/* Basic mutex lock/unlock. */
#define _LOCK(mutex)     ASSERT_PTHREAD(pthread_mutex_lock(&(mutex)))
#define _UNLOCK(mutex)   ASSERT_PTHREAD(pthread_mutex_unlock(&(mutex)))

/* Condition waiting and signalling. */
#define WAIT(mutex, signal) \
    ASSERT_PTHREAD(pthread_cond_wait(&(signal), &(mutex)))
#define SIGNAL(signal)      ASSERT_PTHREAD(pthread_cond_signal(&(signal)))
#define BROADCAST(signal)   ASSERT_PTHREAD(pthread_cond_broadcast(&(signal)))

/* Read and write locks. */
#define _LOCKR(mutex)    ASSERT_PTHREAD(pthread_rwlock_rdlock(&(mutex)))
#define _LOCKW(mutex)    ASSERT_PTHREAD(pthread_rwlock_wrlock(&(mutex)))
#define _UNLOCKRW(mutex) ASSERT_PTHREAD(pthread_rwlock_unlock(&(mutex)))

/* Wrapper around mutex lock/unlock. */
#define WITH_MUTEX(mutex)   _WITH_ENTER_LEAVE(_LOCK(mutex),  _UNLOCK(mutex))
#define WITH_MUTEX_R(mutex) _WITH_ENTER_LEAVE(_LOCKR(mutex), _UNLOCKRW(mutex))
#define WITH_MUTEX_W(mutex) _WITH_ENTER_LEAVE(_LOCKW(mutex), _UNLOCKRW(mutex))

/* Wraps a mutex call around the calculation of error, returns the error code
 * result. */
#define ERROR_WITH_MUTEX(mutex, error) \
    _ERROR_WITH_ENTER_LEAVE(_LOCK(mutex), _UNLOCK(mutex), error)

#define ERROR_WITH_MUTEX_R(mutex, error) \
    _ERROR_WITH_ENTER_LEAVE(_LOCKR(mutex), _UNLOCKRW(mutex), error)

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
