Indented File Parser
====================

This header is required to be included alongside ``common.h`` and ``error.h``.

``parse_indent.h`` is a framework to parse files which structures data though
indentation.  Each line of the file is parsed line by line by the current parser
contained in :func:`parse_line`. Each parser is able to supply a new parser
for any lines which are indented beneath it allowing each indentation level to
be parsed to a different specification.  An incorrectly formatted file, for
example invalid indentation or unparsable data results in an error being
reported and no more parsing will be done.


API
---

..  type:: struct indent_parser

    This struct encapsulates all of the parsing methods required to read the
    file. It contains 3 members:

    ..  member:: void \*context

        Context passed through to associated methods.

    ..  function:: error__t (\*parse_line)(\
            void \*context, const char \*\*line, struct indent_parser \*parser)

        Parses one line using the given indentation, parse context, and parser.
        Must return a new indent_parser and parse context if sub-context lines
        are to be parsed.

    ..  function:: error__t (\*end)(void \*context)

        This is called when the indent parser is finished with at the current
        indentation level, and is optional: should be set to NULL if not
        required.

..  function:: error__t parse_indented_file(\
        const char \*file_name, const struct indent_parser \*parser)

        Begins the parsing of the file as described by `parser`.


..  note::

    The indented file is able to be indented using tabs or whitespaces however
    can not be a mix of both.

Usage
-----

Below is a sample usage for parsnig an indented file, the aim of the code is to
parse a file which contains key value pairs (motor_example.conf) to describe the
configuration for different motor.

motor_example.conf::

    # Comments and blank lines are skipped

    motor_one
        speed 4
        accel 3

    motor_two
        speed 1
        accel 5

parse_motor_example.c::

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdint.h>
    #include <string.h>

    #include "error.h"
    #include "parse.h"
    #include "parse_indent.h"

    struct motor_config {
        char name[32];
        unsigned int speed;
        int acceleration;
    };


    static error__t parse_entry(
        void *context, const char **line, struct indent_parser *next_parser)
    {
        struct motor_config *config = context;
        char key[32];
        int value;
        error__t error =
            parse_name(line, key, sizeof(key))  ?:
            parse_whitespace(line, false)  ?:
            parse_int(line, &value);

        if (error)
            return error;

        if (strcmp(key, "speed") == 0)
            config->speed = (unsigned int) value;
        else if (strcmp(key, "accel") == 0)
            config->acceleration = value;
        else
            error = FAIL_("Unknown Key");

        // next_parser is left empty, no futher indentation will be parsed

        return error;
    }


    /* Called when a section's indented block is closed. */
    static error__t end_section(void *context)
    {
        struct motor_config *config = context;
        printf("Motor Configuration:\n"
            "Motor Name: %s\n"
            "Initial Speed: %u\n"
            "Acceleration: %u\n\n",
            config->name, config->speed, config->acceleration);
        return ERROR_OK;
    }


    static error__t parse_section(
        void *context, const char **line, struct indent_parser *next_parser)
    {
        struct motor_config *config = context;
        *config =  (struct motor_config) {};
        return
            parse_name(line, config->name, sizeof(config->name))  ?:
            DO(
                *next_parser = (struct indent_parser) {
                    .context = config,
                 // Parsing function for first indent
                    .parse_line = parse_entry,
                    .end = end_section,
                });
    }


    int main(int argc, char **argv)
    {
        if (argc != 2)
        {
            fprintf(stderr, "Usage: %s <file>\n", argv[0]);
            return 1;
        }

        struct motor_config config = { };
        struct indent_parser parser = {
            .context = &config,
            .parse_line = parse_section, // Parsing function for zeroth indent
            .end = end_section
        };

        error__t error = parse_indented_file(argv[1], &parser);
        return error_report(error) ? 1 : 0;
    }

Running as thus ::

    $ ./parse_motor_example motor_example.conf
    Motor Configuration:
    Motor Name: motor_one
    Initial Speed: 4
    Acceleration: 3

    Motor Configuration:
    Motor Name: motor_two
    Initial Speed: 1
    Acceleration: 5
