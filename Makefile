#####################################################################
# GNU Makefile Template for multiple project                        #
# Author: Duc Minh Tran                                             #
# Email: 901sttAtgmailDotcom                                        #
#####################################################################

all: _all


# NOTES
#
# The CROSS_COMPILE flag must be a prefix for gcc.
#
# E.g. If the C cross-compiler's name is arm-unknown-linux-gcc
# then CROSS_COMPILE should be "arm-unknown-linux-".
#
# The CC, CXX, ARR, LD, RANLIB flags should not be edited as
# they are handled automatically.
#
# DEBUG_FLAGS and REALEASE_FLAGS should be kept intact.


ARCH            ?=
ARCHFLAGS        =
CROSS_COMPILE   ?=
COMPILER_CC     ?= gcc
COMPILER_CXX    ?= g++
COMPILER_AR     ?= ar
COMPILER_LD     ?= ld
COMPILER_RANLIB ?= ranlib
MKDIR           ?= mkdir
CP              ?= cp
RM              ?= rm
CC              ?= $(CROSS_COMPILE)$(COMPILER_CC)
CXX             ?= $(CROSS_COMPILE)$(COMPILER_CXX)
AR              ?= $(CROSS_COMPILE)$(COMPILER_AR)
LD              ?= $(CROSS_COMPILE)$(COMPILER_LD)
RANLIB          ?= $(CROSS_COMPILE)$(COMPILER_RANLIB)

DEBUG_FLAGS      = -g3 -DDEBUG_ALL
RELEASE_FLAGS    = -O2
INCLUDEDIRS     += -Iinclude
LIBDIRS         += -L.
LDFLAGS         += $(LIBDIRS) -lm
CFLAGS          += -W -Wall -std=c99  $(ARCHFLAGS) $(LDFLAGS) $(INCLUDEDIRS)

ifeq ($(DEBUG), 1)
CFLAGS += $(DEBUG_FLAGS)
else
CFLAGS += $(RELEASE_FLAGS)
endif

# @lewtds: Not sure if this is needed as it looks almost the same as the
#          default implicit rule for C.
# %.o : %.c
# 	$(CC) -o $@ -c $< $(CFLAGS)

include libbogo.mk
include interpreter.mk
include tests.mk

#
# Misc rules
#

.PHONY: clean
clean: clean_libbogo clean_interpreter clean_tests

.PHONY: _all
_all: $(ENGINE_TARGET) $(INTERPRETER_TARGET)
