    .text

    .globl main
    .type main, @function
main:
    enter $208, $0
    movq $0, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -16(%rbp)
    movq $0, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
.LL0:
    movq $3, %r10
    movq %r10, -40(%rbp)
    movq -16(%rbp), %r10
    movq -40(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    cmpq $0, %r10
    je .LL1
    movq $0, %r10
    movq %r10, -64(%rbp)
    movq -64(%rbp), %r10
    movq %r10, -72(%rbp)
.LL2:
    movq $3, %r10
    movq %r10, -80(%rbp)
    movq -72(%rbp), %r10
    movq -80(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    cmpq $0, %r10
    je .LL3
    movq $0, %r10
    movq %r10, -104(%rbp)
    movq -104(%rbp), %r10
    movq %r10, -112(%rbp)
.LL4:
    movq $2, %r10
    movq %r10, -120(%rbp)
    movq -112(%rbp), %r10
    movq -120(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    cmpq $0, %r10
    je .LL5
    movq $1, %r10
    movq %r10, -144(%rbp)
    movq -32(%rbp), %r10
    movq -144(%rbp), %r11
    addq %r11, %r10
    movq %r10, -152(%rbp)
    movq -152(%rbp), %r10
    movq %r10, -32(%rbp)
    movq $1, %r10
    movq %r10, -160(%rbp)
    movq -112(%rbp), %r10
    movq -160(%rbp), %r11
    addq %r11, %r10
    movq %r10, -168(%rbp)
    movq -168(%rbp), %r10
    movq %r10, -112(%rbp)
    jmp .LL4
.LL5:
    movq $1, %r10
    movq %r10, -176(%rbp)
    movq -72(%rbp), %r10
    movq -176(%rbp), %r11
    addq %r11, %r10
    movq %r10, -184(%rbp)
    movq -184(%rbp), %r10
    movq %r10, -72(%rbp)
    jmp .LL2
.LL3:
    movq $1, %r10
    movq %r10, -192(%rbp)
    movq -16(%rbp), %r10
    movq -192(%rbp), %r11
    addq %r11, %r10
    movq %r10, -200(%rbp)
    movq -200(%rbp), %r10
    movq %r10, -16(%rbp)
    jmp .LL0
.LL1:
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

