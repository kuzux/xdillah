CC = i386-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -nostdlib -Iinclude

OBJS = kernel.o kmain.o tty.o ioprim.o gdt.o idt.o \
       string.o desctbls.o interrupts.o isr.o \
       timer.o kb.o panic.o paging.o kheap.o bitmap.o

kernel: $(OBJS) link.ld
	$(CC) -T link.ld -o kernel $(CFLAGS) -lgcc $(OBJS)
	ld -m elf_i386 -T link.ld -o kernel $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS)

%.o: %.s
	nasm -f elf32 $< -o $@

clean:
	rm kernel *.o
