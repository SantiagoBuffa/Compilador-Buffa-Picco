    .text

    .globl main
    .type main, @function
main:
    enter $240, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq $2, %r10
    movq %r10, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -24(%rbp)
    movq $3, %r10
    movq %r10, -32(%rbp)
    movq $4, %r10
    movq %r10, -40(%rbp)
    movq -32(%rbp), %r10
    movq -40(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -48(%rbp)
    movq $5, %r10
    movq %r10, -56(%rbp)
    movq -48(%rbp), %rax
    cqto
    movq -56(%rbp), %r10
    idivq %r10
    movq %rax, -64(%rbp)
    movq $6, %r10
    movq %r10, -72(%rbp)
    movq -64(%rbp), %rax
    cqto
    movq -72(%rbp), %r10
    idivq %r10
    movq %rdx, -80(%rbp)
    movq -24(%rbp), %r10
    movq -80(%rbp), %r11
    subq %r11, %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    movq %r10, -96(%rbp)
    movq $1, %r10
    movq %r10, -104(%rbp)
    movq -104(%rbp), %r10
    movq %r10, -112(%rbp)
    movq -112(%rbp), %r10
    cmpq $0, %r10
    je .LL1
    movq $0, %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    movq %r10, -112(%rbp)
.LL1:
    movq -112(%rbp), %r10
    movq %r10, -136(%rbp)
    movq -136(%rbp), %r10
    cmpq $0, %r10
    jne .LL0
    movq $1, %r10
    movq %r10, -152(%rbp)
    movq -152(%rbp), %r10
    cmpq $0, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -160(%rbp)
    movq -160(%rbp), %r10
    movq %r10, -136(%rbp)
.LL0:
    movq -136(%rbp), %r10
    movq %r10, -168(%rbp)
    movq -96(%rbp), %r10
    movq -184(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -176(%rbp)
    movq -176(%rbp), %r10
    movq %r10, -192(%rbp)
    movq -192(%rbp), %r10
    cmpq $0, %r10
    jne .LL3
    movq -96(%rbp), %r10
    movq -184(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -208(%rbp)
    movq -208(%rbp), %r10
    movq %r10, -192(%rbp)
.LL3:
    movq -192(%rbp), %r10
    movq %r10, -216(%rbp)
    movq -216(%rbp), %r10
    cmpq $0, %r10
    jne .LL2
    movq -96(%rbp), %r10
    movq -184(%rbp), %r11
    cmpq %r11, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -232(%rbp)
    movq -232(%rbp), %r10
    movq %r10, -216(%rbp)
.LL2:
    movq -216(%rbp), %r10
    movq %r10, -240(%rbp)
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

