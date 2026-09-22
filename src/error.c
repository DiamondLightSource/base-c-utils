/* Generic error handling framework. */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <execinfo.h>

#include "common.h" // Only for log_error

#include "error.h"


/* No more than this many error messages can be nested: more would be insane! */
#define MAX_ERROR_DEPTH     10


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Core error reporting mechanism. */

/* This encapsulates an error message. */
struct error__t {
    /* An error message consists of a list of error strings. */
    int count;
    char *messages[MAX_ERROR_DEPTH];
    /* If error_format has been called the formatted error is stored here. */
    char *formatted;
};


/* Creates an error__t with one or two messages. */
error__t _error_create(char *extra, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char *message;
    ASSERT_IO(vasprintf(&message, format, args));
    va_end(args);

    struct error__t *error = malloc(sizeof(struct error__t));
    if (extra)
        *error = (struct error__t) {
            .count = 2, .messages = { extra, message }, };
    else
        *error = (struct error__t) {
            .count = 1, .messages = { message }, };
    return error;
}


void _error_extend(error__t error, const char *format, ...)
{
    /* Treat error_extend() on ERROR_OK as a no-op. */
    if (error)
    {
        ASSERT_OK(error->count < MAX_ERROR_DEPTH);

        va_list args;
        va_start(args, format);
        ASSERT_IO(vasprintf(&error->messages[error->count], format, args));
        va_end(args);
        error->count += 1;
    }
}


bool error_discard(error__t error)
{
    if (error)
    {
        for (int i = 0; i < error->count; i ++)
            free(error->messages[i]);
        free(error->formatted);
        free(error);
        return true;
    }
    else
        return false;
}


const char *error_format(error__t error)
{
    if (!error)
        return "OK";

    /* We'll simply format the stack of messages with the most recent message
     * first.  Count up the length needed. */
    size_t length = 0;
    for (int i = 0; i < error->count; i ++)
        length += strlen(error->messages[i]) + 2;

    char *result = malloc(length);
    free(error->formatted);      // In case we're not the first
    error->formatted = result;

    int ix = 0;
    for (int i = 0; i < error->count; i ++)
    {
        int n = error->count - 1 - i;
        ix += sprintf(result + ix, n ? "%s: " : "%s", error->messages[n]);
    }
    return result;
}


bool error_report(error__t error)
{
    if (error)
        log_error("%s", error_format(error));
    return error_discard(error);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Two mechanisms for reporting extra error information. */
char *_error_extra_io_errno(int error)
{
    /* This is very annoying: strerror() is not not necessarily thread safe ...
     * but not for any compelling reason, see:
     *  http://sources.redhat.com/ml/glibc-bugs/2005-11/msg00101.html
     * and the rather unhelpful reply:
     *  http://sources.redhat.com/ml/glibc-bugs/2005-11/msg00108.html
     *
     * On the other hand, the recommended routine strerror_r() is inconsistently
     * defined -- depending on the precise library and its configuration, it
     * returns either an int or a char*.  Oh dear.
     *
     * Ah well.  We go with the GNU definition, so here is a buffer to maybe use
     * for the message. */
    char str_error[256];
    const char *error_string = strerror_r(error, str_error, sizeof(str_error));
    char *result;
    ASSERT_IO(asprintf(&result, "(%d) %s", error, error_string));
    return result;
}


char *_error_extra_io(void)
{
    return _error_extra_io_errno(errno);
}


void _error_panic(char *extra, const char *filename, int line)
{
    log_error("Unrecoverable error at %s, line %d", filename, line);
    if (extra)
        log_error("Extra context: %s", extra);
    fflush(stderr);
    fflush(stdout);

    /* Now try and create useable backtrace. */
    void *backtrace_buffer[128];
    int count = backtrace(backtrace_buffer, ARRAY_SIZE(backtrace_buffer));
    backtrace_symbols_fd(backtrace_buffer, count, STDERR_FILENO);
    char last_line[128];
    int char_count = snprintf(last_line, sizeof(last_line),
        "End of backtrace: %d lines written\n", count);
    IGNORE(write(STDERR_FILENO, last_line, (size_t) char_count));

    _exit(255);
}

