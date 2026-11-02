// Chinese Remainder Theorem
//
// Dados x = a (mod m) e x = b (mod n), com 0 <= a < m e 0 <= b < n, acha o unico
// c < mmc(m, n) com x = c (mod mmc(m, n)). Existe se e so se mdc(m, n) divide b - a;
// senao retorna -INF.
//
// complexity: O(log(min(m, n)))

#include "src/math/euclid.cpp"

ll crt(ll a, ll m, ll b, ll n) {
    if (n > m) swap(a, b), swap(m, n);

    ll x, y, g = euclid(m, n, x, y);
    if ((a - b) % g != 0) return -INF; // não tem solução

    __int128 k = (__int128)((b-a)/g) * x;
    k %= n/g;
    if (k < 0) k += n/g;

    return (ll)((__int128)m*k + a);
}