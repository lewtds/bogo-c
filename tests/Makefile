TARGET           = test_dsl test_bogo test_tone_and_mark
OBJS             = test_bogo.o test_dsl.o test_tone_and_mark.o
SRCS             = $(OBJS:.o=.c)
INCLUDEDIRS     += -I../include
LIBDIRS         += -L../src/lib
LDFLAGS         += $(LIBDIRS) -lbogoengine -lutf8small -lunittest -lm
CFLAGS          += -W -Wall
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

CFLAGS = -g -std=c99 $(INCLUDEDIRS)

test: test_dsl test_tone_and_mark test_bogo
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

test_dsl: test_dsl.o
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) $^ -o $@ $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(ADDFLAGS)

test_bogo: test_bogo.o
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) $^ -o $@ $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(ADDFLAGS)

test_tone_and_mark: test_tone_and_mark.o
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) $^ -o $@ $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(ADDFLAGS)


.c.o:
#	To use shared library, add -fPIC
#	$(CC) -fPIC -c $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)


clean:
	rm -rf \
		$(BUILD_TEST_DIR) \
		$(OBJS) $(TARGET)

