    .text
    .extern c_print
    .extern c_multiply

    .globl main
    .type main, @function
main:
    enter $32, $0
    movq $7, %r10
    movq %r10, -8(%rbp)
    movq $8, %r10
    movq %r10, -16(%rbp)
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    xorl %eax, %eax
    call c_multiply
    movq %rax, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %rdi
    xorl %eax, %eax
    call c_print
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

