    .text

    .globl emptyCall
    .type emptyCall, @function
emptyCall:
    enter $16, $0
    jmp .LemptyCall_epilogue
.LemptyCall_epilogue:
    leave
    ret
    .size emptyCall, . - emptyCall

    .globl identity
    .type identity, @function
identity:
    enter $16, $0
    movq %rdi, -8(%rbp)
    movq -8(%rbp), %rax
    jmp .Lidentity_epilogue
.Lidentity_epilogue:
    leave
    ret
    .size identity, . - identity

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

    .globl testShadowing
    .type testShadowing, @function
testShadowing:
    enter $64, $0
    movq %rdi, -8(%rbp)
    movq $10, %r10
    movq %r10, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq $2, %r10
    movq %r10, -40(%rbp)
    movq -32(%rbp), %r10
    movq -40(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    movq %r10, -56(%rbp)
    movq -32(%rbp), %rax
    jmp .LtestShadowing_epilogue
.LtestShadowing_epilogue:
    leave
    ret
    .size testShadowing, . - testShadowing

    .globl testControl
    .type testControl, @function
testControl:
    enter $240, $0
    movq %rdi, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -16(%rbp)
    movq $0, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
.LL0:
    movq $0, %r10
    movq %r10, -40(%rbp)
    movq -16(%rbp), %r10
    movq -40(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    cmpq $0, %r10
    je .LL1
    movq $2, %r10
    movq %r10, -64(%rbp)
    movq -16(%rbp), %r10
    movq -64(%rbp), %r11
    cmpq %r11, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -72(%rbp)
    movq -72(%rbp), %r10
    cmpq $0, %r10
    je .LL2
    movq $100, %r10
    movq %r10, -88(%rbp)
    movq -32(%rbp), %r10
    movq -88(%rbp), %r11
    addq %r11, %r10
    movq %r10, -96(%rbp)
    movq -96(%rbp), %r10
    movq %r10, -32(%rbp)
    jmp .LL3
.LL2:
    movq $1, %r10
    movq %r10, -104(%rbp)
    movq -32(%rbp), %r10
    movq -104(%rbp), %r11
    addq %r11, %r10
    movq %r10, -112(%rbp)
    movq -112(%rbp), %r10
    movq %r10, -32(%rbp)
.LL3:
    movq $1, %r10
    movq %r10, -120(%rbp)
    movq -16(%rbp), %r10
    movq -120(%rbp), %r11
    subq %r11, %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    movq %r10, -16(%rbp)
    jmp .LL0
.LL1:
    movq $50, %r10
    movq %r10, -136(%rbp)
    movq -32(%rbp), %r10
    movq -136(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -144(%rbp)
    movq -144(%rbp), %r10
    cmpq $0, %r10
    je .LL4
.LL6:
    movq $80, %r10
    movq %r10, -160(%rbp)
    movq -32(%rbp), %r10
    movq -160(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -168(%rbp)
    movq -168(%rbp), %r10
    cmpq $0, %r10
    je .LL7
    movq $10, %r10
    movq %r10, -184(%rbp)
    movq -32(%rbp), %r10
    movq -184(%rbp), %r11
    subq %r11, %r10
    movq %r10, -192(%rbp)
    movq -192(%rbp), %r10
    movq %r10, -32(%rbp)
    jmp .LL6
.LL7:
    jmp .LL5
.LL4:
    movq $0, %r10
    movq %r10, -200(%rbp)
    movq -200(%rbp), %r10
    movq %r10, -32(%rbp)
.LL5:
    movq $0, %r10
    movq %r10, -208(%rbp)
    movq -32(%rbp), %r10
    movq -208(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -216(%rbp)
    movq -216(%rbp), %r10
    cmpq $0, %r10
    je .LL8
    movq -32(%rbp), %rax
    jmp .LtestControl_epilogue
    jmp .LL9
.LL8:
    movq $1, %r10
    movq %r10, -232(%rbp)
    movq -232(%rbp), %r10
    negq %r10
    movq %r10, -240(%rbp)
    movq -240(%rbp), %rax
    jmp .LtestControl_epilogue
.LL9:
.LtestControl_epilogue:
    leave
    ret
    .size testControl, . - testControl

    .globl sideEffect
    .type sideEffect, @function
sideEffect:
    enter $32, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq x(%rip), %r10
    movq -8(%rbp), %r11
    addq %r11, %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %r10
    movq %r10, x(%rip)
    movq $1, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .LsideEffect_epilogue
.LsideEffect_epilogue:
    leave
    ret
    .size sideEffect, . - sideEffect

    .globl main
    .type main, @function
main:
    enter $176, $0
    movq $100, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, x(%rip)
    xorl %eax, %eax
    call emptyCall
    movq $5, %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %rdi
    xorl %eax, %eax
    call identity
    movq %rax, -24(%rbp)
    movq $10, %r10
    movq %r10, -32(%rbp)
    movq $20, %r10
    movq %r10, -40(%rbp)
    movq -32(%rbp), %rdi
    movq -40(%rbp), %rsi
    xorl %eax, %eax
    call sumar
    movq %rax, -48(%rbp)
    movq -24(%rbp), %rdi
    movq -48(%rbp), %rsi
    xorl %eax, %eax
    call sumar
    movq %rax, -56(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -64(%rbp)
    movq -64(%rbp), %rdi
    xorl %eax, %eax
    call testShadowing
    movq %rax, -72(%rbp)
    movq -72(%rbp), %r10
    movq %r10, -64(%rbp)
    movq $3, %r10
    movq %r10, -80(%rbp)
    movq -80(%rbp), %rdi
    xorl %eax, %eax
    call testControl
    movq %rax, -88(%rbp)
    movq -88(%rbp), %r10
    movq %r10, -64(%rbp)
    movq $0, %r10
    movq %r10, -96(%rbp)
    movq -96(%rbp), %r10
    movq %r10, -104(%rbp)
    movq -104(%rbp), %r10
    cmpq $0, %r10
    je .LL10
    xorl %eax, %eax
    call sideEffect
    movq %rax, -120(%rbp)
    movq -120(%rbp), %r10
    movq %r10, -104(%rbp)
.LL10:
    movq -104(%rbp), %r10
    movq %r10, -128(%rbp)
    movq $1, %r10
    movq %r10, -136(%rbp)
    movq -136(%rbp), %r10
    movq %r10, -144(%rbp)
    movq -144(%rbp), %r10
    cmpq $0, %r10
    jne .LL11
    xorl %eax, %eax
    call sideEffect
    movq %rax, -160(%rbp)
    movq -160(%rbp), %r10
    movq %r10, -144(%rbp)
.LL11:
    movq -144(%rbp), %r10
    movq %r10, -168(%rbp)
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl x
    .align 8
    .type x, @object
    .size x, 8
x:
    .zero 8

