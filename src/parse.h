/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Simple parsing support. */

#ifndef _PARSE_H
#define _PARSE_H 1

/* Must be long enough to send a line of waveform data (256 uints) in one buffer */
#define MAX_LINE_LENGTH 3072 // 3KB

/* Incements *string to first non space character in string. */
void skip_whitespace(const char **string);

/* Advances *string past whitespace, fails with error if no whitespace found and
 * optional flag not set. */
error__t parse_whitespace(const char **string, bool optional);

/* This parses out a sequence of letters and underscores into the result array.
 * The given max_length includes the trailing null character. */
error__t parse_name(const char **string, char result[], size_t max_length);

/* As for parse_name, but also accepts numbers after the leading character. */
error__t parse_alphanum_name(
    const char **string, char result[], size_t max_length);

/* Block names are alphanumeric, but have the special case that they're not
 * allowed to end in digits. */
error__t parse_block_name(
    const char **string, char result[], size_t max_length);

/* Tests whether the next character in *string is ch and if so consumes it and
 * returns true, otherwise returns false. */
bool read_char(const char **string, char ch);

/* Returns the next character at *string and advances past it */
char get_char(const char **string);

/* Tests whether input string matches given comparison string, if so consumes it
 * and returns true, otherwise returns false. */
bool read_string(const char **string, const char *expected);

/* Tests whether input string is a substring of comparison string. */
bool read_substring(const char *string, const char *sub_string);

/* Expects next character to be ch, fails if not. */
error__t parse_char(const char **string, char ch);

/* Parses an unsigned integer from *string. */
error__t parse_uint(const char **string, unsigned int *result);

/* Parses a 32-bit unsigned integer from *string. */
error__t parse_uint32(const char **string, uint32_t *result);

/* Parses a 64-bit unsigned integer from *string. */
error__t parse_uint64(const char **string, uint64_t *result);

/* Parses a signed integer from *string. */
error__t parse_int(const char **string, int *result);

/* Parses a double from *string. */
error__t parse_double(const char **string, double *result);

/* Parses bit from *string. */
error__t parse_bit(const char **string, bool *result);

/* Checks for end of input string. */
error__t parse_eos(const char **string);

/* Parses an array of uints32_ts from string. */
error__t parse_uint32_array(
    const char **string, size_t *length, uint32_t result[], size_t max_length);

/* Write a max of length bytes of the formatted string to *result and increments
 * the string */
error__t __attribute__((format(printf, 3, 4))) format_string(
    char **result, size_t *length, const char *format, ...);

error__t __attribute__((format(printf, 3, 4))) format_string_(
    char *result, size_t length, const char *format, ...);

/* Writes a max of length bytes to *result from value and stores the number of
 * entries in count. */
error__t format_uint32_array(
    char **result, size_t *length, const uint32_t value[], size_t count);

#endif