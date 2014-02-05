#####################################################################
# GNU Makefile Template for multiple project                        #
# Author: Duc Minh Tran                                             #
# Email: 901sttAtgmailDotcom                                        #
#####################################################################


OBJS             = unittest.o
SRCS             = $(patsubst %.c,%.o,$(wildcard *.c))
INCLUDEDIRS     += -I../../include
LIBDIRS         += -L../lib
LDFLAGS         += $(LIBDIRS) -lm
CFLAGS          += -W -Wall
DEBUG_FLAGS      = -g3 -DDEBUG_ALL
RELEASE_FLAGS    = -O2
DEBUG           := 1
SHARED_LIB      := 0
ARCH            ?=
CROSS_COMPILE   ?=
COMPILER_CC     ?= gcc
COMPILER_CXX    ?= g++
COMPILER_AR     ?= ar
COMPILER_LD     ?= ld
COMPILER_RANLIB ?= ranlib
CC              ?= $(CROSS_COMPILE)$(COMPILER_CC)
CXX             ?= $(CROSS_COMPILE)$(COMPILER_CXX)
AR              ?= $(CROSS_COMPILE)$(COMPILER_AR)
LD              ?= $(CROSS_COMPILE)$(COMPILER_LD)
RANLIB          ?= $(CROSS_COMPILE)$(COMPILER_RANLIB)
MKDIR           ?= mkdir
CP              ?= cp
RM              ?= rm
TARGET_LIBDIR    = ../lib


ifeq ($(DEBUG), 1)
	CFLAGS += $(DEBUG_FLAGS)
else
	CFLAGS += $(RELEASE_FLAGS)
endif
ifeq ($(SHARED_LIB), 1)
	TARGET := libunittest.so
else
	TARGET := libunittest.a
endif


.PHONY: all
all:	build_target


$(TARGET): $(OBJS)
ifeq ($(SHARED_LIB), 1)
	$(CC) -shared -Wl,-soname,$@ -o $@ $^
else
	$(AR) rs $@ $^
	$(RANLIB) $@
endif

%.o : %.c
ifeq ($(SHARED_LIB), 1)
	$(CC) -fPIC -c $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
else
	$(CC) -c  $< $(INCLUDEDIRS) $(CFLAGS) $(ARCHFLAGS) $(LDFLAGS) $(ADDFLAGS)
endif


.PHONY: copy_target
copy_target:
	@$(MKDIR) -pv $(TARGET_LIBDIR)
	@$(CP) -fv $(TARGET) $(TARGET_LIBDIR)


.PHONY: build_target
build_target:  $(TARGET)
build_target:  copy_target

.PHONY: clean
clean:
	@$(RM) -fv $(OBJS) $(TARGET)

dep:
	gccmakedep $(SRCS)

