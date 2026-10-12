Mutex and Conditional Macros
============================

Is required to be included with ``common.h`` (see :doc:`common`) and ``error.h``
(see :doc:`error`)

``locking.h`` is designed to extend the :manpage:`pthreads(7)` functions for
conjunction with ``error.h``.  These macros and function wrap the
:manpage:`pthreads(7)` locking and conditional api with :macro:`ASSERT_PTHREAD`
ensuring that mutex operations return a ``0`` code signalling non-error and
exitting the program if ``-1`` is returned.

Locking Macros
--------------

The following locking macros have a uniform style. Each macro has 2 variants for
read/write mutexes denoted by the argument ``rw_mutex``. The read and write
macros are to be used with ``pthread_rwlock_t`` mutexes, where as the standard
locks use ``pthread_mutex_t``.

.. macro::
   _LOCK(mutex)
   _LOCKR(rw_mutex)
   _LOCKW(rw_mutex)

   These wrap the mutex locking functions ensuring the mutex effectively
   captures the lock.

.. macro::
   _UNLOCK(mutex)
   _UNLOCKRW(rw_mutex)

   These wrap the mutex unlocking functions ensuring the mutex effectively
   releases the lock.

..  macro::
    WITH_MUTEX(mutex)
    WITH_MUTEX_R(rw_mutex)
    WITH_MUTEX_W(rw_mutex)

    This macro wraps the statement or block of code that follows with calls to
    the corresponding :macro:`_LOCK` and :macro:`_UNLOCK` macros.  The
    locking/unlocking calls checked for success and an assertion failure is
    raised if an error is detected. This macro is used as follows::


        WITH_MUTEX(mutex)
        {
            statements;
        }

    is equivalent to::

        {
            _LOCK(mutex)
            statements;
            _UNLOCK(mutex)
        }

    ..  warning::

        Do **not** exit the guarded block with ``break`` or ``return``.

..  macro::
    error__t ERROR_WITH_MUTEX(mutex, error)
    error__t ERROR_WITH_MUTEX_R(rw_mutex, error)

    This wraps mutex locking around an error expression using
    :macro:`_ERROR_WITH_ENTER_LEAVE`, and evaluates to the result of evaluating
    `error` under the lock, which must be an expression of type
    :type:`error__t`.

Locking Functions
-----------------

..  function:: void pwait_initialise(pthread_cond_t *signal)

    Initialises the conditional attribute to a use a `CLOCK_MONOTONIC` clock.
    Any failure to initialise ``signal`` results in the unconditional
    termination of the program.

.. warning::

    Any previous content in `signal` is overwritten therefore must not already
    be in use.

..  function:: void compute_deadline(const struct timespec *timeout,
        clockid_t clock_id, struct timespec *deadline)

   Convert a relative timeout into an absolute deadline.

   The current time on the clock ``clock_id`` is read and ``timeout`` is
   added to it. Ensure ``clock_id`` is the same clock that ``signal`` uses,
   which is `CLOCK_MONOTONIC` when ``signal`` is initialised with
   :func:`pwait_initialise`.

..  function:: bool pwait_deadline(pthread_mutex_t *mutex,
        pthread_cond_t *signal, const struct timespec *deadline)

    Wait on a condition variable until it is signalled or an absolute deadline
    passes. Utilises the :manpage:`pthread_cond_timedwait(3)` api. Returns
    ``true`` if the wait ended because the condition variable was signalled, or
    ``false`` if the deadline passed. Errors (excluding timeouts) from
    :manpage:`pthread_cond_timedwait(3)` results in the program exitting
    unconditionally.

..  function:: bool pwait_timeout(
        pthread_mutex_t *mutex, pthread_cond_t *signal,
        const struct timespec *timeout, clockid_t clock_id);

    Performs a timed wait on a given (mutex, signal) pair.  Must be called with
    the mutex held, returns true if the signal was received, false if a timeout
    intervened. Errors (excluding timeouts) from
    :manpage:`pthread_cond_timedwait(3)` results in the program exitting.


Auxilliary Macros
------------------

.. macro::
   WAIT(mutex, signal)

   Equivalent to ``ASSERT_PTHREAD(pthread_cond_wait(&(signal)), &(mutex))``.
   An erroneous result from :manpage:`pthread_cond_timedwait(3)` results in the
   program exitting unconditionally and does not return.

.. macro::
   SIGNAL(signal)

   Equivalent to ``ASSERT_PTHREAD(pthread_cond_signal(&(signal)), &(mutex))``.
   An erroneous result from :manpage:`pthread_cond_timedwait(3)` results in the
   program exitting unconditionally and does not return.

.. macro::
   BROADCAST(signal)

   Equivalent to ``ASSERT_PTHREAD(pthread_cond_broadcast(&(signal)), &(mutex))``
   .  An erroneous result from :manpage:`pthread_cond_timedwait(3)` results in
   the program exitting unconditionally and does not return.

Pitfalls
--------

The functions and macros relating to timeouts and conditionals only asserts the
the underlying API call does not return an error value. This however does not
assert that the condition has been met, see :manpage:`pthread_cond_wait(3)` for
more information.
