global copy_page_phys
global read_eip
global _taskswitch

copy_page_phys:
    push ebx 
    pushf 
    cli

    mov ebx, [esp+12] ; source
    mov ecx, [esp+16] ; destination

    ; disable paging
    mov edx, cr0
    and edx, 0x7fffffff
    mov cr0, edx

    ; we will be copying 4K, i.e. 1024 words
    mov edx, 1024

.loop:
    mov eax, [ebx]
    mov [ecx], eax

    add ebx, 4
    add ecx, 4
    
    dec edx
    jnz .loop

    ;re-enable paging
    mov edx, cr0
    or edx, 0x80000000
    mov cr0, edx

    popf
    pop ebx
    ret

read_eip:
    pop eax
    jmp eax

_taskswitch:
    cli
    mov ecx, [esp+4]
    mov eax, [esp+8]
    mov ebp, [esp+12]
    mov esp, [esp+16]
    mov cr3, eax
    mov eax, 0x12345
    sti
    jmp ecx
