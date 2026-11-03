// Modular Arithmetic Helpers
//
// modmul(a, b, M): computes (a*b) % M exactly.
// modpow(b, e, M): computes b^e % M using binary exponentiation.
//
// Preconditions:
// - M > 0.
// - modmul supports arbitrary ull operands.
// - modpow returns a value in [0, M).
//
// Uses __int128 for exact 64-bit modular multiplication.
//
// complexity:
// - modmul: O(1) machine operations.
// - modpow: O(log e) modular multiplications.

typedef unsigned long long ull;

ull modmul(ull a, ull b, ull M) {
    return (unsigned __int128)a * b % M;
}

ull modpow(ull b, ull e, ull mod){
    ull ans = 1;
    for(; e; b = modmul(b, b, mod), e /= 2)
        if(e & 1) ans = modmul(ans, b, mod);
    return ans;
}
