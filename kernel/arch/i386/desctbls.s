global gdt_flush
global idt_flush
global tss_flush

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

tss_flush:
    mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                      ; 0x28, as it is the 5th selector and each is 8 bytes
                      ; long, but we set the bottom two bits (making 0x2B)
                      ; so that it has an RPL of 3, not zero.
    ltr ax            ; Load 0x2B into the task state register.
    ret
