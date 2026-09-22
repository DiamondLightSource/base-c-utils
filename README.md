# C-Utils

A collection of common utility files for C projects. This project aims to 
improve consistency between repositories and having a straightforward build 
system allowing it to be used with and without conjunction with the epics build
system. 


## Contents 

| File (.c/.h)  | Description | Dependencies |
| ------------  | ----------- | ------------ |
| error         | Framework for handling and reporting errors. | None |
| buffered_file | Buffered interface for reading/writing to a socket | error |
| hashtable     | Hashtable implimentation | error |
| locking       | Helper functions for interations with mutex's | error |
| parse         | Simple parsing support | error | 

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

### EPICS IOC

1) Add this line to configure/RELEASE
```
C-UTILS = /path/to/this/directory
```
2) Add `c-utils` as an external library in src makefiles
```
<your_project>_LIBS += c-utils
```

### Other Projects 

Include as you would another library. 

Add the following flags to your compile command.
```
-I</path/to/this/directory>/include 
-L</path/to/this/directory>/lib/<target-architecture>
-lc-utils
```