    .text

    .globl emptyMethod
    .type emptyMethod, @function
emptyMethod:
    enter $16, $0
.LemptyMethod_epilogue:
    leave
    ret
    .size emptyMethod, . - emptyMethod

    .globl math
    .type math, @function
math:
    enter $288, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq $10, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %r10
    negq %r10
    movq %r10, -40(%rbp)
    movq $5, %r10
    movq %r10, -48(%rbp)
    movq $2, %r10
    movq %r10, -56(%rbp)
    movq $1, %r10
    movq %r10, -64(%rbp)
    movq -56(%rbp), %r10
    movq -64(%rbp), %r11
    subq %r11, %r10
    movq %r10, -72(%rbp)
    movq -48(%rbp), %r10
    movq -72(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -80(%rbp)
    movq $3, %r10
    movq %r10, -88(%rbp)
    movq -80(%rbp), %rax
    cqto
    movq -88(%rbp), %r10
    idivq %r10
    movq %rax, -96(%rbp)
    movq $2, %r10
    movq %r10, -104(%rbp)
    movq -96(%rbp), %rax
    cqto
    movq -104(%rbp), %r10
    idivq %r10
    movq %rdx, -112(%rbp)
    movq -40(%rbp), %r10
    movq -112(%rbp), %r11
    addq %r11, %r10
    movq %r10, -120(%rbp)
    movq -120(%rbp), %r10
    movq %r10, -32(%rbp)
    movq $1, %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    cmpq $0, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -136(%rbp)
    movq -136(%rbp), %r10
    movq %r10, -144(%rbp)
    movq -144(%rbp), %r10
    cmpq $0, %r10
    jne .LL0
    movq $0, %r10
    movq %r10, -160(%rbp)
    movq -160(%rbp), %r10
    movq %r10, -168(%rbp)
    movq -168(%rbp), %r10
    cmpq $0, %r10
    je .LL1
    movq $5, %r10
    movq %r10, -184(%rbp)
    movq -32(%rbp), %r10
    movq -184(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -192(%rbp)
    movq -192(%rbp), %r10
    movq %r10, -168(%rbp)
.LL1:
    movq -168(%rbp), %r10
    movq %r10, -144(%rbp)
.LL0:
    movq -144(%rbp), %r10
    movq %r10, -200(%rbp)
    movq $10, %r10
    movq %r10, -208(%rbp)
    movq -32(%rbp), %r10
    movq -208(%rbp), %r11
    cmpq %r11, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -216(%rbp)
    movq -216(%rbp), %r10
    movq %r10, -224(%rbp)
    movq -200(%rbp), %r10
    cmpq $0, %r10
    je .LL2
    movq -32(%rbp), %r10
    movq %r10, -240(%rbp)
.LL4:
    movq $0, %r10
    movq %r10, -248(%rbp)
    movq -240(%rbp), %r10
    movq -248(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -256(%rbp)
    movq -256(%rbp), %r10
    cmpq $0, %r10
    je .LL5
    movq $1, %r10
    movq %r10, -272(%rbp)
    movq -240(%rbp), %r10
    movq -272(%rbp), %r11
    subq %r11, %r10
    movq %r10, -280(%rbp)
    movq -280(%rbp), %r10
    movq %r10, -240(%rbp)
    jmp .LL4
.LL5:
    jmp .LL3
.LL2:
    xorl %eax, %eax
    call emptyMethod
.LL3:
    movq -32(%rbp), %rax
    jmp .Lmath_epilogue
.Lmath_epilogue:
    leave
    ret
    .size math, . - math

    .globl main
    .type main, @function
main:
    enter $16, $0
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl globalVar
    .align 8
    .type globalVar, @object
    .size globalVar, 8
globalVar:
    .zero 8

    .globl g1
    .align 8
    .type g1, @object
    .size g1, 8
g1:
    .zero 8

    .globl g2
    .align 8
    .type g2, @object
    .size g2, 8
g2:
    .zero 8

    .globl flag
    .align 8
    .type flag, @object
    .size flag, 8
flag:
    .zero 8

