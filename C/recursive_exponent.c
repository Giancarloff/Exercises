#include <stdio.h>
#include <stdlib.h>

typedef long long llint;

llint rec_exp(llint num, llint exp) {
    if (exp == 1) return num;

    return num * rec_exp(num, exp-1);
}

int main(int argc, char* argv[]) {

    llint num, exp;

    // USAGE: ./main num exp
    if (argc == 3) {

        num = (llint) atoi(argv[1]);
        exp = (llint) atoi(argv[2]);

        printf("%lld\n", rec_exp(num, exp));

    } else { // Or type them here
        printf("NUM: ");
        scanf("%lld", &num);
        
        printf("EXP: ");
        scanf("%lld", &exp);

        printf("%lld ^ %lld = %lld\n", num, exp, rec_exp(num, exp));
    }

    return 1;
}