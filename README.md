# C-Utils

A collection of common utility files for C projects. This project aims to
improve consistency between repositories and having a straightforward build
system allowing it to be used with and without conjunction with the epics build
system.


## Library Components


| File (.c/.h)  | Description | Dependencies (.c/.h) |
| ------------  | ----------- | ------------ |
| common        | Common logging and common utility macros | None |
| error         | Framework for handling and reporting errors. | common |
| buffered_file | Buffered interface for reading/writing to a socket | common, error |
| hashtable     | Hashtable implimentation | common |
| locking       | Helper functions for interations with mutex's | common, error |
| parse         | Simple parsing support | common, error |


## Build

### Default ( linux-x86_64 )
```bash
make
```

To compile for other architectures:
```bash
make CC="/path/to/c-compiler" ARCH="<your architecture>"
```

## How to include in your project

### Optional Add Makefile.extra

Makefile.extra is a compilation of linker and compiler flags. Add to src
makefiles to include
```
include /path/to/this/directory/src/Makefile.extra
```

### EPICS Project

1) Add this line to configure/RELEASE
```
C-UTILS = /path/to/this/directory
```
2) Add `c-utils` as an external library in src makefiles
```
<your_project>_LIBS += c-utils
```

3) If including Makefile.extra, add flags to be used by EPICS
```
USR_CFLAGS += $(CFLAGS)
USR_CPPFLAGS += $(CPPFLAGS)
USR_LDFLAGS += $(LDFLAGS)
```

### Other Projects

Include as you would another library.

1) Add the following flags to your compile command.
```
-I</path/to/this/directory>/include
-L</path/to/this/directory>/lib/<target-architecture>
-lc-utils
```

2) If including Makefile.extra, add ` $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) ` to
compile command.
