// Persistent Segment Tree - O(log n) per query/update
// Supports point updates and range queries across versions

struct Node {
	ll val;
	ll l, r;
};

struct perseg {
	ll n, cnt = 0;
	vector<Node> nd;
	vll rt;

	perseg(vll& v, ll q = 0) : n(sz(v)) {
		nd.reserve(2*n + 18*(n+q));
		rt.reserve(q + 2);
		rt.push_back(cnt++);
		build(nd.data(), cnt, rt[0], 0, n-1, v);
	}
	static ll build(Node* d, ll& cnt, ll p, ll l, ll r, vll& v) {
		if (l == r) return d[p].val = v[l];
		d[p].l = cnt++; d[p].r = cnt++;
		ll m = (l+r)/2;
		return d[p].val = build(d, cnt, d[p].l, l, m, v) + build(d, cnt, d[p].r, m+1, r, v);
	}
	ll query(ll a, ll b, ll t) {
		return query(nd.data(), a, b, rt[t], 0, n-1);
	}
	static ll query(Node* d, ll a, ll b, ll p, ll l, ll r) {
		if (b < l || r < a) return 0;
		if (a <= l && r <= b) return d[p].val;
		ll m = (l+r)/2;
		return query(d, a, b, d[p].l, l, m) + query(d, a, b, d[p].r, m+1, r);
	}
	ll set(ll a, ll val, ll t) {
		ll nr = cnt++;
		set(nd.data(), cnt, a, val, rt[t], nr, 0, n-1);
		rt.push_back(nr);
		return (ll)rt.size()-1;
	}
	static ll set(Node* d, ll& cnt, ll a, ll val, ll lp, ll p, ll l, ll r) {
		if (l == r) return d[p].val = val;
		ll m = (l+r)/2;
		if (a <= m) {
			d[p].l = cnt++; d[p].r = d[lp].r;
			return d[p].val = set(d, cnt, a, val, d[lp].l, d[p].l, l, m) + d[d[p].r].val;
		}
		d[p].l = d[lp].l; d[p].r = cnt++;
		return d[p].val = d[d[p].l].val + set(d, cnt, a, val, d[lp].r, d[p].r, m+1, r);
	}
};