#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <syslog.h>
#include <stdio.h>
#include <execinfo.h>
#include <stdarg.h>

#include "common.h"

/* -------------------------------------------------------------------------- */

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static bool daemon_mode = false;
static bool log_verbose = true;
/* Timestamp each log message. */
static bool log_timestamps = false;

void verbose_logging(bool verbose)
{
    log_verbose = verbose;
}


void timestamp_logging(bool timestamps)
{
    log_timestamps = timestamps;
}


void start_logging(const char *ident)
{
    openlog(ident, 0, LOG_DAEMON);
    daemon_mode = true;
}


static void print_timestamp(struct timespec *timestamp)
{
    /* Convert ns into microseconds, the extra ns detail is a bit much. */
    uint32_t usec = (uint32_t) (timestamp->tv_nsec + 500) / 1000;
    if (usec >= 1000000)
    {
        usec -= 1000000;
        timestamp->tv_sec += 1;
    }

    /* Print the result in local time. */
    struct tm tm;
    localtime_r(&timestamp->tv_sec, &tm);

    fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d.%06u: ",
        1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec, usec);
}


void vlog_message(int priority, const char *format, va_list args)
{
    /* Get the timestamp before entering the lock for more honest times. */
    struct timespec now;
    if (log_timestamps)
        clock_gettime(CLOCK_REALTIME, &now);

    pthread_mutex_lock(&log_mutex);
    if (daemon_mode)
        vsyslog(priority, format, args);
    else
    {
        if (log_timestamps)
            print_timestamp(&now);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
    }
    pthread_mutex_unlock(&log_mutex);
}


void log_message(const char *message, ...)
{
    if (log_verbose)
    {
        va_list args;
        va_start(args, message);
        vlog_message(LOG_INFO, message, args);
        va_end(args);
    }
}


void log_error(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vlog_message(LOG_ERR, message, args);
    va_end(args);
}


void dump_binary(FILE *out, const void *buffer, size_t length)
{
    const uint8_t *dump = buffer;

    for (size_t a = 0; a < length; a += 16)
    {
        fprintf(out, "%08zx: ", a);
        for (unsigned int i = 0; i < 16; i ++)
        {
            if (a + i < length)
                fprintf(out, " %02x", dump[a+i]);
            else
                fprintf(out, "   ");
            if (i % 16 == 7)
                fprintf(out, " ");
        }

        fprintf(out, "  ");
        for (unsigned int i = 0; i < 16; i ++)
        {
            uint8_t c = dump[a+i];
            if (a + i < length)
                fprintf(out, "%c", 32 <= c  &&  c < 127 ? c : '.');
            else
                fprintf(out, " ");
            if (i % 16 == 7)
                fprintf(out, " ");
        }
        fprintf(out, "\n");
    }
}

