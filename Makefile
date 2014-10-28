CFLAGS = -ffreestanding -fbuiltin -Wall -Wextra -nostdlib -m32 -Iinclude

OBJS = kernel.o tty.o ioprim.o gdt.o idt.o string.o desctbls.o interrupts.o isr.o timer.o kb.o panic.o kmain.o

kernel: $(OBJS)
	ld -m elf_i386 -T link.ld -o kernel $(OBJS)

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS)

%.o: %.s
	nasm -f elf32 $< -o $@

clean:
	rm kernel *.o
