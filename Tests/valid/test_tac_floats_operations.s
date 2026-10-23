    .text

    .globl operar
    .type operar, @function
operar:
    enter $80, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -24(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    subq %r11, %r10
    movq %r10, -32(%rbp)
    movq -24(%rbp), %r10
    movq -32(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -40(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -40(%rbp), %rax
    cqto
    movq -48(%rbp), %r10
    idivq %r10
    movq %rax, -64(%rbp)
    movq -64(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -72(%rbp), %rax
    jmp .Loperar_epilogue
.Loperar_epilogue:
    leave
    ret
    .size operar, . - operar

    .globl comparar
    .type comparar, @function
comparar:
    enter $32, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    cmpq %r11, %r10
    setl %al
    movzbq %al, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .Lcomparar_epilogue
.Lcomparar_epilogue:
    leave
    ret
    .size comparar, . - comparar

    .globl esIgual
    .type esIgual, @function
esIgual:
    enter $32, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    cmpq %r11, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .LesIgual_epilogue
.LesIgual_epilogue:
    leave
    ret
    .size esIgual, . - esIgual

    .globl main
    .type main, @function
main:
    enter $144, $0
    movq -16(%rbp), %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, global_f(%rip)
    movq -32(%rbp), %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -40(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    negq %r10
    movq %r10, -64(%rbp)
    movq -64(%rbp), %r10
    movq %r10, -72(%rbp)
    movq -40(%rbp), %rdi
    movq -72(%rbp), %rsi
    xorl %eax, %eax
    call operar
    movq %rax, -80(%rbp)
    movq -80(%rbp), %r10
    movq %r10, -88(%rbp)
    movq -40(%rbp), %rdi
    movq global_f(%rip), %rsi
    xorl %eax, %eax
    call comparar
    movq %rax, -96(%rbp)
    movq -96(%rbp), %r10
    movq %r10, -104(%rbp)
    movq -120(%rbp), %r10
    movq %r10, -112(%rbp)
    movq -88(%rbp), %rdi
    movq -112(%rbp), %rsi
    xorl %eax, %eax
    call esIgual
    movq %rax, -128(%rbp)
    movq -128(%rbp), %r10
    movq %r10, -136(%rbp)
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl global_f
    .align 8
    .type global_f, @object
    .size global_f, 8
global_f:
    .zero 8

