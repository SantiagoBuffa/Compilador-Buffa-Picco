    .text

    .globl main
    .type main, @function
main:
    enter $80, $0
    movq -16(%rbp), %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -24(%rbp)
    movq -40(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -64(%rbp), %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -72(%rbp)
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

