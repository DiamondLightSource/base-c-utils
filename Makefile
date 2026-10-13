TOP := $(CURDIR)

# ------------------------------------------------------------------------------
# Build library files


SRC_DIR = $(TOP)/src

ARCH ?= linux-x86_64
CC ?= $(CC)

build:
	$(MAKE) -C $(SRC_DIR) CC=$(CC) ARCH=$(ARCH) TOP=$(TOP)

# ------------------------------------------------------------------------------
# Docs

DOCS_DIR=$(TOP)/docs

.PHONY: docs

docs:
	$(MAKE) -C $(DOCS_DIR)

# ------------------------------------------------------------------------------


clean:
	$(MAKE) -C $(SRC_DIR) TOP=$(TOP) clean
	$(MAKE) -C $(DOCS_DIR) clean
