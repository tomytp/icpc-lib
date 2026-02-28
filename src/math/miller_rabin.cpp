// Miller-Rabin Primality Test (Deterministic for 64-bit)
//
// Checks if a number N is prime using a fixed set of bases that 
// guarantees correctness for any N < 2^64. 
// Uses a long double trick for O(1) modular multiplication.
//
// complexity: O(7 log^3 N) per query

#include "src/math/mod_functions.cpp"

bool isPrime(ull n){
    if(n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    ull A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}, s = __builtin_ctzll(n-1), d = n >> s;
    for (ull a : A) {
        ull p = modpow(a%n, d, n), i = s;
        while(p != 1 && p != n-1 && a % n && i--)
            p = modmul(p, p, n);
        if(p != n-1 && i != s) return 0;
    } 
    return 1;
}