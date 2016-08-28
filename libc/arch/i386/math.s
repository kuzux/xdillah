_sin:
    push ebp
    mov ebp, esp
    sub esp, 4

    fld qword [ebp+8]
    fsin
    fst qword [ebp-4]

    mov eax, [ebp-4]
    
    mov esp, ebp
    pop ebp
    ret

global _sin

_cos:
    push ebp
    mov ebp, esp
    sub esp, 4

    fld qword [ebp+8]
    fcos
    fst qword [ebp-4]

    mov eax, [ebp-4]

    mov esp, ebp
    pop ebp
    ret

global _cos
