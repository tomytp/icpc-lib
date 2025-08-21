// Mo's Algorithm (Offline Range Queries)
//
// Answers offline range queries by ordering them (block or Hilbert curve) to get small pointer movement and amortized updates.
//
// complexity: O((N + Q) sqrt N), O(N)

const ll MAX = 2e5+10;
const ll SQ = sqrt(MAX);
ll ans;

inline void insert(ll p) {
}

inline void erase(ll p) {
}

inline ll hilbert(ll x, ll y) {
	static ll N = 1 << (__builtin_clzll(0ll) - __builtin_clzll(MAX));
	ll rx, ry, s;
	ll d = 0;
	for (s = N/2; s > 0; s /= 2) {
		rx = (x & s) > 0, ry = (y & s) > 0;
		d += s * ll(s) * ((3 * rx) ^ ry);
		if (ry == 0) {
			if (rx == 1) x = N-1 - x, y = N-1 - y;
			swap(x, y);
		}
	}
	return d;
}

#define HILBERT true
vector<ll> MO(vector<pair<ll, ll>> &q) {
	ans = 0;
	ll m = q.size();
	vector<ll> ord(m);
	iota(ord.begin(), ord.end(), 0);
#if HILBERT
	vector<ll> h(m);
	for (ll i = 0; i < m; i++) h[i] = hilbert(q[i].first, q[i].second);
	sort(ord.begin(), ord.end(), [&](ll l, ll r) { return h[l] < h[r]; });
#else
	sort(ord.begin(), ord.end(), [&](ll l, ll r) {
		if (q[l].first / SQ != q[r].first / SQ) return q[l].first < q[r].first;
		if ((q[l].first / SQ) % 2) return q[l].second > q[r].second;
		return q[l].second < q[r].second;
	});
#endif
	vector<ll> ret(m);
	ll l = 0, r = -1;

	for (ll i : ord) {
		ll ql, qr;
		tie(ql, qr) = q[i];
		while (r < qr) insert(++r);
		while (l > ql) insert(--l);
		while (l < ql) erase(l++);
		while (r > qr) erase(r--);
		ret[i] = ans;
	}
	return ret;
}
