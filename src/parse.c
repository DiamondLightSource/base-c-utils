/* Simple parsing support. */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "error.h"

#include "parse.h"



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Simple parsing support. */


void skip_whitespace(const char **string)
{
    while (**string == ' '  ||  **string == '\t')
        *string += 1;
}


/* Expects whitespace and skips it. */
error__t parse_whitespace(const char **string, bool optional)
{
    const char *start = *string;
    skip_whitespace(string);
    return TEST_OK_(optional || *string > start, "Whitespace expected");
}


/* Test for valid character in a name.  We allow ASCII letters and underscores,
 * only. */
static bool valid_name_char(char ch)
{
    return isascii(ch)  &&  (isalpha(ch)  ||  ch == '_');
}

/* Allow numbers as well. */
static bool valid_alphanum_char(char ch)
{
    return isascii(ch)  &&  (isalpha(ch)  ||  ch == '_'  ||  isdigit(ch));
}


static error__t parse_filtered_name(
    const char **string, bool (*filter_char)(char),
    char result[], size_t max_length)
{
    size_t ix = 0;
    while (ix < max_length  &&  filter_char(**string))
    {
        result[ix] = *(*string)++;
        ix += 1;
    }
    return
        TEST_OK_(ix > 0, "No name found")  ?:
        TEST_OK_(ix < max_length, "Name too long")  ?:
        DO(result[ix] = '\0');
}


error__t parse_name(const char **string, char result[], size_t max_length)
{
    return parse_filtered_name(string, valid_name_char, result, max_length);
}


error__t parse_alphanum_name(
    const char **string, char result[], size_t max_length)
{
    return
        TEST_OK_(valid_name_char(**string), "No name found")  ?:
        parse_filtered_name(string, valid_alphanum_char, result, max_length);
}


error__t parse_block_name(
    const char **string, char result[], size_t max_length)
{
    const char *start = *string;
    return
        parse_alphanum_name(string, result, max_length)  ?:
        DO(
            /* Finally remove any trailing digits from the parse. */
            while (isdigit((*string)[-1]))
                *string -= 1;
            result[*string - start] = '\0';
        );
}


bool read_char(const char **string, char ch)
{
    if (**string == ch)
    {
        *string += 1;
        return true;
    }
    else
        return false;
}

char get_char(const char **string)
{
    skip_whitespace(string);
    char result = **string;
    if (result != '\0')
        *string += 1;
    return result;
}

bool read_string(const char **string, const char *expected)
{
    size_t length = strlen(expected);
    if (strncmp(*string, expected, length) == 0)
    {
        *string += length;
        return true;
    }
    else
        return false;
}

error__t parse_char(const char **string, char ch)
{
    return TEST_OK_(read_char(string, ch), "Character '%c' expected", ch);
}


/* Called after a C library conversion function checks that anything was
 * converted and that the conversion was successful.  Relies on errno being zero
 * before conversion started. */
static error__t check_number(const char *start, const char *end)
{
    return
        TEST_OK_(end > start, "Number missing")  ?:
        TEST_OK_IO_(errno == 0, "Error converting number");
}


/* Parsing numbers is rather boilerplate.  This macro encapsulates everything in
 * one common form. */
#define DEFINE_PARSE_NUM(name, type, convert, extra...) \
    error__t name(const char **string, type *result) \
    { \
        errno = 0; \
        const char *start = *string; \
        char *end; \
        *result = (type) convert(start, &end, ##extra); \
        *string = end; \
        return check_number(start, *string); \
    }

DEFINE_PARSE_NUM(parse_int,    int,          strtol, 0)
DEFINE_PARSE_NUM(parse_uint,   unsigned int, strtoul, 0)
DEFINE_PARSE_NUM(parse_uint32, uint32_t,     strtoul, 0)
DEFINE_PARSE_NUM(parse_uint64, uint64_t,     strtoull, 0)
DEFINE_PARSE_NUM(parse_double, double,       strtod)


error__t parse_bit(const char **string, bool *result)
{
    return
        TEST_OK_(**string == '0'  ||  **string == '1', "Invalid bit value")  ?:
        DO(*result = *(*string)++ == '1');
}


error__t parse_eos(const char **string)
{
    return TEST_OK_(**string == '\0', "Unexpected character after input");
}

error__t parse_uint32_array(
    const char **string, size_t *count, uint32_t result[], size_t max_length)
{
    error__t error = ERROR_OK;
    while (!error && !(**string == '\0')) {
        skip_whitespace(string) ;
        error =
            TEST_OK_(*count < max_length, "Too many values") ?:
            parse_uint32(string, &(result[*count])) ;
        (*count)++;
    }
    return error;
}


/* ************************************************************************* */
/* Insert Functions*/

error__t __attribute__((format(printf, 3, 4))) format_string(
    char **result, size_t *length, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int written = vsnprintf(*result, *length, format, args);
    va_end(args);

    bool length_ok = written < (int) *length;
    *result += (size_t) written;
    *length -= (size_t) written;
    return TEST_OK_(written >= 0  &&  length_ok,
        "Result too long");
}

error__t __attribute__((format(printf, 3, 4))) format_string_(
    char *result, size_t length, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int written = vsnprintf(result, length, format, args);
    va_end(args);

    return TEST_OK_(written >= 0  &&  written < (int) length,
        "Result too long");
}

error__t format_uint32_array(
    char **result, size_t *length, const uint32_t value[], size_t count)
{
    error__t error = ERROR_OK;
    for (size_t i = 0; !error  &&  i < count; i ++)
        error = format_string(result, length, " %u", value[i]);
    return error;
}
