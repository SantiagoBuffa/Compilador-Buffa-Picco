    .text

    .globl f
    .type f, @function
f:
    enter $32, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    cmpq $0, %r10
    je .LL0
    movq $1, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .Lf_epilogue
    jmp .LL1
.LL0:
    movq $2, %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %rax
    jmp .Lf_epilogue
.LL1:
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

