
include Makefile.extra

# Must contain prefix 'lib' for epics to find it
LIBRARY_NAME = libc-utils.a

SRCS += buffered_file.c 
SRCS += error.c 
SRCS += hashtable.c 
SRCS += locking.c 
SRCS += parse.c 

INCS += buffered_file.h 
INCS += error.h 
INCS += hashtable.h 
INCS += locking.h 
INCS += parse.h 

OBJS = $(addprefix $(ARCH)/, $(SRCS:.c=.o))

INCLUDE_PATH = ../include
LIB_DIR = ../lib/$(ARCH)
HEADERS = $(addprefix $(INCLUDE_PATH)/, $(INCS))

install: $(OBJS) $(HEADERS)
	ar rc $(LIB_DIR)/$(LIBRARY_NAME) $(OBJS)

COMPILE = $(CC) -fPIC $(USR_LDFLAGS) $(USR_CFLAGS)

$(ARCH)/%.o: %.c $(ARCH)
	$(COMPILE) -c -o $@ $< 

$(INCLUDE_PATH)/%.h: %.h
	cp $< $@

$(ARCH): 
	mkdir -p $(ARCH)

clean: 
	rm -rf linux-*

.DELETE_ON_ERROR:
