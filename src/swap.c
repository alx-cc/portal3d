#include "swap.h"

void int_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void float_swap(float* a, float* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

