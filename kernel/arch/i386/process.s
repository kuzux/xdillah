global copy_page_phys

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
