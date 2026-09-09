TOP := $(CURDIR)


# ------------------------------------------------------------------------------
# Build library files
#

SRC_DIR = $(TOP)/src

ARCH ?= linux-x86_64
CC ?= $(CC)

LIB_BASE_DIR = $(TOP)/lib
LIB_DIR = $(LIB_BASE_DIR)/$(ARCH)

INC_DIR= $(TOP)/include

# Only build wanted files by specifing DEPS
build: $(LIB_DIR) $(INC_DIR)
	$(MAKE) -C $(SRC_DIR) CC="$(CC)" ARCH="$(ARCH)" 

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(INC_DIR):
	mkdir -p $(INC_DIR)

# ------------------------------------------------------------------------------
# Documentation
#

docs:



clean: $(SRC_DIR)
	$(MAKE) -C $< clean
