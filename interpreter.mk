# This file is a part of bogoengine project.
#
# Copyright (C) 2014 Trung Ngo <ndtrung4419 AT gmail DOT com>
#
# This file is distributed under the GPL v3 license. See COPYING at the
# project's top level for more details.
#


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

.PHONY: clean_interpreter
clean_interpreter:
	rm -rf $(INTERPRETER_OBJ) $(INTERPRETER_TARGET)
