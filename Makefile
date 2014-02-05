#####################################################################
# GNU Makefile Template for multiple project                        #
# Author: Duc Minh Tran                                             #
# Email: 901sttAtgmailDotcom                                        #
#####################################################################


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
# export DEBUG            = 0
# export SHARED_LIB       = 0
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
# export CC              ?= $(CROSS_COMPILE)$(COMPILER_CC)
# export CXX             ?= $(CROSS_COMPILE)$(COMPILER_CXX)
# export AR              ?= $(CROSS_COMPILE)$(COMPILER_AR)
# export LD              ?= $(CROSS_COMPILE)$(COMPILER_LD)
# export RANLIB          ?= $(CROSS_COMPILE)$(COMPILER_RANLIB)
# DEBUG_FLAGS             = -g3 -DDEBUG_ALL
# RELEASE_FLAGS           = -O2



.PHONY: all
all: build_app

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

