    .text

    .globl sumar
    .type sumar, @function
sumar:
    enter $32, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .Lsumar_epilogue
.Lsumar_epilogue:
    leave
    ret
    .size sumar, . - sumar

    .globl factorial
    .type factorial, @function
factorial:
    enter $80, $0
    movq %rdi, -8(%rbp)
    movq $2, %r10
    movq %r10, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    cmpq $0, %r10
    je .LL0
    movq $1, %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %rax
    jmp .Lfactorial_epilogue
.LL0:
    movq $1, %r10
    movq %r10, -48(%rbp)
    movq -8(%rbp), %r10
    movq -48(%rbp), %r11
    subq %r11, %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %rdi
    xorl %eax, %eax
    call factorial
    movq %rax, -64(%rbp)
    movq -8(%rbp), %r10
    movq -64(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -72(%rbp)
    movq -72(%rbp), %rax
    jmp .Lfactorial_epilogue
.Lfactorial_epilogue:
    leave
    ret
    .size factorial, . - factorial

    .globl main
    .type main, @function
main:
    enter $64, $0
    movq $20, %r10
    movq %r10, -8(%rbp)
    movq $22, %r10
    movq %r10, -16(%rbp)
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    xorl %eax, %eax
    call sumar
    movq %rax, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq $5, %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %rdi
    xorl %eax, %eax
    call factorial
    movq %rax, -48(%rbp)
    movq -48(%rbp), %r10
    movq %r10, -56(%rbp)
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

