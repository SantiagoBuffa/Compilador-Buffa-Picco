    .text

    .globl calc
    .type calc, @function
calc:
    enter $272, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq $42, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    negq %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    negq %r10
    movq %r10, -64(%rbp)
    movq -64(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -8(%rbp), %r10
    negq %r10
    movq %r10, -80(%rbp)
    movq -80(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -16(%rbp), %r10
    negq %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -8(%rbp), %r10
    negq %r10
    movq %r10, -96(%rbp)
    movq -96(%rbp), %r10
    negq %r10
    movq %r10, -104(%rbp)
    movq -104(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -8(%rbp), %r10
    negq %r10
    movq %r10, -112(%rbp)
    movq -112(%rbp), %r10
    negq %r10
    movq %r10, -120(%rbp)
    movq -120(%rbp), %r10
    negq %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    movq %r10, -40(%rbp)
    movq $10, %r10
    movq %r10, -136(%rbp)
    movq -8(%rbp), %r10
    movq -136(%rbp), %r11
    addq %r11, %r10
    movq %r10, -144(%rbp)
    movq -144(%rbp), %r10
    negq %r10
    movq %r10, -152(%rbp)
    movq -152(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -168(%rbp), %r10
    movq %r10, -160(%rbp)
    movq -16(%rbp), %r10
    movq -160(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -176(%rbp)
    movq -176(%rbp), %r10
    negq %r10
    movq %r10, -184(%rbp)
    movq -184(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -40(%rbp), %r10
    negq %r10
    movq %r10, -192(%rbp)
    movq -8(%rbp), %r10
    movq -192(%rbp), %r11
    subq %r11, %r10
    movq %r10, -200(%rbp)
    movq -200(%rbp), %r10
    movq %r10, -40(%rbp)
    movq $5, %r10
    movq %r10, -208(%rbp)
    movq -208(%rbp), %r10
    negq %r10
    movq %r10, -216(%rbp)
    movq -8(%rbp), %r10
    movq -216(%rbp), %r11
    addq %r11, %r10
    movq %r10, -224(%rbp)
    movq $2, %r10
    movq %r10, -232(%rbp)
    movq -40(%rbp), %r10
    movq -232(%rbp), %r11
    subq %r11, %r10
    movq %r10, -240(%rbp)
    movq -240(%rbp), %r10
    negq %r10
    movq %r10, -248(%rbp)
    movq -224(%rbp), %r10
    movq -248(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -256(%rbp)
    movq -256(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %r10
    negq %r10
    movq %r10, -264(%rbp)
    movq -264(%rbp), %rax
    jmp .Lcalc_epilogue
.Lcalc_epilogue:
    leave
    ret
    .size calc, . - calc

    .globl main
    .type main, @function
main:
    enter $160, $0
    movq $100, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    negq %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %r10
    movq %r10, g_int(%rip)
    movq -32(%rbp), %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    negq %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %r10
    movq %r10, g_flt(%rip)
    movq $7, %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    negq %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -64(%rbp)
    movq -80(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -72(%rbp), %r10
    negq %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    movq %r10, -96(%rbp)
    movq -64(%rbp), %r10
    negq %r10
    movq %r10, -104(%rbp)
    movq -120(%rbp), %r10
    movq %r10, -112(%rbp)
    movq -96(%rbp), %r10
    movq -112(%rbp), %r11
    addq %r11, %r10
    movq %r10, -128(%rbp)
    movq -128(%rbp), %r10
    negq %r10
    movq %r10, -136(%rbp)
    movq -104(%rbp), %rdi
    movq -136(%rbp), %rsi
    xorl %eax, %eax
    call calc
    movq %rax, -144(%rbp)
    movq -144(%rbp), %r10
    movq %r10, -152(%rbp)
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl g_int
    .align 8
    .type g_int, @object
    .size g_int, 8
g_int:
    .zero 8

    .globl g_flt
    .align 8
    .type g_flt, @object
    .size g_flt, 8
g_flt:
    .zero 8

