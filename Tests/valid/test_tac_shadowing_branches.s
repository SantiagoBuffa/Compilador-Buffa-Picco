    .text

    .globl testParallelBranches
    .type testParallelBranches, @function
testParallelBranches:
    enter $48, $0
    movq %rdi, -8(%rbp)
    movq -8(%rbp), %r10
    cmpq $0, %r10
    je .LL0
    movq $10, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq -32(%rbp), %r10
    movq %r10, global_val(%rip)
    jmp .LL1
.LL0:
    movq $20, %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    movq %r10, global_val(%rip)
.LL1:
.LtestParallelBranches_epilogue:
    leave
    ret
    .size testParallelBranches, . - testParallelBranches

    .globl testMultiLevelNesting
    .type testMultiLevelNesting, @function
testMultiLevelNesting:
    enter $48, $0
    movq $1, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, -16(%rbp)
    movq $2, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %r10
    movq %r10, -32(%rbp)
    movq $3, %r10
    movq %r10, -40(%rbp)
    movq -40(%rbp), %r10
    movq %r10, -48(%rbp)
    movq -48(%rbp), %r10
    movq %r10, global_val(%rip)
    movq -32(%rbp), %r10
    movq %r10, global_val(%rip)
    movq -16(%rbp), %r10
    movq %r10, global_val(%rip)
.LtestMultiLevelNesting_epilogue:
    leave
    ret
    .size testMultiLevelNesting, . - testMultiLevelNesting

    .globl main
    .type main, @function
main:
    enter $32, $0
    movq $0, %r10
    movq %r10, -8(%rbp)
    movq -8(%rbp), %r10
    movq %r10, global_val(%rip)
    movq $1, %r10
    movq %r10, -16(%rbp)
    movq -16(%rbp), %rdi
    xorl %eax, %eax
    call testParallelBranches
    movq $0, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rdi
    xorl %eax, %eax
    call testParallelBranches
    xorl %eax, %eax
    call testMultiLevelNesting
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

    .bss
    .globl global_val
    .align 8
    .type global_val, @object
    .size global_val, 8
global_val:
    .zero 8

