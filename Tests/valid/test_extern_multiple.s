    .text
    .extern print_int
    .extern sqrt_approx
    .extern is_even

    .globl main
    .type main, @function
main:
    enter $64, $0
    movq $4, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, count(%rip)
    movq count(%rip), %rdi
    xorl %eax, %eax
    call is_even
    movq %rax, -16(%rbp)
    movq -16(%rbp), %r10
    movq %r10, -24(%rbp)
    movq -40(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %rdi
    xorl %eax, %eax
    call sqrt_approx
    movq %rax, -48(%rbp)
    movq -48(%rbp), %r10
    movq %r10, -56(%rbp)
    movq count(%rip), %rdi
    xorl %eax, %eax
    call print_int
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl count
    .align 8
    .type count, @object
    .size count, 8
count:
    .zero 8

