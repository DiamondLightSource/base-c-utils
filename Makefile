TOP := $(CURDIR)

# ------------------------------------------------------------------------------
# Build library files
#

SRC_DIR = $(TOP)/src

ARCH ?= linux-x86_64
CC ?= $(CC)

LIB_BASE_DIR = $(TOP)/lib
LIB_DIR = $(LIB_BASE_DIR)/$(ARCH)

INC_DIR = $(TOP)/include

build: create_dirs 
	$(MAKE) -C $(SRC_DIR) CC="$(CC)" ARCH="$(ARCH)" 

REQUIRED_DIRS += $(LIB_DIR) $(LIB_BASE_DIR) $(INC_DIR)
create_dirs:
	mkdir -p $(REQUIRED_DIRS)

# ------------------------------------------------------------------------------

clean: make_clean rm_dirs

rm_dirs: 
	rm -rf $(REQUIRED_DIRS)

make_clean: $(SRC_DIR)
	$(MAKE) -C $< clean