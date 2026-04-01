// Primitive Root (Generator)
//
// Finds the smallest primitive root modulo a prime P.
// Factorizes P-1 and checks that for every prime factor F
// of P-1, the candidate G satisfies G^((P-1)/F) != 1 (mod P).
//
// complexity: O(time_to_factor(p-1) + ans * |factors(P-1)| * log P)
// can be adapted to p not prime

#include "src/math/mod_functions.cpp"

ll primitive_root(ll p){
    vll fact;
    ll phi = p-1,  n = phi;
    // mudar p-1 para caso nao primo
    for (ll i=2; i*i<=n; i++)
        if (n % i == 0) {
            fact.push_back (i);
            while (n % i == 0)
                n /= i;
        }
    if (n > 1)
        fact.push_back(n);

    forn(res,2, p+1) {
        bool ok = true;
        for (ll i=0; i<sz(fact) && ok; ++i)
            ok &= modpow (res, phi / fact[i], p) != 1;
        if (ok)  return res;
    }
    return -1;
}
