// Miller-Rabin Primality Test (Deterministic for 64-bit)
//
// Checks whether n is prime using the seven deterministic bases:
// {2, 325, 9375, 28178, 450775, 9780504, 1795265022}.
//
// Correct for every n < 2^64.
//
// Requires:
// - modmul and modpow from mod_functions.cpp.
// - Exact modular multiplication.
//
// complexity:
// - O(log n) modular multiplications per query (7 bases).

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