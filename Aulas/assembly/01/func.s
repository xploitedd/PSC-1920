    .text
    .globl func
func:
    movq %rsi, %rax
    addq %rdi, %rax
    ret
