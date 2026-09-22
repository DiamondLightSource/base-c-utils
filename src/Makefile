
include Makefile.extra

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

INC_DIR = ../include
LIB_DIR = ../lib/$(ARCH)
HEADERS = $(addprefix $(INC_DIR)/, $(INCS))

install: $(OBJS) $(HEADERS)
	ar rc $(LIB_DIR)/$(LIBRARY_NAME) $(OBJS)

COMPILE = $(CC) -fPIC $(LDFLAGS) $(CFLAGS) $(CPPFLAGS)

$(ARCH)/%.o: %.c $(ARCH)
	$(COMPILE) -c -o $@ $< 

$(INC_DIR)/%.h: %.h
	cp $< $@

$(ARCH): 
	mkdir -p $(ARCH)

clean: 
	rm -rf linux-*

.DELETE_ON_ERROR:
