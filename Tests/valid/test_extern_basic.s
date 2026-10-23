    .text
    .extern suma

    .globl main
    .type main, @function
main:
    enter $48, $0
    movq $10, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -16(%rbp)
    movq $20, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -16(%rbp), %rdi
    movq -32(%rbp), %rsi
    xorl %eax, %eax
    call suma
    movq %rax, -40(%rbp)
    movq -40(%rbp), %r10
    movq %r10, global_res(%rip)
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl global_res
    .align 8
    .type global_res, @object
    .size global_res, 8
global_res:
    .zero 8

