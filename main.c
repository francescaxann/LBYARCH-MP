#include <stdio.h>
#include <stdlib.h>

/* C version of dot product */
double c_dot(int n, const double *a, const double *b) {
    double s = 0.0;
    for (int i = 0; i < n; ++i) {
        s += a[i] * b[i];
    }
    return s;
}

/* Declaration of assembly function implemented in asm_dot.s
   double asm_dot(int n, const double *a, const double *b);
*/
extern double asm_dot(int n, const double *a, const double *b);

int main(void) {
    /* Example: you can change n and the vector contents as needed. */
    int n = 5;
    double a[] = {1.0, 2.0, 3.0, 4.0, 0.0};
    double b[] = {1.0, 2.0, 3.0, 4.0, 0.0};

    /* compute with C version */
    double result_c = c_dot(n, a, b);

    /* compute with assembly version */
    double result_asm = asm_dot(n, a, b);

    printf("C dot result     : %.10f\n", result_c);
    printf("ASM dot result   : %.10f\n", result_asm);

    if (result_c == result_asm) {
        printf("Results MATCH\n");
    } else {
        printf("Results DIFFER\n");
    }

    return 0;
}
