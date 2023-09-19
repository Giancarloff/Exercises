#include <stdio.h>

int factorial(int n) {

    if (n < 0) return 0;
    if (n == 0 || n == 1) return 1;
    return n*factorial(n-1);
}

int main() {

    int n;

    printf("INT: %ld\n", sizeof(int));

    printf("Give me an integer:\n");
    scanf("%d", &n);

    int f = factorial(n);

    printf("The factorial of %d is %d.\n", n, f);

    return 0;
}