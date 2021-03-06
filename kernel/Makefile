HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))
 
CFLAGS?=-O2 -ggdb
CPPFLAGS?=
LDFLAGS?=
LIBS?=
 
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

XDILLAH_VERSION?=
XDILLAH_COMPILE_DATE?=
XDILLAH_COMPILE_ENV?=
XDILLAH_AUTHOR?=
 
CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_xdillah_kernel -DXDILLAH_VERSION=\""$(XDILLAH_VERSION)"\" \
	-DXDILLAH_COMPILE_DATE=\"$(XDILLAH_COMPILE_DATE)\" -DXDILLAH_COMPILE_ENV=\""$(XDILLAH_COMPILE_ENV)"\" \
	-DXDILLAH_AUTHOR=\"$(XDILLAH_AUTHOR)\" -Iinclude -I../libc/include
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(HOSTARCH)
 
include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)
 
OBJS:=\
$(KERNEL_ARCH_OBJS) \
bitmap.o \
gdt.o \
idt.o \
ioprim.o \
isr.o \
kb.o \
kheap.o \
kmain.o \
paging.o \
timer.o \
tty.o \
serial.o \
fs.o \
initrd.o \
elf.o \
tasking.o \
tss.o \
amfs.o \

CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o
 
ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \
 
OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ) \
 
all: xdillah.kernel
 
.PHONY: all clean install install-headers install-kernel
 
xdillah.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)
 
%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

arch/i386/crtn.o: arch/i386/crtn.s
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

arch/i386/crti.o: arch/i386/crti.s
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.s
	nasm -f elf $< -o $@
 
clean:
	rm -f xdillah.kernel $(OBJS) $(ALL_OUR_OBJS) *.o */*.o */*/*.o
 
install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rv include/ $(DESTDIR)$(INCLUDEDIR)
 
install-kernel: xdillah.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp xdillah.kernel $(DESTDIR)$(BOOTDIR)
