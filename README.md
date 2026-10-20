# Base-C-Utils

A collection of common utility files for C projects with no external
dependencies. This project aims to improve consistency between repositories.
This module does not provide a static library file information on how to include
in a project see the [Build section](#Build).

## Library Components

| File (.c/.h)   | Description                                        | Dependencies (.c/.h) |
| ------------   | -------------------------------------------------- | -------------------- |
| common         | Common logging and common utility macros           | None                 |
| error          | Framework for handling and reporting errors.       | common               |
| buffered_file  | Buffered interface for reading/writing to a socket | common, error        |
| hashtable      | Hashtable implementation                           | common               |
| locking        | Helper functions for interations with mutex's      | common, error        |
| parse          | Simple parsing support                             | common, error        |
| parse_indent   | Parsing a file with data structured through indentation | common, error |
| Makefile.extra | Common compiler flags to be included in makefiles  | None                 |


## Build

The library files in this project are not to be built here, instead these
library files are to be used as source files and built inside of another
project. This is done so the other projects can specify the
architecture/compiler of choice without this library having been built for that
target previously.

### Include in a EPICS project

Add the following lines...

1) To `configure/RELEASE`

```
C_UTILS = /path/to/this/directory/src
```

2) To the `src/Makefile`

```
## Optional
include $(C_UTILS)/Makefile.extra
USR_CFLAGS += $(CFLAGS)

USR_CPPFLAGS += -iquote$(C_UTILS)

<name>_SRCS += $(notdir $(wildcard $(C_UTILS)/*.c))
VPATH += $(C_UTILS)
```

### Include in other projects

Here is an example of how to include in other projects Makefile. This may
require adjustment for project with different or more complicated build systems.

1) Add the following lines to your src Makefile.

```make
C_UTILS = /path/to/this/directory/src

include $(C_UTILS)/Makefile.extra # Optional

# Add include path
CPPFLAGS += -iquote$(C_UTILS)

# Add C_UTILS to your list of sources and add to VPATH
<name>_SRCS = $(notdir $(wildcard $(C_UTILS)/*.c))
VPATH += $(C_UTILS)
```
