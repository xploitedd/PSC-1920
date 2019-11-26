    .text
    .global memchr
/* memchr(ptr -> rdi, value -> rsi, num -> rdx) */
memchr:
    xor %r8, %r8
memchr_lb:
    cmp $0, %rdx
    jz memchr_nf

    /* load qword if num >= 8 */
    cmp $8, %rdx
    jge memchr_lqword

    /* load byte if num < 8 */
    movb (%rdi, %r8), %r9b
    inc %r8
    mov $1, %r11
    jmp memchr_loop_init
memchr_lqword:
    movq (%rdi, %r8), %r9
    add $8, %r8
    mov $8, %r11
memchr_loop_init:
    /* initalize loop registers */
    sub %r11, %rdx
    xor %rcx, %rcx
    mov %rsi, %r10
memchr_loop:
    /* check if we need to read another memory zone */
    cmp %rcx, %r11
    jz memchr_lb

    /* check if the byte being analyzed corresponds */
    mov %r9, %rax
    and %r10, %rax
    sub %r10, %rax
    jz memchr_f

    /* iterate to the next byte available */
    shl $8, %r10
    inc %rcx
    jmp memchr_loop
memchr_nf:
    /* no value was found */
    xor %rax, %rax
    ret
memchr_f:
    /* the value was found */
    sub %r11, %r8
    add %r8, %rcx
    lea (%rdi, %rcx), %rax
    ret
