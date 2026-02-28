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
    cout << sz(tj.bcc) << ln;
    for (auto& comp : tj.bcc) {
        cout << sz(comp) << ln;
        for (auto [u, v] : comp)
            cout << min(u, v) << " " << max(u, v) << ln;
    }
}
