TARGET           = bogo
OBJS             = main.o
SRCS             = $(OBJS:.o=.c)
INCLUDEDIRS     += -I../../include
LIBDIRS         += -L../lib
LDFLAGS         += $(LIBDIRS) -lbogoengine -lutf8small -lm -lreadline
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
BUILD_DIR        = ../build


.PHONY: all
all:	release


$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(INCLUDEDIRS) $(CFLAGS) $(LDFLAGS) $(ADDFLAGS)

.c.o:
#	To use shared library, add -fPIC
#	$(CC) -fPIC -c $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)


.PHONY: copy_target
copy_target:
	@$(MKDIR) -pv $(BUILD_DIR)
	@$(CP) -fv $(TARGET) $(BUILD_DIR)


.PHONY: debug
debug:  CFLAGS += $(DEBUG_FLAGS)
debug:  $(TARGET)
debug:  copy_target

.PHONY: release
release:  CFLAGS += $(RELEASE_FLAGS)
release:  $(TARGET)
release:  copy_target

.PHONY: clean
clean:
	@$(RM) -fv $(OBJS) $(TARGET)

dep:
	gccmakedep $(SRCS)

