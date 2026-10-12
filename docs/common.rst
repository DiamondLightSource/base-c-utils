Common Macro Definitions
========================

The header ``common.h`` is the backbone of the other functionality in this 
library. It is composed of logging functionality, frequently used helpful 
macros and macros with no other natural home.

Message Logging Control
-----------------------

By default all error messages are sent to ``stderr``, but syslog can be used
instead by calling :func:`start_logging`.

..  function:: void vlog_message(int priority, const char *format, va_list args)

    Sends the given message to ``stderr`` or to syslog, depending on whether
    :func:`start_logging` has been called.

..  function::
    void log_message(const char *message, ...)
    void log_error(const char *message, ...)

    Calls :func:`vlog_message` with with `priority` set to ``LOG_INFO`` for
    :func:`log_message`, and ``LOG_ERR`` for :func:`log_error`.

    Note that :func:`error_report` uses :func:`log_error`.

..  function:: void verbose_logging(bool verbose)

    This controls wether to output emit the output from :func:`log_message`.

..  function:: void timetamp_logging(bool timestamps)

    Add timestamps to all logged output. The output is in outputted in the
	format: ``YYYY-MM-DD hh-mm-ss.usec``.

..  function:: void start_logging(const char *ident)

    This invokes :func:`!openlog` (3) and sends all future messages to the
    system log with the log identifier `ident`.


Miscellaneous Helpers
---------------------

These macros have no other natural home and have found their place in this
header file.

..  macro:: size_t ARRAY_SIZE(type array[])

    If the number of elements of `array` is known at compile time this macro
    returns the number of elements.

..  macro:: to_type CAST_FROM_TO(from_type, to_type, value)

    In some situations the compiler will not accept an ordinary C cast of the
    form ``(type) value`` because of anxieties about aliasing, or if a ``const``
    attribute needs to be removed, or if some other low level bit preserving
    conversion is required.  This macro performs this cast in a more compiler
    friendly manner (via a ``union`` type), and checks that `value` has type
    `from_type` and that `to_type` and `value` have the same size.

    For example, this macro is used to remove the ``const`` attribute from a
    hashtable key in ``hashtable.c`` thus (here :func:`!release_key` takes a
    ``void *`` argument)::

        static void release_key(struct hash_table *table, const void *key)
        {
            table->key_ops->release_key(
                CAST_FROM_TO(const void *, void *, key));
        }

    Another application is the following which extracts the bit pattern of a
    floating point number as an integer::

        uint32_t bit_pattern = CAST_FROM_TO(float, uint32_t, 0.1F);

..  macro:: to_type CAST_TO(to_type, value)

    This is a short-cut wrapper for :macro:`CAST_FROM_TO` for use in the case
    when `from_type` is no more specific than ``typeof(value)``.

..  macro:: type ENSURE_TYPE(type, value)

    This is a weak cast from `value` to `type` which ensures that it is valid to
    assign `value` to this `type`.  Note that this will not work if `type` is a
    written out function type, in this case a typedef name would have to be
    used.

..  macro:: IGNORE(expr)

    Discards a return value without compiler warning even when
    ``warn_unused_result`` is in force.

..  macro:: unlikely(expr)

    Provides a hint to the compiler that ``expr`` is likely to be ``0``, can
    help in the optimisation of very rarely taken branches.

..  macro::
    COMPILE_ASSERT(expr)
    STATIC_COMPILE_ASSERT(expr)

    This macro forces a compile time error if `expr` evaluates to ``false`` at
    compile time.  :macro:`COMPILE_ASSERT` must be used inside a function
    declaration, while :macro:`STATIC_COMPILE_ASSERT` must be used at the top
    declaration level.

..  macro::
    MIN(x, y)
    MAX(x, y)

    Macro safe minimum and maximum functions.

..  macro:: SQR(x)

	Macro safe squaring function.

..  macro:: container_of(ptr, type, member)

    Casts a member of a structure out to the containing structure.  For example,
    given `py` constructed thus::

        struct xy { int x, y; } xy;
        int *py = &xy.y;

    a pointer to `xy` can be reconstructed as::

        struct xy *pxy = container_of(py, struct xy, y);

..   macro:: FOR_DOWN_FROM(i, n)

    Iterates through a loop backwards. Counts ``i`` down from ``n-1`` to 0. For
    example::

        FOR_DOWN_FROM(i, 10)
        {
           printf("%d ", i);
        }
        print("\n");

    Will print ``9 8 ... 2 1 0``.

..   macro:: type* CALLOC(type, nelm)

    Type aware calloc. Functionally equivalent to
    ``(type *)calloc(nelm * sizeof(type))``


Debug Helpers
-------------

.. function:: void dump_binary(FILE *out, const void *buffer, size_t length)

   Dumps binary data from buffer into ``out`` in ASCII format.
