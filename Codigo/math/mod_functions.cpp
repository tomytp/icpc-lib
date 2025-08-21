// Modular Arithmetic Helpers
//
// Provides modular add/sub/mul, fast exponentiation, and modular inverse under fixed MOD.
//
// complexity: O(log E) for power/inverse, O(1)

const ll MOD = 1'000'000'007;

inline ll sum(ll a, ll b) { a += b; if (a >= MOD) a -= MOD; return a; }
inline ll sub(ll a, ll b) { a -= b; if (a < 0)  a += MOD; return a; }
inline ll mult(ll a, ll b) { return (a * b) % MOD; }

inline ll pot(ll base, ll exp) {
    ll res = 1;
    while (exp) {
        if (exp & 1) res = mult(res, base);
        base = mult(base, base);
        exp >>= 1;
    }
    return res;
}

inline ll inv_mod(ll a) {return pot(a, MOD-2);}
