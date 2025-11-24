#include <emmintrin.h>

double dotproduct_c(double* A, double* B, long long n) {
    double sdot = 0.0;
    
    __m128d xmm_sum = _mm_setzero_pd();  // Initialize sum to 0.0
    
    // Process elements one at a time using scalar SIMD operations
    for (long long i = 0; i < n; i++) {
        // Load scalar values into XMM registers (scalar SIMD)
        __m128d xmm_a = _mm_load_sd(&A[i]);
        __m128d xmm_b = _mm_load_sd(&B[i]);
        
        // Multiply: scalar double-precision multiplication
        __m128d xmm_prod = _mm_mul_sd(xmm_a, xmm_b);  // mulsd
        
        // Add to accumulator: scalar double-precision addition
        xmm_sum = _mm_add_sd(xmm_sum, xmm_prod);
    }
    
    // Store the result from XMM register to memory
    _mm_store_sd(&sdot, xmm_sum);
    
    return sdot;
}
