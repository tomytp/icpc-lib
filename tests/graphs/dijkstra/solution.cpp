#include "src/extra/template.cpp"
#include "src/graphs/dijkstra.cpp"

int main() {
    _;
    ll n, m, s; cin >> n >> m >> s;
    vector<vector<p64>> g(n);
    while (m--) {
        ll u, v, w; cin >> u >> v >> w;
        g[u].push_back({v, w});
    }
    v64 d, p;
    dijkstra(s, d, p, g);
    forn(i, 0, n) {
        cout << (d[i] >= INF ? -1 : d[i]);
        if (i < n - 1) cout << " ";
    }
    cout << ln;
}
