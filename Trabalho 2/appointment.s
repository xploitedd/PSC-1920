    .text
    .global get_appointment
/* get_appointment(agenda[] -> rdi, month -> rsi, day -> rdx, index -> rcx) */
get_appointment:
    /* obtain the base address for the struct */
    shl $8, %rsi
    lea (%rdi, %rsi), %rax
    test %rax, %rax
    jz not_found

    /* obtain the base address for the Appointments */
    mov 8(%rax, %rdx, 8), %rax
    test %rax, %rax
    jz not_found

    /* check if index < length */
    mov (%rax), %r8
    cmp %rcx, %r8
    jle not_found

    /* obtain the address of the first element of the array */
    mov 8(%rax), %rax
    /* obtain the address of the corresponding element of the array */
    shl $4, %rcx
    lea (%rax, %rcx), %rax
    ret
not_found:
    xor %rax, %rax
    ret
