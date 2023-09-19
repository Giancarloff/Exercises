#include <stdio.h>

int main() {

    int n, sum, i, temp;

    printf("N:\n");
    scanf("%d", &n);

    int list[n];

    printf("Give %d ints:\n", n);

    for (i = 0; i < n; i++) {
        scanf("%d", &temp);
        list[i] = temp;
    }

    for (i = 0; i < n; i++) {
        sum += list[i];
    }

    printf("\nThe average is: %f\n", (float) sum/n);

    return 0;
}