#include <stdio.h>

int main () {

    int a, b;

    printf("a:\n");
    scanf("%d", &a);
    printf("b:\n");
    scanf("%d", &b);

    printf("%d at %p \n", a, &a);
    printf("%d at %p \n", b, &b);

    int *pa = &a, *pb = &b;
    int c;
    c = *pa;
    *pa = *pb;
    *pb = c;

    printf("%d at %p \n", a, &a);
    printf("%d at %p \n", b, &b);

    return 0;
}