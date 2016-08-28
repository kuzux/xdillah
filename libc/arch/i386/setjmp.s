geteip:
    mov eax, [esp]
    ret

internal_setjmp:
    push ebp
    mov ebp, esp
    sub esp, 8
    ; eax is ebp-4
    ; ebx is ebp-8

    mov [ebp-4], eax
    mov [ebp-8], ebx
    mov eax, [8+ebp]

    mov [ebp-4], ebx

    mov dword [eax], dword ebx

    mov ebx, [ebp-8]

    mov [eax+4], ebx
    mov [eax+8], ecx
    mov [eax+12], edx
    ; move the eflags register later to eax+16
    mov [eax+20], cs
    mov [eax+24], ds
    mov [eax+28], es
    mov [eax+32], fs
    mov [eax+36], gs
    mov [eax+40], ss
    mov [eax+44], edi
    mov [eax+48], esi
    mov ebx, ebp
    sub ebx, 12 ; get old base pointer
    mov [eax+52], ebx
    mov [eax+56], ebx ; we dont have the old esp value
    call geteip ; moves the eip register to eax
    mov [eax+60], eax

    mov esp, ebp
    pop ebp
    ret

global internal_setjmp


internal_longjmp:
    push ebp
    mov ebp, esp
    sub esp, 8
    mov [ebp-4], eax
    mov [ebp-8], ebx
    mov eax, [8+ebp]
    mov ebx, [eax]

    mov dword [ebp-4], dword ebx

    mov ebx, [ebp-8]

    mov ebx, [eax+4]
    mov ecx, [eax+8]
    mov edx, [eax+12]
    ;eflags are not yet moved
    ;mov eflags, [eax+16]
    mov cs, [eax+20]
    mov ds, [eax+24]
    mov es, [eax+28]
    mov fs, [eax+32]
    mov gs, [eax+36]
    mov ss, [eax+40]
    mov edi, [eax+44]
    mov esi, [eax+48]
    mov ebp, [eax+52]
    mov esp, [eax+56]

    mov esp, ebp
    pop ebp

    jmp [eax+60]

global internal_longjmp
