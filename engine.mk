# This file is a part of bogoengine project.
#
# Copyright (C) 2014 Trung Ngo <ndtrung4419 AT gmail DOT com>
#
# This file is distributed under the GPL v3 license. See COPYING at the
# project's top level for more details.
#

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

ifdef SHARED_LIB

ENGINE_TARGET = libbogo.so

$(ENGINE_TARGET): CFLAGS += -fPIC --shared
$(ENGINE_TARGET): $(ENGINE_OBJ) $(ENGINE_HDRS)
	$(CC) -o $@ $(CFLAGS) $^

else

ENGINE_TARGET = libbogo.a

$(ENGINE_TARGET): $(ENGINE_OBJ) $(ENGINE_HDRS)
	$(AR) rs $@ $^

js: $(ENGINE_TARGET)
	$(CC) -O2 $(ENGINE_TARGET) -o libbogo.js \
		-s EXPORTED_FUNCTIONS="['_sass_compile_emscripten']" \
		-s DISABLE_EXCEPTION_CATCHING=0

endif


CLOBBER += $(ENGINE_TARGET) $(ENGINE_OBJ) libbogo.so
