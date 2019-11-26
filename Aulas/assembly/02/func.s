    .text
    .global func1
    .global func2
    .global func3
    .global func4
    .global func5
    .global func6
func1:
    movq $8, %rax
    ret

func2:
    movq %rdi, %rax
    addq $5, %rax
    ret

func3:
    movq %rdi, %rax
    addq %rsi, %rax
    ret

func4:
    xorq %rax, %rax
    jmp func4_start
func4_loop:
    incq %rax
func4_start:
    movb (%rdi, %rax), %cl
    testb %cl, %cl
    jne func4_loop
    ret

func5:
    movq (%rdi, %rsi, 4), %rax
    ret

func6:
    movl 16(%rdi), %eax
    ret
