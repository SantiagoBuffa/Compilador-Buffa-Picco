    .text

    .globl triggerSideEffect
    .type triggerSideEffect, @function
triggerSideEffect:
    enter $32, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq side_effects_count(%rip), %r10
    movq -8(%rbp), %r11
    addq %r11, %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %r10
    movq %r10, side_effects_count(%rip)
    movq $1, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .LtriggerSideEffect_epilogue
.LtriggerSideEffect_epilogue:
    leave
    ret
    .size triggerSideEffect, . - triggerSideEffect

    .globl main
    .type main, @function
main:
    enter $208, $0
    movq $0, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, side_effects_count(%rip)
    movq $0, %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    cmpq $0, %r10
    je .LL0
    xorl %eax, %eax
    call triggerSideEffect
    movq %rax, -40(%rbp)
    movq -40(%rbp), %r10
    movq %r10, -24(%rbp)
.LL0:
    movq -24(%rbp), %r10
    movq %r10, -48(%rbp)
    movq $1, %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -64(%rbp)
    movq -64(%rbp), %r10
    cmpq $0, %r10
    jne .LL1
    xorl %eax, %eax
    call triggerSideEffect
    movq %rax, -80(%rbp)
    movq -80(%rbp), %r10
    movq %r10, -64(%rbp)
.LL1:
    movq -64(%rbp), %r10
    movq %r10, -48(%rbp)
    movq $0, %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    movq %r10, -96(%rbp)
    movq -96(%rbp), %r10
    cmpq $0, %r10
    je .LL3
    xorl %eax, %eax
    call triggerSideEffect
    movq %rax, -112(%rbp)
    movq -112(%rbp), %r10
    movq %r10, -96(%rbp)
.LL3:
    movq -96(%rbp), %r10
    movq %r10, -120(%rbp)
    movq -120(%rbp), %r10
    cmpq $0, %r10
    jne .LL2
    movq $0, %r10
    movq %r10, -136(%rbp)
    movq -136(%rbp), %r10
    movq %r10, -144(%rbp)
    movq -144(%rbp), %r10
    cmpq $0, %r10
    je .LL4
    xorl %eax, %eax
    call triggerSideEffect
    movq %rax, -160(%rbp)
    movq -160(%rbp), %r10
    movq %r10, -144(%rbp)
.LL4:
    movq -144(%rbp), %r10
    movq %r10, -120(%rbp)
.LL2:
    movq -120(%rbp), %r10
    movq %r10, -48(%rbp)
    movq $1, %r10
    movq %r10, -168(%rbp)
    movq -168(%rbp), %r10
    cmpq $0, %r10
    sete %al
    movzbq %al, %r10
    movq %r10, -176(%rbp)
    movq -176(%rbp), %r10
    movq %r10, -184(%rbp)
    movq -184(%rbp), %r10
    cmpq $0, %r10
    je .LL5
    xorl %eax, %eax
    call triggerSideEffect
    movq %rax, -200(%rbp)
    movq -200(%rbp), %r10
    movq %r10, -184(%rbp)
.LL5:
    movq -184(%rbp), %r10
    movq %r10, -48(%rbp)
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl side_effects_count
    .align 8
    .type side_effects_count, @object
    .size side_effects_count, 8
side_effects_count:
    .zero 8

