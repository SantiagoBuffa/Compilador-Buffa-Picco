    .text

    .globl computeFour
    .type computeFour, @function
computeFour:
    enter $80, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq %rdx, -24(%rbp)
    movq %rcx, -32(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -40(%rbp)
    movq -24(%rbp), %r10
    movq -32(%rbp), %r11
    subq %r11, %r10
    movq %r10, -48(%rbp)
    movq -40(%rbp), %r10
    movq -48(%rbp), %r11
    imulq %r11, %r10
    movq %r10, -56(%rbp)
    movq $2, %r10
    movq %r10, -64(%rbp)
    movq -56(%rbp), %rax
    cqto
    movq -64(%rbp), %r10
    idivq %r10
    movq %rax, -72(%rbp)
    movq -72(%rbp), %rax
    jmp .LcomputeFour_epilogue
.LcomputeFour_epilogue:
    leave
    ret
    .size computeFour, . - computeFour

    .globl addTwo
    .type addTwo, @function
addTwo:
    enter $32, $0
    movq %rdi, -8(%rbp)
    movq %rsi, -16(%rbp)
    movq -8(%rbp), %r10
    movq -16(%rbp), %r11
    addq %r11, %r10
    movq %r10, -24(%rbp)
    movq -24(%rbp), %rax
    jmp .LaddTwo_epilogue
.LaddTwo_epilogue:
    leave
    ret
    .size addTwo, . - addTwo

    .globl main
    .type main, @function
main:
    enter $112, $0
    movq $10, %r10
    movq %r10, -8(%rbp)
    movq $20, %r10
    movq %r10, -16(%rbp)
    movq $30, %r10
    movq %r10, -24(%rbp)
    movq $40, %r10
    movq %r10, -32(%rbp)
    movq -8(%rbp), %rdi
    movq -16(%rbp), %rsi
    movq -24(%rbp), %rdx
    movq -32(%rbp), %rcx
    xorl %eax, %eax
    call computeFour
    movq $1, %r10
    movq %r10, -40(%rbp)
    movq $2, %r10
    movq %r10, -48(%rbp)
    movq -40(%rbp), %rdi
    movq -48(%rbp), %rsi
    xorl %eax, %eax
    call addTwo
    movq %rax, -56(%rbp)
    movq $3, %r10
    movq %r10, -64(%rbp)
    movq $4, %r10
    movq %r10, -72(%rbp)
    movq -64(%rbp), %rdi
    movq -72(%rbp), %rsi
    xorl %eax, %eax
    call addTwo
    movq %rax, -80(%rbp)
    movq $10, %r10
    movq %r10, -88(%rbp)
    movq $5, %r10
    movq %r10, -96(%rbp)
    movq -56(%rbp), %rdi
    movq -80(%rbp), %rsi
    movq -88(%rbp), %rdx
    movq -96(%rbp), %rcx
    xorl %eax, %eax
    call computeFour
    movq %rax, -104(%rbp)
    movq -104(%rbp), %r10
    movq %r10, -112(%rbp)
    movq $0, %rax
    jmp .Lmain_epilogue
    movq $0, %rax
.Lmain_epilogue:
    leave
    ret
    .size main, . - main

