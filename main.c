#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <math.h>

extern double dotproduct_asm(double* A, double* B, long long n);
double dotproduct_c(double* A, double* B, long long n);

double get_time() {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

void initialize_vectors(double* A, double* B, long long n) {
    srand((unsigned int)time(NULL));
    for (long long i = 0; i < n; i++) {
        A[i] = ((double)rand() / RAND_MAX) * 10.0;
        B[i] = ((double)rand() / RAND_MAX) * 10.0;
    }
}

int are_equal(double a, double b) {
    double epsilon = 1e-9;
    return fabs(a - b) < epsilon;
}

void run_benchmark(long long n) {
    printf("\n========================================\n");
    printf("Vector Size: n = 2^%d = %lld elements\n", (int)log2((double)n), n);
    printf("========================================\n");

    // Allocate memory for vectors
    double* A = (double*)malloc(n * sizeof(double));
    double* B = (double*)malloc(n * sizeof(double));

    if (A == NULL || B == NULL) {
        printf("Memory allocation failed for n = %lld\n", n);
        if (A) free(A);
        if (B) free(B);
        return;
    }

    // Initialize vectors
    initialize_vectors(A, B, n);
    printf("Vectors initialized with random values.\n\n");

    // Variables for timing
    const int RUNS = 20;
    double total_time_c = 0.0;
    double total_time_asm = 0.0;
    double result_c = 0.0;
    double result_asm = 0.0;

    // Benchmark C version
    printf("Running C kernel (20 iterations)...\n");
    for (int i = 0; i < RUNS; i++) {
        double start = get_time();
        result_c = dotproduct_c(A, B, n);
        double end = get_time();
        total_time_c += (end - start);
    }
    double avg_time_c = total_time_c / RUNS;
    printf("C Kernel Result: %.10f\n", result_c);
    printf("C Kernel Average Time: %.6f seconds\n\n", avg_time_c);

    // Benchmark x86-64 Assembly version
    printf("Running x86-64 Assembly kernel (20 iterations)...\n");
    for (int i = 0; i < RUNS; i++) {
        double start = get_time();
        result_asm = dotproduct_asm(A, B, n);
        double end = get_time();
        total_time_asm += (end - start);
    }
    double avg_time_asm = total_time_asm / RUNS;
    printf("Assembly Kernel Result: %.10f\n", result_asm);
    printf("Assembly Kernel Average Time: %.6f seconds\n\n", avg_time_asm);

    // Correctness check
    printf("Correctness Check:\n");
    if (are_equal(result_c, result_asm)) {
        printf("✓ PASS: C and Assembly results match!\n");
        printf("  Difference: %.15e\n", fabs(result_c - result_asm));
    } else {
        printf("✗ FAIL: Results do not match!\n");
        printf("  C Result:        %.10f\n", result_c);
        printf("  Assembly Result: %.10f\n", result_asm);
        printf("  Difference:      %.10f\n", fabs(result_c - result_asm));
    }

    // Performance comparison
    printf("\nPerformance Comparison:\n");
    double speedup = avg_time_c / avg_time_asm;
    printf("C vs Assembly speedup: %.2fx\n", speedup);
    if (speedup > 1.0) {
        printf("Assembly is %.2f%% faster than C\n", (speedup - 1.0) * 100.0);
    } else if (speedup < 1.0) {
        printf("C is %.2f%% faster than Assembly\n", (1.0 / speedup - 1.0) * 100.0);
    }

    // Free memory
    free(A);
    free(B);
}

int main() {
    printf("============================================================\n");
    printf("DOT PRODUCT BENCHMARK: C vs x86-64 Assembly (SIMD)\n");
    printf("============================================================\n");
    printf("Using scalar SIMD registers (XMM) and SIMD instructions\n");
    printf("Testing with 20 runs per configuration\n");

    // Test different vector sizes
    long long sizes[] = {
        1LL << 20,  // 2^20
        1LL << 24,  // 2^24
        1LL << 28   // 2^28 
    };

    for (int i = 0; i < 3; i++) {
        run_benchmark(sizes[i]);
    }

    printf("\n============================================================\n");
    printf("Benchmark completed successfully!\n");
    printf("============================================================\n");

    return 0;
}
