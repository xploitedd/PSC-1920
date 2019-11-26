    .section .rodata
nl: .byte '\n'
    
    .text
    .global println
println:
    pushq %rdi
    call strlen
    popq %rdi

    movq %rax, %rdx
    movq %rdi, %rsi
    movq $1, %rdi
    movq $1, %rax
    syscall

    movq $1, %rdx
    leaq nl(%rip), %rsi
    movq $1, %rdi
    movq $1, %rax
    syscall

    ret
