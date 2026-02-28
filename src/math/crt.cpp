// Chinese Remainder Theorem
// se x = a(mod m) e x = b(mod n) então existe unico c<mmc(m,n) tal que x = c(mod mmc(m,n))
// se e só se mdc(m,n)| (b-a)
// essa funcao acha c e é -INF se for impossivel

#include "src/math/euclid.cpp"

ll crt(ll a, ll m, ll b, ll n) {
	if (n > m) swap(a, b), swap(m, n);
	ll x, y, g = euclid(m, n, x, y);
    if((a - b) % g != 0) return -INF; //no solution
	x = (b - a) % n * x % n / g * m + a;
	return x < 0 ? x + m*n/g : x;
}


