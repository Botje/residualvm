MODULE := engines/kq8

MODULE_OBJS := \
	detection.o \
	inifile.o \
	kq8.o \
	primitives.o \
	registry.o \
	script.o \
	statements.o \
	vol/archive.o \
	vol/blast.o

# This module can be built as a plugin
ifeq ($(ENABLE_KQ8), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
