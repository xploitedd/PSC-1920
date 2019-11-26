    .text
    .global rotate_right
/* rotate_right(value -> rdi, n -> rsi) */
rotate_right:
    /* load l-qword and h-qword to two registers for faster loop access */
    movq (%rdi), %rcx       /* low qword */
    movq 8(%rdi), %rdx      /* high qword */

    /* zero the r10 and r11 registers */
    xor %r10, %r10
    xor %r11, %r11
rr_loop:
    /* loop condition */
    test %rsi, %rsi
    jz rr_end
    dec %rsi

    /* shift l-qword left and save the carry */
    shr $1, %rcx
    setc %r10b
    ror $1, %r10

    /* shift h-qword left and save the carry */
    shr $1, %rdx
    setc %r11b
    ror $1, %r11
    
    /* insert the new bits at the end of l-qword and h-qword */
    or %r10, %rdx
    or %r11, %rcx

    jmp rr_loop
rr_end:
    /* update the l-qword and h-qword to the memory */
    movq %rcx, (%rdi)
    movq %rdx, 8(%rdi)
    ret
