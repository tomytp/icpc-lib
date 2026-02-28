#include "src/extra/template.cpp"
#include "src/graphs/articulation_points.cpp"

int main() {
    _;
    ll n, m; cin >> n >> m;
    vector<v64> g(n);
    forn(i, 0, m) {
        ll u, v; cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    TJ tj(n, g);
    vector<p64> bridges;
    for (auto [u, v] : tj.bridges)
        bridges.push_back({min(u, v), max(u, v)});
    sort(bridges.begin(), bridges.end());
    cout << sz(bridges) << ln;
    for (auto [u, v] : bridges) cout << u << " " << v << ln;
}
