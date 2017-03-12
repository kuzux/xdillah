HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))
 
CFLAGS?=-O2 -ggdb
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib
 
CFLAGS:=$(CFLAGS) -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_xdillah_libc -Iinclude -I../kernel/include
LIBK_CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_xdillah_kernel
 
ARCHDIR:=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS:=$(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS:=$(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
 
FREEOBJS:=\
$(ARCH_FREEOBJS) \
stdio.o \
ctype.o \
errno.o \
stdlib/ato.o \
stdlib/strto.o \
stdlib/rand.o \
stdlib/alloc.o \
stdlib/env.o \
stdlib/arrays.o \
stdlib/arith.o \
stdlib/wchar.o \
string/mem.o \
string/str.o \
string/strn.o \
string/strpbrk.o \
string/strtok.o \
math/trig.o \
math/hyp.o \
math/explog.o \
math/pow.o \
math/erf.o \
math/round.o \
math/fpmanip.o \
math/minmax.o \
math/other.o \
setjmp.o \
unistd/alloc.o \

HOSTEDOBJS:=\
$(ARCH_HOSTEDOBJS) \
 
OBJS:=\
$(FREEOBJS) \
$(HOSTEDOBJS) \
 
LIBK_OBJS:=$(FREEOBJS:.o=.libk.o)
 
BINARIES=libc.a libg.a libk.a crt0.o
 
all: $(BINARIES)
 
.PHONY: all clean install install-headers install-libs
 
libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)
 
libg.a:
	$(AR) rcs $@
 
libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

crt0.o: arch/i386/crt0.o
	cp $< $@

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)
 
%.o: %.s
	nasm -f elf $< -o $@
 
%.libk.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)
 
%.libk.o: %.s
	nasm -f elf $< -o $@
 
clean:
	rm -f $(BINARIES) $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
 
install: install-headers install-libs
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rv include/ $(DESTDIR)$(INCLUDEDIR)
 
install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)
