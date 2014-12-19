MODULE := engines/kq8

MODULE_OBJS := \
	detection.o \
	inifile.o \
	kq8.o \
	script.o \
	statements.o

# This module can be built as a plugin
ifeq ($(ENABLE_KQ8), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
