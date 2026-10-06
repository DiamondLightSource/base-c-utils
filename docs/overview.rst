Introduction to Base C Utils
====================================

The table below shows the correspondence between header files and documentation
chapters (the header files are listed roughly in the order they need to be
``#include``\ d):

==================  ===============================
``common.h``        :doc:`common`
``error.h``         :doc:`error`
``hashtable.h``     :doc:`hashtable`
==================  ===============================

:doc:`error`:
    A generic error handling mechanism is used throughout this support module
    and the associated header file is available for use elsewere.

:doc:`hashtable`:
    This is a hashtable implementation with decent performance taking much
    detailed inspiration from Python's implementation of dictionaries.
