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

.PHONY: clean_libbogo
clean_libbogo:
	rm -rf $(ENGINE_TARGET) $(ENGINE_OBJ) libbogo.so
