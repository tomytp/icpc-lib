// Hungarian algorithm — min-cost perfect matching
// a[i][j] = cost of assigning worker i to job j (n workers, m >= n jobs)
// Returns {min cost, assignment}: assignment[i] = j
pair<ll, vll> hungarian(const vector<vll> &a) {
	if (a.empty()) return {0, {}};
	ll n = sz(a) + 1, m = sz(a[0]) + 1;
	vll u(n), v(m), p(m), ans(n - 1);
	forn(i, 1, n) {
		p[0] = i;
		ll j0 = 0; // "dummy" worker 0
		vll dist(m, INF), pre(m, -1);
		vector<bool> done(m + 1);
		do { // dijkstra
			done[j0] = true;
			ll i0 = p[j0], j1 = 0, delta = INF;
			forn(j, 1, m) if (!done[j]) {
				auto cur = a[i0 - 1][j - 1] - u[i0] - v[j];
				if (cur < dist[j]) dist[j] = cur, pre[j] = j0;
				if (dist[j] < delta) delta = dist[j], j1 = j;
			}
			forn(j, 0, m) {
				if (done[j]) u[p[j]] += delta, v[j] -= delta;
				else dist[j] -= delta;
			}
			j0 = j1;
		} while (p[j0]);
		while (j0) { // update alternating path
			ll j1 = pre[j0];
			p[j0] = p[j1], j0 = j1;
		}
	}
	forn(j, 1, m) if (p[j]) ans[p[j] - 1] = j - 1;
	return {-v[0], ans}; // min cost
}
