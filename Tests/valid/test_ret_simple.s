    .text

    .globl f
    .type f, @function
f:
    enter $16, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %rax
    jmp .Lf_epilogue
.Lf_epilogue:
    leave
    ret
    .size f, . - f

    .globl main
    .type main, @function
main:
    enter $16, $0
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

