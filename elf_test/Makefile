CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
BINDIR?=$(EXEC_PREFIX)/bin

OBJS = main.o

BINARIES = elf_test

all: $(BINARIES)

.PHONY: all clean install install-binaries install-headers

elf_test: $(OBJS)
	$(info asd)
	$(CC) -o $@ $(CFLAGS) -nostartfiles $(CPPFLAGS) $(OBJS) $(LIBS)

%.o: %.c 
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(OBJS) $(BINARIES) *.o */*.o */*/*.o

install: install-headers install-binaries

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -Rv include/ $(DESTDIR)$(INCLUDEDIR)

install-binaries: $(BINARIES)
	mkdir -p $(DESTDIR)$(BINDIR)
	cp $(BINARIES) $(DESTDIR)$(BINDIR)
