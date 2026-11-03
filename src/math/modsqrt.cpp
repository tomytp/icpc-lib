// Discrete Square Root
//
// Tonelli-Shanks: finds x such that x^2 = a (mod p),
// or returns -1 if no solution exists.
//
// Preconditions:
// - p is prime.
// - 2 <= p <= LLONG_MAX.
//
// Uses exact modular multiplication through mod_functions.cpp.
//
// complexity:
// - Tonelli-Shanks: O(log^2 p) modular operations.
// - Finding a quadratic non-residue: additional O(q log p),
//   where q is the number of candidates tested.

#include "src/math/mod_functions.cpp"

ll mod_sqrt(ll a, ll p) {
    assert(p >= 2);

    a %= p;
    if (a < 0) a += p;

    if (p == 2) return a;
    if (a == 0) return 0;

    if (modpow(a, (p-1)/2, p) != 1) return -1;

    if (p % 4 == 3)
        return modpow(a, p/4 + 1, p);

    auto mul = [&](ll x, ll y) -> ll {
        return (ll)((__int128)x * y % p);
    };

    ll s = p-1, z = 2;
    int r = 0, m;

    while (s % 2 == 0)
        ++r, s /= 2;

    // Find a quadratic non-residue.
    while (modpow(z, (p-1)/2, p) != p-1)
        ++z;

    ll x = modpow(a, (s+1)/2, p);
    ll b = modpow(a, s, p);
    ll g = modpow(z, s, p);

    for (;;) {
        ll t = b;

        for (m = 0; m < r && t != 1; ++m)
            t = mul(t, t);

        if (m == 0) return x;

        assert(m < r);

        ll gs = modpow(g, 1LL << (r-m-1), p);

        g = mul(gs, gs);
        x = mul(x, gs);
        b = mul(b, g);
    }
}