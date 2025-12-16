#include <stdlib.h>
#include <stdio.h>

int triple(int num) {
    return num*3;
}

int square(int num) {
    return num*num;
}

void transform(int val, int (*fn)(int)) {
    int result = fn(val);
    printf("Result: %i\n", result);
}

void combine(int val1, int val2, double (*fn)(int, int)) {
    double result = fn(val1, val2);
    printf("Result: %f\n", result);
}

double addAndHalve(int val1, int val2) {
    return ((double) val1 + val2) / 2;
}

int main() {
    transform(4, triple);
    transform(4, square);
    combine(3, 4, addAndHalve);
}