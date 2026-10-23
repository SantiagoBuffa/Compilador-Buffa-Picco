#include <stdio.h>

void c_print(long val) {
    printf("[C RUNTIME OUTPUT: %ld]\n", val);
}

long c_multiply(long a, long b) {
    return a * b;
}
