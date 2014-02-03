
# Those are flag which use to support the build,
# and default when building
# Un-comment and edit to overwrite them

# The CROSS_COMPILE flag must be full prefix off gcc
# E.g C Cross compiler is arm-unknown-linux-gcc
# then the CROSS_COMPILE flag is "arm-unknown-linux-"

# Should not edit CC, CXX, ARR, LD, RANLIB flag
# they will be handled automatically

# DEBUG_FLAGS and REALEASE_FLAGS cannot be overwrite

# export INCLUDEDIRS     += -I../../include
# export LIBDIRS         += -L../lib
# export LDFLAGS         += $(LIBDIRS) -lm
# export CFLAGS          += -W -Wall -std=c99
export CFLAGS          += -std=c99
# export ARCH            ?=
# export CROSS_COMPILE   ?=
# export COMPILER_CC     ?= gcc
# export COMPILER_CXX    ?= g++
# export COMPILER_AR     ?= ar
# export COMPILER_LD     ?= ld
# export COMPILER_RANLIB ?= ranlib
# export MKDIR           ?= mkdir
# export CP              ?= cp
# export RM              ?= rm
# DEBUG_FLAGS      = -g3 -DDEBUG_ALL
# RELEASE_FLAGS    = -O2
# CC               = $(CROSS_COMPILE)$(COMPILER_CC)
# CXX              = $(CROSS_COMPILE)$(COMPILER_CXX)
# AR               = $(CROSS_COMPILE)$(COMPILER_AR)
# LD               = $(CROSS_COMPILE)$(COMPILER_LD)
# RANLIB           = $(CROSS_COMPILE)$(COMPILER_RANLIB)


.PHONY: all
all: release

.PHONY: debug
debug:  MAKEFILE_FLAGS := debug
debug:  build_app


.PHONY:	release
release:  MAKEFILE_FLAGS := release
release:  build_app

.PHONY:	test
test: MAKEFILE_FLAGS := test
test: build_app
	$(MAKE) -C tests

.PHONY: install
install: MAKEFILE_FLAGS := release
install: build_app

.PHONY: clean
clean: MAKEFILE_FLAGS := clean
clean:  build_app
	$(MAKE) -C tests $(MAKEFILE_FLAGS)

.PHONY: build_app
build_app:
	$(MAKE) -C src $(MAKEFILE_FLAGS)

