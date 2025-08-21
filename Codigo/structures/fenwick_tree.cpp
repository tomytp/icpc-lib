// Fenwick Tree (Binary Indexed Tree)
//
// Supports point updates and prefix/range sum queries in logarithmic time using a 1-indexed BIT.
//
// complexity: O(log N) per op, O(N)

struct Bit {
	ll n;
	v64 bit;
	Bit(ll _n=0) : n(_n), bit(n + 1) {}
	Bit(v64& v) : n(v.size()), bit(n + 1) {
		for (ll i = 1; i <= n; i++) {
			bit[i] += v[i - 1];
			ll j = i + (i & -i);
			if (j <= n) bit[j] += bit[i];
		}
	}
	void update(ll i, ll x) { // soma x na posicao i
		for (i++; i <= n; i += i & -i) bit[i] += x;
	}
	ll pref(ll i) { // soma [0, i]
		ll ret = 0;
		for (i++; i; i -= i & -i) ret += bit[i];
		return ret;
	}
	ll query(ll l, ll r) {  // soma [l, r]
		return pref(r) - pref(l - 1); 
	}
	ll upper_bound(ll x) {
		ll p = 0;
		for (ll i = __lg(n); i+1; i--) 
			if (p + (1<<i) <= n and bit[p + (1<<i)] <= x)
				x -= bit[p += (1 << i)];
		return p;
	}
};
