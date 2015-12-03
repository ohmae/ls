## @file Makefile
#
# Copyright(C) 2015 大前良介(OHMAE Ryosuke)
#
# This software is released under the MIT License.
# http://opensource.org/licenses/MIT
#
# @breaf
# @author <a href="mailto:ryo@mm2d.net">大前良介(OHMAE Ryosuke)</a>
# @date 2015/10/25

CC   = gcc
RM   = rm -rf
MAKE = make

CFLAGS = -Wall -g3 -O2
COPTS  = -D_DEBUG_
LDFLAGS =
# MODULES = $(patsubst %.c,%,$(wildcard *.c))
MODULES = ls1 ls2 ls3 ls4 ls5 ls6 ls7 ls8 ls9 ls10 ls11 ls12

.PHONY: all clean
all: $(MODULES)

clean:
	$(RM) $(MODULES)

%:%.c
	$(CC) $(CFLAGS) $(COPTS) $(LDFLAGS) $< -o $@
