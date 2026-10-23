    .text

    .globl test_ast
    .type test_ast, @function
test_ast:
    enter $128, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq $1, %r10
    movq %r10, -24(%rbp)
    movq $2, %r10
    movq %r10, -32(%rbp)
    movq $3, %r10
    movq %r10, -40(%rbp)
    movq -32(%rbp), %r10
    movq -40(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -48(%rbp)
    movq -24(%rbp), %r10
    movq -48(%rbp), %r11
    addq %r11, %r10
    movq %r10, -56(%rbp)
    movq -56(%rbp), %r10
    movq %r10, -64(%rbp)
    movq -16(%rbp), %r10
    cmpq $0, %r10
    je .LL0
.LL2:
    movq $0, %r10
    movq %r10, -80(%rbp)
    movq -64(%rbp), %r10
    movq -80(%rbp), %r11
    cmpq %r11, %r10
    setg %al
    movzbq %al, %r10
    movq %r10, -88(%rbp)
    movq -88(%rbp), %r10
    cmpq $0, %r10
    je .LL3
    movq $1, %r10
    movq %r10, -104(%rbp)
    movq -64(%rbp), %r10
    movq -104(%rbp), %r11
    subq %r11, %r10
    movq %r10, -112(%rbp)
    movq -112(%rbp), %r10
    movq %r10, -64(%rbp)
    jmp .LL2
.LL3:
    jmp .LL1
.LL0:
    movq $0, %r10
    movq %r10, -120(%rbp)
    movq -64(%rbp), %rdi
    movq -120(%rbp), %rsi
    xorl %eax, %eax
    call test_ast
.LL1:
    jmp .Ltest_ast_epilogue
.Ltest_ast_epilogue:
    leave
    ret
    .size test_ast, . - test_ast

    .globl main
    .type main, @function
main:
    enter $16, $0
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl global_var
    .align 8
    .type global_var, @object
    .size global_var, 8
global_var:
    .zero 8

    .globl global_var2
    .align 8
    .type global_var2, @object
    .size global_var2, 8
global_var2:
    .zero 8

    .globl f
    .align 8
    .type f, @object
    .size f, 8
f:
    .zero 8

