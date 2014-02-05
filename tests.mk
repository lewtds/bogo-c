# This file is a part of bogoengine project.
#
# Copyright (C) 2014 Trung Ngo <ndtrung4419 AT gmail DOT com>
#
# This file is distributed under the GPL v3 license. See COPYING at the
# project's top level for more details.
#


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
$(TEST_TARGETS): $(TEST_OBJS) tests/unittest/unittest.o
	gcc $@.o -o $@ $(CFLAGS)

.PHONY: build_tests
build_tests: $(ENGINE_TARGET) $(ENGINE_HDRS) $(TEST_TARGETS)

.PHONY: test
test: build_tests
	LD_LIBRARY_PATH=. ./tests/test_bogo
	LD_LIBRARY_PATH=. ./tests/test_dsl
	LD_LIBRARY_PATH=. ./tests/test_tone_and_mark
	LD_LIBRARY_PATH=. ./tests/test_utf8 < $(TEST_UTF8_INPUT)

.PHONY: clean_tests
clean_tests:
	rm -rf $(TEST_OBJS) $(TEST_TARGETS) tests/unittest/unittest.o
