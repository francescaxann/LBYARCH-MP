.text
.globl _asm_dot
.p2align 4

# double asm_dot(int n, const double *a, const double *b)
# macOS calling convention:
#   rdi = n
#   rsi = a
#   rdx = b
# return in xmm0

_asm_dot:
    testq %rdi, %rdi
    je _zero_return

    # zero accumulator: xorps is allowed for xmm registers
    xorps %xmm0, %xmm0

    movq %rdi, %rcx     # loop counter n
    movq %rsi, %r8      # ptr a
    movq %rdx, %r9      # ptr b

.Lloop:
    movsd (%r8), %xmm1
    movsd (%r9), %xmm2
    mulsd %xmm2, %xmm1
    addsd %xmm1, %xmm0

    addq $8, %r8
    addq $8, %r9
    decq %rcx
    jne .Lloop

    ret

_zero_return:
    xorps %xmm0, %xmm0
    ret
