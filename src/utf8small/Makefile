TARGET           = libutf8small.a
OBJS             = utf8small.o
SRCS             = $(OBJS:.o=.c)
INCLUDEDIRS     += -I../../include
LIBDIRS         += -L../lib
LDFLAGS         += $(LIBDIRS) -lm
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
TARGET_LIBDIR    = ../lib


.PHONY: all
all:	release


$(TARGET): $(OBJS)
#	For making static library, un-comment the 2 following lines
	$(AR) rs $@ $^
	$(RANLIB) $@
# For making shared library, un-comment the following line
#	$(CC) -shared -Wl,-soname,$@ -o $@ $^

.c.o:
#	To use shared library, add -fPIC
#	$(CC) -fPIC -c $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)


.PHONY: copy_target
copy_target:
	@$(MKDIR) -pv $(TARGET_LIBDIR)
	@$(CP) -fv $(TARGET) $(TARGET_LIBDIR)


.PHONY: debug
debug:	CFLAGS += $(DEBUG_FLAGS)
debug:	$(TARGET)
debug:	copy_target

.PHONY: release
release:  CFLAGS += $(RELEASE_FLAGS)
release:  $(TARGET)
release:  copy_target

.PHONY: clean
clean:
	@$(RM) -fv $(OBJS) $(TARGET)

dep:
	gccmakedep $(SRCS)

