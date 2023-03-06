#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {

    // Had to ask ChatGPT how to read a string of unknown length
    char *str;
    int size = 1;
    int i = 0;
    char c;

    str = (char *) malloc(sizeof(char));

    printf("Gib string:\n"); // This is my code

    while ((c = getchar()) != '\n') {
        str[i++] = c;
        if (i == size) {
            size *= 2;
            str = (char *) realloc(str, size * sizeof(char));
        }
    }

    str[i] = '\0';

    // This loop and the printf are also my code
    for (int k = i; 0 <= k; k--) {
        printf("%c", str[k]);
    }
    printf("\n");

    free(str);

    return 0;
}