/* Helper functions and related utilities. */

#ifndef _COMMON_H
#define _COMMMON_H 1


/* Routines to write informative message or error to stderr or syslog. */
void log_message(const char *message, ...)
    __attribute__((format(printf, 1, 2)));
void log_error(const char *message, ...)
    __attribute__((format(printf, 1, 2)));
void vlog_message(int priority, const char *format, va_list args);

/* Controls whether to emit log_message() output. */
void verbose_logging(bool verbose);
/* Controls whether to print timestamps on logged message output. */
void timestamp_logging(bool timestamps);
/* Once this has been called all logged message will be sent to syslog. */
void start_logging(const char *ident);


/* A dance for generating unique local identifiers.  This involves a number of
 * tricky C preprocessor techniques, and uses the gcc __COUNTER__ extension. */
#define _CONCATENATE(a, b)  a##b
#define CONCATENATE(a, b)   _CONCATENATE(a, b)
#define UNIQUE_ID()         CONCATENATE(_eid__, __COUNTER__)

/* For ignoring return values even when warn_unused_result is in force. */
#define IGNORE(e)       do if(e) {} while (0)

/* A tricksy compile time bug checking macro modified from the kernel.  Causes a
 * compiler error if e doesn't evaluate to true (a non-zero value). */
#define COMPILE_ASSERT(e)           ((void) sizeof(struct { int:-!(e); }))

/* For a static version we drop the COMPILE_ASSERT() expression into a
 * discardable anonymous function. */
#define _id_STATIC_COMPILE_ASSERT(f, e) \
    static inline void f(void) { COMPILE_ASSERT(e); }
#define STATIC_COMPILE_ASSERT(e)    _id_STATIC_COMPILE_ASSERT(UNIQUE_ID(), e)

/* A tricksy macro dance so that we can have a macro with a default argument.
 * Calling GET_DEFAULT(a, b) returns b while GET_DEFAULT(a) returns a. */
#define _ARG2(x, y, extra...)   y
#define GET_DEFAULT(def, arg...)    _ARG2(!, ##arg, def)


/* A loop for counting down: surprisingly tricksy for something so simple.
 * Counts i from n-1 downto 0. */
#define _id_FOR_DOWN_FROM(loop, i, n) \
    for (unsigned int i = n; ( { bool loop = i > 0; i --; loop; } ); )
#define FOR_DOWN_FROM(i, n) \
    _id_FOR_DOWN_FROM(UNIQUE_ID(), i, n)


/* Generic squaring function. */
#define _id_SQR(temp, x)    ( { typeof(x) temp = (x); temp * temp; } )
#define SQR(x)  _id_SQR(UNIQUE_ID(), x)

/* Type aware calloc. */
#define CALLOC(type, nelm)      (type *) calloc(nelm, sizeof(type))

/* Return size of array */
#define ARRAY_SIZE(a)   (sizeof(a)/sizeof((a)[0]))

/* Use this to mark functions that can be constant folded, ie depend only on
 * their arguments and global state. */
#define _pure __attribute__((pure))


/* Casting from one type to another with checking via a union.  Needed in
 * particular to reassure the compiler about aliasing. */
#define _id_CAST_FROM_TO(_union, from_type, to_type, value) \
    ( { \
        COMPILE_ASSERT(sizeof(from_type) == sizeof(to_type)); \
        union { \
            from_type _value; \
            to_type _cast; \
        } _union = { ._value = (value) }; \
        _union._cast; \
    } )
#define CAST_FROM_TO(args...) \
    _id_CAST_FROM_TO(UNIQUE_ID(), args)

#define CAST_TO(to_type, value) CAST_FROM_TO(typeof(value), to_type, value)

/* A macro for ensuring that a value really is assign compatible to the
 * requested type.  Note that due to restrictions on syntax this won't work if
 * type is a written out function type, as in that case the [] part needs to be
 * inside the type definition! */
#define ENSURE_TYPE(type, value)    (*(type []) { (value) })


/* A couple of handy macros: macro safe MIN and MAX functions. */
#define _MIN(tx, ty, x, y) \
    ( { typeof(x) tx = (x); typeof(y) ty = (y); tx < ty ? tx : ty; } )
#define _MAX(tx, ty, x, y) \
    ( { typeof(x) tx = (x); typeof(y) ty = (y); tx > ty ? tx : ty; } )
#define MIN(x, y)   _MIN(UNIQUE_ID(), UNIQUE_ID(), x, y)
#define MAX(x, y)   _MAX(UNIQUE_ID(), UNIQUE_ID(), x, y)


/* Casts a member of a structure out to the containing structure. */
#define _id_container_of(mptr, ptr, type, member) \
    ( { \
        typeof(((type *)0)->member) *mptr = (ptr); \
        (type *)((void *) mptr - offsetof(type, member)); \
    } )
#define container_of(args...)   _id_container_of(UNIQUE_ID(), args)
        

/* Debug utility for dumping binary data in ASCII format. */
void dump_binary(FILE *out, const void *buffer, size_t length);

#endif