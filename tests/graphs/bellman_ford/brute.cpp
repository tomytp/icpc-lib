#include "src/extra/template.cpp"

int main() {
    _;
    ll n, m, s; cin >> n >> m >> s;
    vector<tuple<ll,ll,ll>> edges(m);
    for (auto& [u, v, w] : edges) cin >> u >> v >> w;

    v64 d(n, INF);
    d[s] = 0;
    forn(i, 0, n - 1)
        for (auto [u, v, w] : edges)
            if (d[u] < INF) d[v] = min(d[v], d[u] + w);

    // detect nodes reachable from a negative cycle
    vector<bool> neg(n, false);
    forn(i, 0, n)
        for (auto [u, v, w] : edges)
            if (d[u] < INF && d[u] + w < d[v]) neg[v] = true;
    forn(i, 0, n)
        for (auto [u, v, w] : edges)
            if (neg[u]) neg[v] = true;
    forn(i, 0, n) if (neg[i]) d[i] = -INF;

    forn(i, 0, n) {
        cout << d[i];
        if (i < n - 1) cout << " ";
    }
    cout << ln;
}
