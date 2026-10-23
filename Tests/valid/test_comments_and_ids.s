    .text

    .globl main
    .type main, @function
main:
    enter $16, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -16(%rbp)
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

