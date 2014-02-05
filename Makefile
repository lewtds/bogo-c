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

#
# Build libbogo
#

UTF8_SRC      = src/utf8small/utf8small.c
ENGINE_SRC    = $(UTF8_SRC) \
                src/engine/bogo.c \
                src/engine/dsl.c
ENGINE_HDRS   = include/bogo.h \
                include/common.h \
                include/dsl.h \
                include/utf8small.h
ENGINE_OBJ    = $(ENGINE_SRC:.c=.o)

ifeq ($(SHARED_LIB), 1)

ENGINE_TARGET = libbogo.so

$(ENGINE_TARGET): CFLAGS += -fPIC --shared
$(ENGINE_TARGET): $(ENGINE_OBJ) $(ENGINE_HDRS)
	gcc -o $@ $(CFLAGS) $^

else

ENGINE_TARGET = libbogo.a

$(ENGINE_TARGET): $(ENGINE_OBJ) $(ENGINE_HDRS)
	$(AR) rs $@ $^

endif

#
# Build the interactive interpreter
#

INTERPRETER_TARGET = bogo
INTERPRETER_SRC    = src/interpreter.c
INTERPRETER_OBJ    = $(INTERPRETER_SRC:.c=.o)
INTERPRETER_LIBS   = -lreadline -lbogo

$(INTERPRETER_TARGET): LDFLAGS += $(INTERPRETER_LIBS)

# @lewtds: I don't like it that $(ENGINE_TARGET) got included in $^.
#          We already have that covered with -lbogo.
$(INTERPRETER_TARGET): $(INTERPRETER_OBJ) $(ENGINE_TARGET) $(ENGINE_HDRS)
	gcc $^ -o $@ $(CFLAGS)

#
# Tests
#

TEST_DATA_DIR    = ./tests/test_data
TEST_UTF8_INPUT  = $(TEST_DATA_DIR)/utf8_input.txt

TEST_TARGETS     = tests/test_dsl \
                   tests/test_bogo \
                   tests/test_tone_and_mark \
                   tests/test_utf8
TEST_OBJS        = $(TEST_TARGETS:=.o)
TEST_LIBS        = -lbogo

$(TEST_TARGETS): LDFLAGS += $(TEST_LIBS)
$(TEST_TARGETS): $(TEST_OBJS) $(ENGINE_TARGET) $(ENGINE_HDRS)
	gcc $@.o tests/unittest/unittest.o -o $@ $(CFLAGS)

.PHONY: build_tests
build_tests: $(TEST_TARGETS)

.PHONY: test
test:
test: build_tests
	LD_LIBRARY_PATH=. ./tests/test_bogo
	LD_LIBRARY_PATH=. ./tests/test_dsl
	LD_LIBRARY_PATH=. ./tests/test_tone_and_mark
	LD_LIBRARY_PATH=. ./tests/test_utf8 < $(TEST_UTF8_INPUT)

#
# Misc rules
#

.PHONY: clean
clean:
	rm -rf $(INTERPRETER_TARGET) \
	       $(INTERPRETER_OBJ) \
	       $(ENGINE_TARGET) \
	       $(ENGINE_OBJ) \
	       libbogo.so

.PHONY: _all
_all: $(ENGINE_TARGET) $(INTERPRETER_TARGET)
