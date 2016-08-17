CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
BINDIR?=$(EXEC_PREFIX)/bin

OBJS = initrd.o

BINARIES = generate_initrd

CC = gcc

all: $(BINARIES)

.PHONY: all clean install install-binaries install-headers

generate_initrd: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(CPPFLAGS) $(OBJS) $(LIBS)

%.o: %.c 
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(OBJS) $(BINARIES) *.o */*.o */*/*.o

install: install-headers install-binaries

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rv include $(DESTDIR)$(INCLUDEDIR)

install-binaries: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(BINDIR)
