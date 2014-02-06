# This file is a part of bogoengine project.
#
# Copyright (C) 2014 Duc Minh Tran <901sttAtgmailDotcom>
# Copyright (C) 2014 Trung Ngo <ndtrung4419 AT gmail DOT com>
#
# This file is distributed under the GPL v3 license. See COPYING at the
# project's top level for more details.
#

.PHONY: all
all: _all

# If you need to do cross-compiling, set the CC, LD, AR environment variables
# to the ones specific to your toolchain. Make a shellscript if you need to
# save keystrokes.
#
# $ make CC=my-gcc LD=my-ld AR=my-ar CFLAGS=--my-platform-flag all

CC              ?= gcc

INCLUDEDIRS     += -Iinclude
LIBDIRS         += -L.
LDFLAGS         += $(LIBDIRS) -lm
CFLAGS          += -W -Wall -std=c99 $(LDFLAGS) $(INCLUDEDIRS)

DEBUG_FLAGS     := -g3 -DDEBUG_ALL
RELEASE_FLAGS   := -O2

ifdef DEBUG
CFLAGS += $(DEBUG_FLAGS)
else
CFLAGS += $(RELEASE_FLAGS)
endif

# @lewtds: Not sure if this is needed as it looks almost the same as the
#          default implicit rule for C.
# %.o : %.c
# 	$(CC) -o $@ -c $< $(CFLAGS)

include engine.mk
include interpreter.mk
include tests.mk

#
# Misc rules
#

.PHONY: clean
clean:
	rm -rf $(CLOBBER)

.PHONY: _all
_all: $(ENGINE_TARGET) $(INTERPRETER_TARGET)
