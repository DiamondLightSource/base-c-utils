
ifndef TOP
$(error Do not call this make file directly)
endif

include Makefile.extra

LIBRARY_NAME = libc-utils.a

SRCS=$(wildcard *.c)
HEADERS=$(wildcard *.h)

INC_DIR = $(TOP)/include
LIB_BASE_DIR = $(TOP)/lib
LIB_DIR = $(LIB_BASE_DIR)/$(ARCH)
OBJ_DIR = O.$(ARCH)

REQUIRED_DIRS = $(INC_DIR) $(LIB_DIR) $(OBJ_DIR) 

INCS = $(addprefix $(INC_DIR)/, $(HEADERS))
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
LIB = $(LIB_DIR)/$(LIBRARY_NAME)

default: $(REQUIRED_DIRS) $(OBJS) $(INCS) $(LIB)
	
COMPILE = $(CC) -fPIC $(LDFLAGS) $(CFLAGS) $(CPPFLAGS) $(CFLAGS_EXTRA)

$(REQUIRED_DIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: %.c
	$(COMPILE) -c -o $@ $<

$(INCS): $(INC_DIR)
	cp $(HEADERS) $(INC_DIR)

$(LIB): $(LIB_DIR)
	ar rc $(LIB) $(OBJS)

clean: 
	rm -rf $(REQUIRED_DIRS)
	rm -rf $(LIB_BASE_DIR)

.DELETE_ON_ERROR:
