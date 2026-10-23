    .text

    .globl f
    .type f, @function
f:
    enter $64, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    cmpq $0, %r10
    je .LL0
    movq $0, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    cmpq $0, %r10
    je .LL2
    movq $1, %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %rax
    jmp .Lf_epilogue
    jmp .LL3
.LL2:
    movq $2, %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %rax
    jmp .Lf_epilogue
.LL3:
    jmp .LL1
.LL0:
    movq $3, %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %rax
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

