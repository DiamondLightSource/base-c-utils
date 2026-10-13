Buffered IO Functionality
=========================

Is required to be included with ``common.h`` (see :doc:`common`) and ``error.h``
(see :doc:`error`)

Akin to :manpage:`fdopen(3)` however is designed to be used for IO over sockets
and other interfaces. It allows for a buffered interface to be used when
reading and writing.


Creation and Deletion API
-------------------------

..  type:: struct buffered_file

    This is an opaque type for the read and write operations over a socket. A
    :type:`buffered_file` can be created by calling :func:`bf_create_file` and
    should be deleted by :func:`bf_destroy_file` which will free all resources
    stored in the :type:`buffered_file`.

..  function:: struct buffered_file *bf_create_file(\
        int sock, size_t in_buf_size, size_t out_buf_size)

    Creates buffered file mounted on `sock`. The `sock` should therefore have a
    lifetime at least equal to that of the returned `buffered_file`.

..  function:: error__t bf_destroy_file(struct buffered_file *file)

    Releases all resources associated with the buffered file and returns any
    errors stored inside of the :type:`buffered_file`.

..  function:: bool bf_check_file(struct buffered_file *file)

    Returns the error status of the buffered file.  If false is returned then an
    error condition has been detected, and no more read or write operations will
    take place and the `file` should be destroyed with :func:`bf_destroy_file`.

Read API
--------

..  function:: bool bf_read_line(\
        struct buffered_file *file, char line[], size_t line_size, bool flush)

    Reads one newline terminated line from file into `line`.  Returns false if
    EOF is encountered, or if the line buffer overruns.  If flush is requested
    then any pending output is written.

..  function:: bool bf_read_block(\
        struct buffered_file *file, char data[], size_t length)

    Reads `length` size block of data from `file` into `data`, returns false if
    EOF is encountered, or an a read error occurs before `length` bytes of data
    is read.

Write API
---------

..  function:: bool bf_write_string(\
        struct buffered_file *file, const char string[], size_t length)

    Writes a given character array to output.  Flushing the out buffer if
    required to write all of `string`. Returns false if unable to write for any
    reason.

..  function:: bool bf_write_formatted_string(\
        struct buffered_file *file, const char *format, ...)

    Writes a formatted string to output.  Attempts to flush the buffer once if
    the initial output buffer has filled up. Returns false if there is not
    enough space after the repeated attempt to write the string.

..  function:: bool bf_write_block(\
        struct buffered_file *file, const void *buffer, size_t length)

    Writes buffer to output.  The output buffer is bypassed, after first being
    flushed if necessary.  Returns false if unable to write for any reason.

..  function:: bool bf_write_char(struct buffered_file *file, char ch)

    Writes a single character to output.  Returns false if unable to write for
    any reason.

..  function:: bool bf_flush_out(struct buffered_file *file)

    Ensures output buffer is flushed to socket.  Returns false if unable to
    write for any reason.
