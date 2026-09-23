TOP := $(CURDIR)

# ------------------------------------------------------------------------------
# Build library files
#

SRC_DIR = $(TOP)/src

ARCH ?= linux-x86_64
CC ?= $(CC)

build: 
	$(MAKE) -C $(SRC_DIR) CC=$(CC) ARCH=$(ARCH) TOP=$(TOP)

# ------------------------------------------------------------------------------

clean:
	$(MAKE) -C src clean ARCH=$(ARCH) TOP=$(TOP)
