// Extended Eucidian Algorithm
// Acha mdc(a,b) e x e y que satisfazem a identidade de bezout

ll euclid(ll a, ll b, ll &x, ll &y) {
	if (!b) return x = 1, y = 0, a;
	ll d = euclid(b, a % b, y, x);
	return y -= a/b * x, d;
}