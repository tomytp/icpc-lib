// Discrete Logarithm (Baby-step Giant-step)
//
// Finds the smallest x >= 1 such that a^x = b (mod m),
// or returns -1 if no solution exists.
//
// Works with composite moduli and does not require gcd(a,m) = 1.
//
// Preconditions:
// - m > 0.
//
// Uses __int128 for exact modular multiplication.
//
// complexity:
// - Time: O(sqrt(m) * log m).
// - Memory: O(sqrt(m)).

ll mod_log(ll a, ll b, ll m) {
    assert(m > 0);

    a %= m;
    b %= m;
    if (a < 0) a += m;
    if (b < 0) b += m;

    ll n = (ll)sqrtl((long double)m) + 1;

    auto mul = [&](ll x, ll y) -> ll {
        return (ll)((__int128)x * y % m);
    };

    ll e = 1 % m, f = 1 % m, j = 1;
    map<ll, ll> mp;

    while (j <= n) {
        e = f = mul(e, a);
        if (e == b) return j;
        mp[mul(e, b)] = j++;
    }

    if (gcd(m, e) == gcd(m, b)) {
        forn(i, 2, n+2) {
            e = mul(e, f);
            if (mp.count(e))
                return (ll)((__int128)n * i - mp[e]);
        }
    }

    return -1;
}