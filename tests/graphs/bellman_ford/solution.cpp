#include "src/extra/template.cpp"
#include "src/graphs/bellman_ford.cpp"

int main() {
    _;
    ll n, m, s; cin >> n >> m >> s;
    vector<tuple<ll,ll,ll>> edges(m);
    for (auto& [u, v, w] : edges) cin >> u >> v >> w;

    v64 d, p;
    bellman_ford(s, n, d, p, edges);
    forn(i, 0, n) {
        cout << d[i];
        if (i < n - 1) cout << " ";
    }
    cout << ln;
}
