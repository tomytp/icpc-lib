// Bellman-Ford Single-Source Shortest Paths
//
// Computes shortest paths from s on directed graphs with possibly negative
// edge weights. Nodes reachable via a negative cycle get dist = -INF.
//
// complexity: O(N*M), O(N + M)

// d = distance | p = from/path | edges: {u, v, w} directed
void bellman_ford(ll s, ll n, v64& d, v64& p, vector<tuple<ll,ll,ll>> edges) {
    d.assign(n, INF);
    p.assign(n, -1);
    d[s] = 0;

    sort(edges.begin(), edges.end(), [](auto& a, auto& b) {
        ll sa = get<0>(a) < get<1>(a) ? get<0>(a) : -get<0>(a);
        ll sb = get<0>(b) < get<1>(b) ? get<0>(b) : -get<0>(b);
        return sa < sb;
    });

    ll lim = n / 2 + 2; // use n-1 with unsorted edges
    forn(i, 0, lim) for (auto [u, v, w] : edges) {
        if (d[u] == INF || d[u] == -INF) continue;
        ll nd = d[u] + w;
        if (nd < d[v]) {
            p[v] = u;
            d[v] = (i < lim - 1 ? nd : -INF);
        }
    }
    forn(i, 0, lim) for (auto [u, v, w] : edges)
        if (d[u] == -INF) d[v] = -INF;
}
