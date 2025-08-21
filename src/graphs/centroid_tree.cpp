// Centroid Tree
//
// Constroi a centroid tree
// p[i] eh o pai de i na centroid-tree
// dist[i][k] = distancia na arvore original entre i
// e o k-esimo ancestral na arvore da centroid
//
// O(n log(n)) de tempo e memoria

vector<v64> g(MAX), dist(MAX);
vector<ll> sz(MAX), rem(MAX), p(MAX);

ll dfs_sz(ll i, ll l=-1) {
	sz[i] = 1;
	for (ll j : g[i]) if (j != l and !rem[j]) sz[i] += dfs_sz(j, i);
	return sz[i];
}

ll centroid(ll i, ll l, ll size) {
	for (ll j : g[i]) if (j != l and !rem[j] and sz[j] > size / 2)
		return centroid(j, i, size);
	return i;
}

void dfs_dist(ll i, ll l, ll d=0) {
	dist[i].push_back(d);
	for (ll j : g[i]) if (j != l and !rem[j])
		dfs_dist(j, i, d+1);
}

void decomp(ll i, ll l = -1) {
	ll c = centroid(i, i, dfs_sz(i));
	rem[c] = 1, p[c] = l;
	dfs_dist(c, c);
	for (ll j : g[c]) if (!rem[j]) decomp(j, c);
}

void build(ll n) {
	forn(i,0,n) rem[i] = 0, dist[i].clear();
	decomp(0);
	forn(i,0,n) reverse(dist[i].begin(), dist[i].end());
}