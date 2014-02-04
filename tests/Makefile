#####################################################################
# GNU Makefile Template for multiple project                        #
# Author: Duc Minh Tran                                             #
# Email: 901sttAtgmailDotcom                                        #
#####################################################################


TARGET           = test_dsl test_bogo test_tone_and_mark
OBJS             = $(TARGET:=.o)
SRCS             = $(patsubst %.c,%.o,$(wildcard *.c))
INCLUDEDIRS     += -I../include -I../src/engine -I../src/utf8small
LIBDIRS         += -L../src/lib
LDFLAGS         += $(LIBDIRS) -lbogoengine -lutf8small -lunittest -lm
LDFLAGS         += $(LIBDIRS)
CFLAGS          += -W
DEBUG_FLAGS      = -g3 -DDEBUG_ALL
RELEASE_FLAGS    = -O2
ARCH            ?=
CROSS_COMPILE   ?=
COMPILER_CC     ?= gcc
COMPILER_CXX    ?= g++
COMPILER_AR     ?= ar
COMPILER_LD     ?= ld
COMPILER_RANLIB ?= ranlib
CC               = $(CROSS_COMPILE)$(COMPILER_CC)
CXX              = $(CROSS_COMPILE)$(COMPILER_CXX)
AR               = $(CROSS_COMPILE)$(COMPILER_AR)
LD               = $(CROSS_COMPILE)$(COMPILER_LD)
RANLIB           = $(CROSS_COMPILE)$(COMPILER_RANLIB)
MKDIR           ?= mkdir
CP              ?= cp
RM              ?= rm
BUILD_DIR        = ./

TEST_DATA_DIR    = ../test_data
BUILD_TEST_DIR   = ../build

TEST_UTF8_INPUT  = $(TEST_DATA_DIR)/utf8_input.txt
TEST_UTF8_SRC    = test_utf8.c
TEST_UTF8_EXEC   = $(BUILD_TEST_DIR)/test_utf8

LD_LIB_TMP      := $(LD_LIBRARY_PATH)
LD_LIBRARY_PATH := ../src/lib:$(LD_LIB_TMP)
export LD_LIBRARY_PATH

test: $(TARGET)
	./test_dsl
	./test_tone_and_mark
	./test_bogo

test_utf: $(TEST_UTF8_SRC)
	@mkdir -p $(BUILD_TEST_DIR)
	@$(CC) -o $(TEST_UTF8_EXEC) \
		-I$(INCLUDE) \
		$(TEST_UTF8_SRC) \
		$(SRC_FILES)
	@$(TEST_UTF8_EXEC) < $(TEST_UTF8_INPUT)



$(TARGET): $(OBJS)
	$(CC) $@.o -o $@ $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(ADDFLAGS)

%.o : %.c
ifeq ($(SHARED_LIB), 1)
	$(CC) -fPIC -c $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
else
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
endif


clean:
	@$(RM) -rvf \
		$(BUILD_TEST_DIR) \
		$(OBJS) $(TARGET)

