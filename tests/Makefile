UTF8_SRC = \
	../src/utf8small.c

UNITTEST_SRC = \
	../src/unittest.c
UNITTEST_OBJ = $(patsubst %.c,%.o,$(UNITTEST_SRC))


SRC_FILES = $(UTF8_SRC) $(UNITTEST_SRC)

TEST_DSL_SRC = \
	../src/dsl.c \
	../src/utf8small.c \
	test_dsl.c \
	$(UNITTEST_SRC)
TEST_DSL_OBJ = $(patsubst %.c,%.o,$(TEST_DSL_SRC))

BOGO_SRC = \
	../src/dsl.c \
	../src/utf8small.c \
	$(UNITTEST_SRC)
BOGO_OBJ = $(patsubst %.c,%.o,$(BOGO_SRC))

INCLUDE          = ../src/
TEST_DATA_DIR    = ../test_data
BUILD_TEST_DIR   = ../build

TEST_UTF8_INPUT  = $(TEST_DATA_DIR)/utf8_input.txt
TEST_UTF8_SRC    = test_utf8.c
TEST_UTF8_EXEC   = $(BUILD_TEST_DIR)/test_utf8

CFLAGS = -g -std=c99 -I$(INCLUDE)

test: test_dsl test_tone_and_mark test_bogo
	./test_dsl
	./test_tone_and_mark
	./test_bogo

build-tests: test_dsl test_tone_and_mark test_bogo

test_utf: $(TEST_UTF8_SRC)
	@mkdir -p $(BUILD_TEST_DIR)
	@$(CC) -o $(TEST_UTF8_EXEC) \
		-I$(INCLUDE) \
		$(TEST_UTF8_SRC) \
		$(SRC_FILES)
	@$(TEST_UTF8_EXEC) < $(TEST_UTF8_INPUT)

test_dsl: $(TEST_DSL_SRC)

test_bogo: $(BOGO_SRC) test_bogo.c

test_tone_and_mark: $(BOGO_SRC) test_tone_and_mark.c

clean:
	rm -rf \
		$(BUILD_TEST_DIR) \
		*.o *.i *.gch *.a \
		test_dsl test_tone_and_mark test_bogo
