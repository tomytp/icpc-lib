// Extended Euclidean Algorithm
// Retorna g = mdc(a,b) e encontra x,y tais que ax+by=g.
// Requer a,b >= 0.

ll euclid(ll a, ll b, ll &x, ll &y) {
	if (!b) return x = 1, y = 0, a;
	ll d = euclid(b, a % b, y, x);
	return y -= a/b * x, d;
}