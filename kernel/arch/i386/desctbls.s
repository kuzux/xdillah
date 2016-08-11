global gdt_flush
global idt_flush

gdt_flush:
    mov eax, [esp+4]    ; load the gdt pointer address from the argument passed
    lgdt [eax]          ; load the gdt
    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush     ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
