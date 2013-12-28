UTF8_SRC = \
	../src/utf8small.c

UNITTEST_SRC = \
	../src/unittest.c
UNITTEST_OBJ = $(patsubst %.c,%.o,$(UNITTEST_SRC))


SRC_FILES = $(UTF8_SRC) $(UNITTEST_SRC)

TEST_DSL_SRC = \
	../src/dsl.o \
	../src/utf8small.o \
	test_dsl.o \
	$(UNITTEST_SRC)
TEST_DSL_OBJ = $(patsubst %.c,%.o,$(TEST_DSL_SRC))

TEST_BOGO_SRC = \
	../src/bogo.o \
	../src/list.o \
	../src/utf8small.o \
	test_bogo.o \
	$(UNITTEST_SRC)
TEST_BOGO_OBJ = $(patsubst %.c,%.o,$(TEST_BOGO_SRC))

INCLUDE          = ../src/
TEST_DATA_DIR    = ../test_data
BUILD_TEST_DIR   = ../build

TEST_UTF8_INPUT  = $(TEST_DATA_DIR)/utf8_input.txt
TEST_UTF8_SRC    = test_utf8.c
TEST_UTF8_EXEC   = $(BUILD_TEST_DIR)/test_utf8

CFLAGS = -g -std=c99 -I$(INCLUDE)

test: test_dsl test_bogo
	./test_dsl
	./test_bogo

test_utf: $(TEST_UTF8_SRC)
	@mkdir -p $(BUILD_TEST_DIR)
	@$(CC) -o $(TEST_UTF8_EXEC) \
		-I$(INCLUDE) \
		$(TEST_UTF8_SRC) \
		$(SRC_FILES)
	@$(TEST_UTF8_EXEC) < $(TEST_UTF8_INPUT)

test_dsl: $(TEST_DSL_OBJ)

test_bogo: $(TEST_BOGO_OBJ)

clean:
	rm -rf \
		$(BUILD_TEST_DIR) \
		*.o *.i *.gch *.a
