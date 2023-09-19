#include <iostream>
using namespace std;

// In this specific case I had to consider the zero-eth fibonacci number as 0, hence
// the code below.
int fib(int n) { 
    if (n == 0) return 0;
    if (n < 2) return 1;
    return fib(n-1) + fib(n-2);
}

// Checking
int main() {

    cout << fib(0) << endl;
    cout << fib(1) << endl;
    cout << fib(16) << endl;
    cout << fib(17) << endl;

    return 0;
}