    .text
    .global find_and_copy
/* find_and_copy(dst %rdi, dim %rsi, ptrs %rdx, n %rcx, ctx %r8, eval %r9, backwards) */
find_and_copy:
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushq %rbx
    pushq %rdi
    pushq %rbp
    movq %rsp, %rbp
    subq $8, %rsp

    /* prepare registers for usage */
    movq %rsi, %r12
    movq %rdx, %r13
    movq %rcx, %r14
    movq %r8, %r15
    movq %r9, %rbx

    movq $-1, (%rsp)
loop_s:
    /* continue loop if --n >= 0 */
    cmp $0, %r14
    jl loop_end
    dec %r14

    /* call the compare function in %rbx */
    movq (%r13, %r14, 8), %rdi
    movq %r15, %rsi
    call *%rbx
    test %rax, %rax
    /* if the compare function returns false, do another loop cycle */
    jz loop_s

    /* save the current index and break if backwards is true */
    movq %r14, (%rsp)
    cmp $1, 64(%rbp)
    jz loop_end

    jmp loop_s
loop_end:
    /* check if the saved index is different than -1 */
    movq (%rsp), %r15
    leave
    popq %rdi
    cmp $-1, %r15
    jz not_copy
copy:
    /* if different than -1 copy the dim bytes using memcpy */
    movq (%r13, %r15, 8), %rsi
    movq %r12, %rdx
    call memcpy
not_copy:
    mov %r15, %rax
    popq %rbx
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    ret
