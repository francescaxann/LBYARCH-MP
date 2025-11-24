.code
dotproduct_asm proc
    ; Initialize sum to 0.0 in XMM0 (scalar SIMD register)
    xorpd xmm0, xmm0        ; XMM0 = 0.0 (clear XMM0)
    
    ; Check if n <= 0
    test r8, r8
    jle done                ; if n <= 0, return 0.0
    
    ; RCX = pointer to A
    ; RDX = pointer to B
    ; R8  = counter (n)
    ; XMM0 = accumulator (sum)
    
loop_start:
    ; Load scalar double from A[i] into XMM1
    movsd xmm1, qword ptr [rcx]     ; XMM1 = A[i] (scalar load)
    
    ; Load scalar double from B[i] into XMM2
    movsd xmm2, qword ptr [rdx]     ; XMM2 = B[i] (scalar load)
    
    ; Multiply A[i] * B[i] - scalar double-precision multiplication
    mulsd xmm1, xmm2                ; XMM1 = A[i] * B[i] (scalar multiply)
    
    ; Add to accumulator - scalar double-precision addition
    addsd xmm0, xmm1                ; XMM0 += A[i] * B[i] (scalar add)
    
    ; Move to next elements
    add rcx, 8                      ; Move pointer A to next double (8 bytes)
    add rdx, 8                      ; Move pointer B to next double (8 bytes)
    
    ; Decrement counter and loop
    dec r8                          ; n--
    jnz loop_start                  ; if n != 0, continue loop
    
done:
    ; Result is already in XMM0 (return value for double)
    ret
dotproduct_asm endp
end
