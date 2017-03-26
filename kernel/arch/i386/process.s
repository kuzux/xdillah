global copy_page_phys
global read_eip
global _taskswitch
global _usermode 

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
    mov ecx, [esp+4]    ; eip
    mov eax, [esp+8]    ; physical address of current directory
    ; temporarily disable stack moving, but we'll need to do that soon
    ; maybe we should have the kernel stack identity mapped?
    ; mov esp, [esp+12]   ; esp
    mov ebp, [esp+16]   ; ebp
    mov cr3, eax        ; set the page directory
    sti
    ; jumping to the new eip is also currently broken
    ; that is like the whole promise of multitasking
    jmp ecx

_usermode:
    cli
    mov ax, 0x23
    mov ds, ax
    mov es, ax
    mov fs, ax

    mov eax, esp
    push 0x23
    push eax
    pushf
    push 0x1b
    push usermode1
    iret
usermode1:
