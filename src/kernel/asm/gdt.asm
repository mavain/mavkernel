extern gdt_load
gdt_load:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]

    lgdt [eax]

    mov esp, ebp
    pop ebp

    jmp 0x10:.far_jmp_lbl       ; I spent a week trying to figure out why the ISRs would #GPF
    .far_jmp_lbl:               ; If you don't far jump the GDT is still old and RIP your ISRs and patience

    ret