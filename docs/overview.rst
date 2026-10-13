Introduction to Base C Utils
====================================

The table below shows the correspondence between header files and documentation
chapters (the header files are listed roughly in the order they need to be
``#include``\ d):

===================  =====================
``common.h``         :doc:`common`
``error.h``          :doc:`error`
``hashtable.h``      :doc:`hashtable`
``locking.h``        :doc:`locking`
``parse.h``          :doc:`parse`
``buffered_file.h``  :doc:`buffered_file`
``parse_indent.h``   :doc:`parse_indent`
===================  =====================

:doc:`common`:
    A collection of common macros and functions used throughout this support
    module.  This header must be included when using any of the other files.

:doc:`error`:
    A generic error handling mechanism is used throughout this support module
    and the associated header file is available for use elsewere.
    Depends on :doc:`common`.

:doc:`hashtable`:
    This is a hashtable implementation with decent performance taking much
    detailed inspiration from Python's implementation of dictionaries.
    Depends on :doc:`common`.

:doc:`locking`:
    Macros and helper function for commonly used :manpage:`pthreads(7)` api's.
    Depends on :doc:`common` and :doc:`error` to be included alongside.

:doc:`parse`:
    String parsing utilities with helpful formatting functions.
    Depends on :doc:`common` and :doc:`error`.

:doc:`buffered_file`:
    Buffered interfaced for IO operations also working on sockets.
    Depends on :doc:`common` and :doc:`error`.

:doc:`parse_indent`:
    A module to aid in the parsing of files with information grouped though
    levels of indentation.
    Depends on :doc:`common` and :doc:`error`.
