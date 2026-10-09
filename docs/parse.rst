String Parsing Support
======================

Is required to be included with ``common.h`` (see :doc:`common`) and ``error.h``
(see :doc:`error`).

The header file ``parse.h`` is designed to be used for parsing a string that is
null terminated and does not contain a ``\n`` character.  A sequence of these
functions is designed to chained together to parse a whitespace delimited
command/string using the ``?:`` operator.


Common Parse Functions
----------------------

Where applicable these functions use the standard C conversion functions for
converting a string to the corresponding data type. For example
:func:`parse_int` uses :manpage:`strtol(3)` for converting to a value, and will
stop at the first non digit character found.

..  function:: error__t parse_whitespace(const char **string, bool optional)

    Advances `*string` past whitespace, If no whitespace is found and
    `optional` flag is not set and error is returned otherwise we consume the
    whitespace and return `ERROR_OK`.  The `optional` flag results in
    `ERROR_OK` to always be returned.

..  function:: error__t parse_char(const char **string, char ch)

    If the character at `**string` is `ch` we consume the character and
    return ``ERROR_OK`` otherwise we return an error.

..  function:: error__t parse_uint(const char **string, unsigned int *result)

    Parses an unsigned integer from `*string`.  If successfull advances `string`
    , stores the result of the parse in `result` and returns `ERROR_OK`
    otherwise returns the error

..  function:: error__t parse_uint32(const char **string, uint32_t *result)

    Parses a 32-bit unsigned integer from `*string`.  If successfull advances
    `string`, stores the result of the parse in `result` and returns
    `ERROR_OK` otherwise returns the error.

..  function:: error__t parse_uint64(const char **string, uint64_t *result)

    Parses a 64-bit unsigned integer from `*string`.  If successfull advances
    `string`, stores the result of the parse in `result` and returns
    `ERROR_OK` otherwise returns the error.

..  function:: error__t parse_int(const char **string, int *result)

    Parses a signed integer from `*string`.  If successfull advances `string`,
    stores the result of the parse in `result` and returns `ERROR_OK`
    otherwise returns the error

..  function:: error__t parse_double(const char **string, double *result)

    Parses a double from `*string`.  If successfull advances `string`, stores
    the result of the parse in `result` and returns `ERROR_OK` otherwise
    returns the error.

..  function:: error__t parse_bit(const char **string, bool *result)

    Parses bit from `*string`.  If successfull advances `string` , stores the
    result of the parse in `result` and returns `ERROR_OK` otherwise returns
    the error.

..  function:: error__t parse_eos(const char **string)

    Checks for end of the input string.  If `\0` is found returns `ERROR_OK`
    otherwise returns an error.

Other Functions
---------------

..  function:: bool read_char(const char **string, char ch)

    Tests whether the next character in `*string` is `ch` and if so consumes
    it and returns true, otherwise returns false.

..  function:: char get_char(const char **string)

    Returns the next character at `*string` and advances past it.

..  function:: void skip_whitespace(const char **string)

    Increments `*string` to first non space character in string.

..  function:: size_t read_whitespace(const char **string)

    Increments `*string` to first non space character in string. Returns number
    of whitespace characters seen.

..  function:: error__t parse_name(\
        const char **string, char result[], size_t max_length)

    This parses out a sequence of letters and underscores into the result array.
    The given max_length includes the trailing null character.

..  function:: error__t parse_alphanum_name(\
        const char **string, char result[], size_t max_length)

    This parses out a sequence of letters, underscores and numbers into the
    result array. The name must not start with a number. The given max_length
    includes the trailing null character.

..  function:: error__t parse_block_name(\
        const char **string, char result[], size_t max_length)

    This parses out a sequence of letters, underscores and numbers into the
    result array. The name must not start or end with a number.  The given
    max_length includes the trailing null character.

..  function:: bool read_string(const char **string, const char *expected)

    Tests whether input string matches given comparison string, if so consumes
    it and returns true, otherwise returns false.

..  function:: bool read_substring(const char *string, const char *sub_string)

    Tests whether input string is a substring of comparison string.


Array Parsing functions
-----------------------

These functions parse a whitespace delimited array, stopping at the first
character which cannot be parsed or end of line.

..  function:: error__t parse_uint32_array(const char **string, size_t *length,\
        uint32_t result[], size_t max_length)

    Parses a maximum of `max_length` uint32s from string into result. Counts
    the number of entries in length.

..  function:: error__t parse_uint_array(const char **string, size_t *length, \
        unsigned int result[], size_t max_length)

    Parses a maximum of `max_length` unsigned ints from string into result.
    Counts the number of entries in length.

..  function:: error__t parse_double_array(const char **string, size_t *length,\
        double result[], size_t max_length)

    Parses a maximum of `max_length` doubles from string into result. Counts
    the number of entries in length.


Insert functions
----------------

..  function:: error__t format_string_(\
       char *result, size_t length, const char *format, ...)

    Write a max of length bytes of the formatted string to `*result`. Returns
    an error if the write failed or there was unsufficent space for the
    formatted string to be written.

..  function:: error__t format_string(\
        char **result, size_t *length, const char *format, ...)

    Write a max of length bytes of the formatted string to `*result`,
    increments the string and readjusts `length` to the resulting space after
    the write. This is helpful to be used when recursively formatting a string.
    See :func:`format_string_` for error return conditions.

..  function:: error__t format_double(\
       char *result, size_t length, double value)

    Formats `value` to a precision of 10 digits, removing any leading
    spaces. See :func:`format_string_` for error return conditions.

..  function::
    error__t format_uint32_array(\
        char **result, size_t *length, const uint32_t value[], size_t count)

    Attempts to write `count` entries from `value` into `string` up to a
    maximum of `length` bytes. See :func:`format_string_` for error return
    conditions.
