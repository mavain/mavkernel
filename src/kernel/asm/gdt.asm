extern gdt_load
gdt_load:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]

    lgdt [eax]

    mov esp, ebp
    pop ebp
    ret