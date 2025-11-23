#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <immintrin.h> // Required for SIMD intrinsics

// C Kernel using SIMD Intrinsics 
double c_dot_simd(int n, const double *a, const double *b) {
    // xmm0 will accumulate the dot product (sdot)
    __m128d sdot_vec = _mm_setzero_pd(); 
    
    // Loop must be unrolled or vectorized for proper performance measurement
    for (int i = 0; i < n; i++) {
        // Load a_i into the low 64 bits of xmm1
        __m128d a_i_vec = _mm_load_sd(&a[i]); 
        
        // Load b_i into the low 64 bits of xmm2
        __m128d b_i_vec = _mm_load_sd(&b[i]); 
        
        // Multiply: xmm1 = a_i * b_i (Scalar Double-Precision)
        __m128d product = _mm_mul_sd(a_i_vec, b_i_vec); 
        
        // Accumulate: xmm0 = xmm0 + product (Scalar Double-Precision)
        sdot_vec = _mm_add_sd(sdot_vec, product); 
    }
    
    // Extract the final scalar result from the XMM register
    double sdot;
    _mm_store_sd(&sdot, sdot_vec);
    
    return sdot;
}

// Declaration of Assembly Function (Windows x64)
// The function name should match the label used in the assembly file: _asm_dot
extern double _asm_dot(int n, const double *a, const double *b);


// Main Program (:
int main(void) {
    // use n = 2^20, 2^24, 2^30 (or max supported) for the final analysis.
    int n = 1000; // ex size for a quick test
    
    // Allocate and initialize vectors A and B
    double *a = (double *)malloc(n * sizeof(double));
    double *b = (double *)malloc(n * sizeof(double));
    
    if (!a || !b) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // int with sample values for testing (you must use random for final project)
    for (int i = 0; i < n; ++i) {
        a[i] = (double)(i % 5 + 1); // 1.0, 2.0, 3.0, 4.0, 5.0, 1.0, ...
        b[i] = (double)(i % 5 + 1);
    }

    // EXECUTE C INTRINSIC KERNEL 
    double result_c = c_dot_simd(n, a, b);

    // EXECUTE ASSEMBLY KERNEL (Windows x64)
    double result_asm = _asm_dot(n, a, b);

    printf("Vector Length (n): %d\n", n);
    printf("C (Intrinsic) Result : %.10f\n", result_c);
    printf("ASM (x64 Win) Result : %.10f\n", result_asm);

    // FOOR CORRECTNESS CHECK
    // Use a small tolerance for floating-point comparison
    double tolerance = 1e-9;
    if (fabs(result_c - result_asm) < tolerance) {
        printf("\n Results MATCH (within tolerance %.1e). Correctness check passed.\n", tolerance);
    } else {
        printf("\nResults DIFFER. Correctness check FAILED.\n");
    }
    
    free(a);
    free(b);
    return 0;
}
